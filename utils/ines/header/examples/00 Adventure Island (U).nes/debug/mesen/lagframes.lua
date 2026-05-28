-- example00_lagframes.lua
-- Mesen 2.x Lua script for Example 00.
-- Counts frames where NMI happened before main loop reached its sleep point.

local NMI_SLEEP_FLAG = 0x0036

local mem =
    (emu.memType and emu.memType.nesInternalRam) or
    (emu.memType and emu.memType.cpuDebug) or
    (emu.memType and emu.memType.cpu)

local eventEndFrame =
    (emu.eventType and emu.eventType.endFrame)

local eventNmi =
    (emu.eventType and emu.eventType.nmi)

local lagFrames = 0
local nmiFrames = 0
local measuredFrames = 0
local warmupNmis = 8
local lastLag = false
local lastLagFlash = 0

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

local function resetCounters()
    lagFrames = 0
    nmiFrames = 0
    measuredFrames = 0
    warmupNmis = 8
    lastLag = false
    lastLagFlash = 0
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
        return
    end

    measuredFrames = measuredFrames + 1

    if flag == 0 then
        lagFrames = lagFrames + 1
        lastLag = true
        lastLagFlash = 20
    else
        lastLag = false
    end
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

emu.addEventCallback(drawOverlay, eventEndFrame)

emu.displayMessage("Example 00", "Lag frame overlay enabled")
