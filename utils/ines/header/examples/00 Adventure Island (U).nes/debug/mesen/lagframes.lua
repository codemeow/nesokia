-- example00_lagframes.lua
-- Mesen 2.x Lua script for Example 00.
-- Counts frames where NMI happened before main loop reached its sleep point.

-- Keep this in sync with example.dbg:
--   sym ... name="nsk_nmi_sleep_flag" ... val=0x34
-- Zeropage layout changes can move this address.
local NMI_SLEEP_FLAG = 0x0034

local mem =
    (emu.memType and emu.memType.nesInternalRam) or
    (emu.memType and emu.memType.cpuDebug) or
    (emu.memType and emu.memType.cpu)

local eventEndFrame =
    (emu.eventType and emu.eventType.endFrame)

local eventNmi =
    (emu.eventType and emu.eventType.nmi)

local callbackWrite =
    (emu.callbackType and emu.callbackType.write)

local GRAPH_RECORDS = 250
local GRAPH_X = 3
local GRAPH_Y = 174
local GRAPH_W = 250
local GRAPH_H = 58
local GRAPH_SCANLINES = 262
local GRAPH_VBLANK_SCANLINE = 241

local lagFrames = 0
local nmiFrames = 0
local measuredFrames = 0
local warmupNmis = 8
local lastLag = false
local lastLagFlash = 0
local frameDoneScanline = nil
local frameDoneRecorded = false
local scanlineHistory = {}

local prevResetKey = false
local drawCalls = 0
local drawErrorLogged = false

local function read8(addr)
    return emu.read(addr, mem)
end

local function selectConsoleScreen()
    pcall(function()
        emu.selectDrawSurface(emu.drawSurface.consoleScreen)
    end)
end

local function drawBox(x, y, w, h, color)
    pcall(function()
        emu.drawRectangle(x, y, w, h, color, true, 1, 0)
    end)
end

local function drawLine(x1, y1, x2, y2, color)
    pcall(function()
        emu.drawLine(x1, y1, x2, y2, color, 1)
    end)
end

local function drawText(x, y, text, fg, bg)
    -- Mesen 2.x signature.
    local ok = pcall(function()
        emu.drawString(x, y, text, fg, bg, 0, 1, 0)
    end)

    -- Old Mesen fallback.
    if not ok then
        ok = pcall(function()
            emu.drawString(x, y, text, fg, 1)
        end)
    end

    if not ok and not drawErrorLogged then
        drawErrorLogged = true
        emu.log("lagframes.lua: emu.drawString failed")
    end
end

local function currentScanline()
    local ok, state = pcall(function()
        return emu.getState()
    end)

    if ok and state and state["ppu.scanline"] ~= nil then
        return state["ppu.scanline"]
    end

    return nil
end

local function pushScanlineRecord(scanline, isLag)
    table.insert(scanlineHistory, 1, {
        scanline = scanline,
        lag = isLag
    })

    while #scanlineHistory > GRAPH_RECORDS do
        table.remove(scanlineHistory)
    end
end

local function resetCounters()
    lagFrames = 0
    nmiFrames = 0
    measuredFrames = 0
    warmupNmis = 8
    lastLag = false
    lastLagFlash = 0
    frameDoneScanline = nil
    frameDoneRecorded = false
    scanlineHistory = {}
end

local function onSleepFlagWrite(address, value)
    -- nsk_main marks the frame loop as complete with:
    --   inc nsk_nmi_sleep_flag
    --
    -- 6502 INC does a dummy write with the old value first, so only the
    -- non-zero write is the actual "main reached sleep" signal. NMI writes
    -- zero to reset the flag and is ignored here.
    if value == 0 or frameDoneRecorded then
        return
    end

    frameDoneScanline = currentScanline()
    frameDoneRecorded = true
end

local function onNmi()
    -- If the flag is still 0 when NMI starts, main did not reach:
    --   inc nsk_nmi_sleep_flag
    --   sleep...
    -- since the previous NMI.
    --
    -- Do not detect this via write callbacks on nsk_nmi_sleep_flag:
    -- 6502 INC is a read-modify-write instruction and performs a dummy
    -- write of the old value before writing the incremented value.
    local flag = read8(NMI_SLEEP_FLAG)

    nmiFrames = nmiFrames + 1

    if warmupNmis > 0 then
        warmupNmis = warmupNmis - 1
        frameDoneScanline = nil
        frameDoneRecorded = false
        return
    end

    measuredFrames = measuredFrames + 1

    if flag == 0 then
        lagFrames = lagFrames + 1
        lastLag = true
        lastLagFlash = 20
        pushScanlineRecord(GRAPH_SCANLINES - 1, true)
    else
        lastLag = false
        pushScanlineRecord(frameDoneScanline or 0, false)
    end

    frameDoneScanline = nil
    frameDoneRecorded = false
end

local function graphY(scanline)
    if scanline < 0 then
        scanline = 0
    elseif scanline >= GRAPH_SCANLINES then
        scanline = GRAPH_SCANLINES - 1
    end

    return GRAPH_Y + math.floor(scanline * (GRAPH_H - 1) / (GRAPH_SCANLINES - 1))
end

local function drawScanlineGraph()
    local bg = 0x44000000
    local grid = 0x66505050
    local vblank = 0x99F0D050
    local okColor = 0xDD40E060
    local warnColor = 0xDDF0D050
    local lagColor = 0xDDE04040

    drawBox(GRAPH_X - 1, GRAPH_Y - 1, GRAPH_W + 2, GRAPH_H + 2, bg)
    drawLine(GRAPH_X, GRAPH_Y, GRAPH_X + GRAPH_W, GRAPH_Y, grid)
    drawLine(GRAPH_X, GRAPH_Y + GRAPH_H - 1, GRAPH_X + GRAPH_W, GRAPH_Y + GRAPH_H - 1, grid)

    local midY = graphY(120)
    local vblankY = graphY(GRAPH_VBLANK_SCANLINE)
    drawLine(GRAPH_X, midY, GRAPH_X + GRAPH_W, midY, grid)
    drawLine(GRAPH_X, vblankY, GRAPH_X + GRAPH_W, vblankY, vblank)

    local prevX = nil
    local prevY = nil
    local prevLag = false

    for i, record in ipairs(scanlineHistory) do
        local x = GRAPH_X + GRAPH_W - i
        if x < GRAPH_X then
            break
        end

        local y = graphY(record.scanline)
        local color = okColor

        if record.lag then
            color = lagColor
        elseif record.scanline >= GRAPH_VBLANK_SCANLINE then
            color = warnColor
        end

        drawLine(x, y, x, y, color)

        if prevX and not prevLag and not record.lag then
            drawLine(prevX, prevY, x, y, color)
        end

        prevX = x
        prevY = y
        prevLag = record.lag
    end

    local latest = scanlineHistory[1]
    local latestText = "line: --"
    if latest then
        if latest.lag then
            latestText = "line: LAG"
        else
            latestText = string.format("line: %03d", latest.scanline)
        end
    end

    drawText(GRAPH_X + 4, GRAPH_Y + 4, latestText, 0xFFFFFF, 0x000000)
    drawText(GRAPH_X + 4, GRAPH_Y + GRAPH_H - 12, "vblank", 0xF0D050, 0x000000)
end

local function drawOverlay()
    drawCalls = drawCalls + 1
    selectConsoleScreen()

    local total = math.max(1, measuredFrames)
    local rate = (lagFrames * 10000 / total)
    local rateInt = math.floor(rate)
    local rateText = string.format("%d.%02d%%", math.floor(rateInt / 100), rateInt % 100)

    local flag = read8(NMI_SLEEP_FLAG)
    local status = lastLag and "LAG" or "OK"

    local fg = 0xFFFFFF
    local bg = lastLagFlash > 0 and 0x600000 or 0x000000

    drawBox(0, 0, 120, 58, bg)
    drawText(4, 4,  string.format("lag: %d", lagFrames), fg, bg)
    drawText(4, 14, string.format("nmi: %d", nmiFrames), fg, bg)
    drawText(4, 24, string.format("rate: %s", rateText), fg, bg)
    drawText(4, 34, string.format("last: %s  flag:$%02X", status, flag), fg, bg)
    drawText(4, 44, string.format("draw: %d  R: reset", drawCalls), 0xB0B0B0, bg)

    drawScanlineGraph()

    if lastLagFlash > 0 then
        lastLagFlash = lastLagFlash - 1
    end

    local resetKey = false
    pcall(function()
        resetKey = emu.isKeyPressed("R")
    end)

    if resetKey and not prevResetKey then
        resetCounters()
    end
    prevResetKey = resetKey
end

if eventNmi then
    emu.addEventCallback(onNmi, eventNmi)
else
    emu.log("lagframes.lua: emu.eventType.nmi is not available")
end

if callbackWrite then
    emu.addMemoryCallback(onSleepFlagWrite, callbackWrite, NMI_SLEEP_FLAG)
else
    emu.log("lagframes.lua: emu.callbackType.write is not available")
end

emu.addEventCallback(drawOverlay, eventEndFrame)

emu.displayMessage("Example 00", "Lag/scanline overlay enabled")
