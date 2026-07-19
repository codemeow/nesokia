; @file nsk_default_nmi.inc
; @brief Default NMI processor configuration
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_DEFAULT_NMI__
::__NSK_DEFAULT_NMI__ = 1

.include "nsk_common_meta.inc"

.if !.defined(::NSK_NMI_UPDATEPPUCTRL)
    ::NSK_NMI_UPDATEPPUCTRL = 1
.endif

.if !.defined(::NSK_NMI_UPDATEPPUMASK)
    ::NSK_NMI_UPDATEPPUMASK = 1
.endif

.if !.defined(::NSK_NMI_UPDATEPPUSCROLL)
    ::NSK_NMI_UPDATEPPUSCROLL = 1
.endif

.if !.defined(::NSK_NMI_UPDATESPRITES)
    ::NSK_NMI_UPDATESPRITES = 1
.endif

.if !.defined(::NSK_NMI_UPDATESPRITES_ADDR)
    ::NSK_NMI_UPDATESPRITES_ADDR = NSK::CPU::RAM::OAMBUFFER
.endif

.endif