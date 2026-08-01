; @file nsk_default_ppu.inc
; @brief Default PPU module configuration
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_DEFAULT_PPU__
::__NSK_DEFAULT_PPU__ = 1

.if !.defined(::NSK_FEATURE_PPU)
    ::NSK_FEATURE_PPU = 1
.endif

.if !.defined(::NSK_PPU_NMICONTROL)
    ::NSK_PPU_NMICONTROL = 1
.endif

.if !.defined(::NSK_PPU_RENDERCONTROL)
    ::NSK_PPU_RENDERCONTROL = 1
.endif

.endif
