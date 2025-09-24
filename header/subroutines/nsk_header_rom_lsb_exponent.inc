; @file nsk_header_romlsb_exponent.asm
; @brief Include module for the ROM LSB byte calculations for the exponential
;   encoding
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_ROM_LSB_EXPONENT__
::__NSK_HEADER_ROM_LSB__ = 1

.linecont +

; @def NSK_HEADER_EXPONENT_LOGLOOP_LIMIT
; @brief Upper bound for exponent search when encoding ROM size (NES 2.0).
;
; Limits the assembly-time loop that searches for the exponent E in the
; representation `size = (2^E) * (M * 2 + 1)`. Raising this value increases
; the risk of compile error; lowering it may cause valid sizes to be rejected.
::_NSK_HEADER_EXPONENT_LOGLOOP_LIMIT = $1f

; @def _NSK_HEADER_MULTIPLIER_NOTFOUND
; @brief Special constant to express the uninitialized value of the multiplier
::_NSK_HEADER_MULTIPLIER_NOTFOUND = $ff

; @def _NSK_HEADER_EXPONENT_NOTFOUND
; @brief Special constant to express the uninitialized value of the exponent
::_NSK_HEADER_EXPONENT_NOTFOUND   = $ff

; @macro _exponent_loop
; @brief Validate a dyadic candidate and, if valid, report exponent/multiplier.
;
; @param    multiplier      Candidate multiplier MM (∈ {0,1,2,3}).
; @param    dyadic          Candidate power-of-two factor 
;                           (∈ [0.._NSK_HEADER_EXPONENT_LOGLOOP_LIMIT]).
; @param    out_exponent    Symbol to receive the exponent E
; @param    out_multiplier  Symbol to receive the normalized multiplier
; @param    out_found       Symbol set to 1 if a match was found, 0 otherwise.
.macro _exponent_loop multiplier, dyadic, out_exponent, out_multiplier, out_found
    exponent_loop:
        ; Find E = log2(_dyadic) by repeated doubling.
        .repeat ::_NSK_HEADER_EXPONENT_LOGLOOP_LIMIT, _loop_exponent
            .if _dyadic = (1 << _loop_exponent)
                out_exponent .set _loop_exponent
            .endif
        .endrepeat

    exponent_check:
        ; If we matched some 1<<n, we have E and MM.
        .if _exponent <> ::_NSK_HEADER_EXPONENT_NOTFOUND
            out_multiplier .set multiplier
            out_found .set 1
        .endif
.endmacro

; @macro _dyadic_check
; @brief Compute and test a dyadic candidate, then try to derive (E, MM).
;
; @param    multiplier      Candidate multiplier MM (∈ {0,1,2,3}).
; @param    rom_size        ROM size (bytes)
; @param    divisor         Divisor used to produce the dyadic candidate.
; @param    out_exponent    Symbol to receive the exponent E
; @param    out_multiplier  Symbol to receive the multiplier MM
; @param    out_found       Symbol set to 1 if a match was found, 0 otherwise.
.macro _dyadic_check multiplier, rom_size, divisor, out_exponent, out_multiplier, out_found
    dyadic_set:
        ; Candidate 2^E (must be a power of two)
         _dyadic .set rom_size / _divisor

    dyadic_check:
        ; Power-of-two test:
        ;   x is power of two ⇔ x>0 and (x & (x-1)) == 0
        ; Rationale: a power of two has a single 1-bit; subtracting 1 flips
        ; all lower bits, so the AND is zero if there was exactly one 1-bit.
        .if (_dyadic & (_dyadic - 1)) = 0
            
            ; Find the exponent value or fail
            _exponent_loop          \
                multiplier,         \
                _dyadic,            \
                out_exponent,       \
                out_multiplier,     \
                out_found
            
        .endif
.endmacro

; @macro _multiplier_check
; @brief Try factoring `rom_size` against the given multiplier MM.
;
; @param    multiplier      Candidate multiplier MM (∈ {0,1,2,3}).
; @param    rom_size        ROM size (bytes)
; @param    out_exponent    Symbol to receive the exponent E
; @param    out_multiplier  Symbol to receive the multiplier MM
; @param    out_found       Symbol set to 1 if a match was found, 0 otherwise.
.macro _multiplier_check multiplier, rom_size, out_exponent, out_multiplier, out_found
    divisor_set:
        _divisor .set multiplier * 2 + 1

    divisor_check:
        ; If this odd factor divides the size, the quotient may be 2^E.
        .if rom_size .mod _divisor = 0
            
            ; Check the 2-adic value for this multiplier
            _dyadic_check           \
                multiplier,         \
                rom_size,           \
                _divisor,           \
                out_exponent,       \
                out_multiplier,     \
                out_found
        
        .endif
.endmacro

; @macro nsk_header_romlsb_exponent
; @brief Compute the NES 2.0 ROM-size LSB (exponent–multiplier encoding).
;
; Encodes `rom_size` using the NES 2.0 exponent–multiplier scheme and writes
; the least significant size byte as `(E << 2) | MM`, where E is the exponent
; of the power-of-two factor and MM ∈ {0..3} encodes the multiplier.
;
; @param rom_size   ROM size in bytes (constant expression).
; @param out_result Symbol that will be set to the resulting LSB value.
;
; @error Emits a build error if `rom_size` cannot be expressed as NES 2.0 format
.macro nsk_header_romlsb_exponent rom_size, out_result
    const_check:
        .ifnconst rom_size
            .error "`rom_size` must be a constant expression"
        .endif

    vars_init:
        ; Multiplier (MM) ∈ {0,1,2,3}
        _multiplier    .set ::_NSK_HEADER_MULTIPLIER_NOTFOUND
        ; Exponent (E) ∈ [0..63]
        _exponent      .set ::_NSK_HEADER_EXPONENT_NOTFOUND
        ; Skip flag for the excessive iterations
        _found    .set 0

    multiplier_loop:
        .repeat 4, _loop_multiplier

            .if _found = 0
                _multiplier_check           \
                    _loop_multiplier,       \
                    rom_size,               \
                    _exponent,              \
                    _multiplier,            \
                    _found
            .endif

        .endrepeat

    error_check:
        .if _found = 0
            ; Either the size isn't of the form 2^E×(1|3|5|7), 
            ; or the exponent cap is too small.
            .error                                                          \
                .sprintf(                                                   \
                    "Size must be encodable via NES 2.0 encoding (see %s)", \
                    "https://www.nesdev.org/wiki/NES_2.0#PRG-ROM_Area"      \
                )
        .endif

    write:
        out_result .set (_exponent << 2) | _multiplier
.endmacro

.delmacro _exponent_loop
.delmacro _dyadic_check
.delmacro _multiplier_check

.endif
