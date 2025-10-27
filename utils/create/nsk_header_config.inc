; @file nsk_header_config.inc
; @brief User configuration for NES 2.0 header generation.
;
; Edit this file to set ROM header parameters.
; All required constants and flags will be generated automatically.
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CONFIG__
::__NSK_HEADER_CONFIG__ = 1


.define NSK_SEGMENT_HEADER "HEADER"

::NSK_HEADER_PRGROM_SIZE = 1024

::NSK_HEADER_CHRROM_SIZE = 2048

::NSK_HEADER_PRGRAM_SIZE = 4096

::NSK_HEADER_CHRRAM_SIZE = 8192

::NSK_HEADER_PRGNVRAM_SIZE = 1024

::NSK_HEADER_CHRNVRAM_SIZE = 1024

::NSK_HEADER_REGION = nsk::region::Japan

::NSK_HEADER_VS_PPU = nsk::console::VS::ppu::not_applicable

::NSK_HEADER_VS_HARDWARE = nsk::console::VS::hardware::not_applicable

::NSK_HEADER_ROMS_MISC = 2

::NSK_HEADER_EXPANSION_DEVICE = nsk::device::konami::hypershot

::NSK_HEADER_NAMETABLE_HARDWIRED = nsk::nametable::layout::vertical_mirroring

::NSK_HEADER_BATTERY = nsk::battery::present

::NSK_HEADER_TRAINER = nsk::trainer::absent

::NSK_HEADER_NAMETABLE_ALTERNATIVE = nsk::nametable::mode::alternative

::NSK_HEADER_CONSOLE_TYPE = nsk::console::nintendo_vs_system

::NSK_HEADER_MAPPER_ID = 304

::NSK_HEADER_SUBMAPPER_ID = 0


.endif
