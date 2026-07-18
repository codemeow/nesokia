; @file nsk_header_defaults.inc
; @brief Include module for the default header values
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_DEFAULTS__
::__NSK_HEADER_DEFAULTS__ = 1

.if !.defined(NSK_HEADER_SEGMENT)
    .define NSK_SEGMENT_HEADER "HEADER"
.endif

.if !.defined(::NSK_HEADER_CONSOLE_TYPE)
    ::NSK_HEADER_CONSOLE_TYPE = NSKHW::CONSOLE::FAMICOM
.endif

.if !.defined(::NSK_HEADER_REGION)
    ::NSK_HEADER_REGION = NSKHW::REGION::NTSC
.endif

.if !.defined(::NSK_HEADER_MAPPER_ID)
    ::NSK_HEADER_MAPPER_ID = 0
.endif

.if !.defined(::NSK_HEADER_SUBMAPPER_ID)
    ::NSK_HEADER_SUBMAPPER_ID = 0
.endif

.if !.defined(::NSK_HEADER_NAMETABLE_HARDWIRED)
    ::NSK_HEADER_NAMETABLE_HARDWIRED = NSKHW::NM_LAYOUT::VERTICAL_ARRANGEMENT
.endif

.if !.defined(::NSK_HEADER_NAMETABLE_ALTERNATIVE)
    ::NSK_HEADER_NAMETABLE_ALTERNATIVE = NSKHW::NM_ALTERNATIVE::NORMAL
.endif

.if !.defined(::NSK_HEADER_BATTERY)
    ::NSK_HEADER_BATTERY = NSKHW::BATTERY::NOT_PRESENT
.endif

.if !.defined(::NSK_HEADER_TRAINER)
    ::NSK_HEADER_TRAINER = NSKHW::TRAINER::NOT_PRESENT
.endif

.if !.defined(::NSK_HEADER_PRGROM_SIZE)
    ::NSK_HEADER_PRGROM_SIZE = 0
.endif

.if !.defined(::NSK_HEADER_CHRROM_SIZE)
    ::NSK_HEADER_CHRROM_SIZE = 0
.endif

.if !.defined(::NSK_HEADER_PRGRAM_SIZE)
    ::NSK_HEADER_PRGRAM_SIZE = 0
.endif

.if !.defined(::NSK_HEADER_CHRRAM_SIZE)
    ::NSK_HEADER_CHRRAM_SIZE = 0
.endif

.if !.defined(::NSK_HEADER_PRGNVRAM_SIZE)
    ::NSK_HEADER_PRGNVRAM_SIZE = 0
.endif

.if !.defined(::NSK_HEADER_CHRNVRAM_SIZE)
    ::NSK_HEADER_CHRNVRAM_SIZE = 0
.endif

.if !.defined(::NSK_HEADER_ROMS_MISC)
    ::NSK_HEADER_ROMS_MISC = 0
.endif

.if !.defined(::NSK_HEADER_VS_PPU)
    ::NSK_HEADER_VS_PPU = NSKHW::VSPPU::NONE
.endif

.if !.defined(::NSK_HEADER_VS_HARDWARE)
    ::NSK_HEADER_VS_HARDWARE = NSKHW::VSHARDWARE::NONE
.endif

.if !.defined(::NSK_HEADER_EXPANSION_DEVICE)
    ::NSK_HEADER_EXPANSION_DEVICE = NSKHW::DEVICES::NONE::UNSPECIFIED
.endif


.endif