; @file nsk_common_addr.inc
; @brief Include module for the 16-bit addresses manipulations
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_ADDR__
::__NSK_COMMON_ADDR__ = 1

.linecont +

.include "nsk_common_push.inc"

; @macro      _p16_set_a
; @brief      Sets a 16-bit pointer to the value of register A (low byte).
;
; @param      dst   Target pointer address (2 bytes) to write to.
;
; @details
; This macro assigns the current value of the A register to the low byte
; of the 16-bit destination address (`dst`), and sets the high byte to 0.
;
; ### Example
; ~~~
; lda #$52
; _p16_set_a $8022
; ~~~
; Results in memory:
; ~~~
; Address   Value
; $8022     $52    ; low byte (LSB)
; $8023     $00    ; high byte (MSB)
; ~~~
.macro _p16_set_a dst
    push a

    sta dst

    lda #$00
    sta dst + 1

    pull a
.endmacro

; @macro      _p16_set_x
; @brief      Sets a 16-bit pointer to the value of register X (low byte).
;
; @param      dst   Target pointer address (2 bytes) to write to.
;
; @details
; This macro assigns the current value of the X register to the low byte
; of the 16-bit destination address (`dst`), and sets the high byte to 0.
;
; ### Example
; ~~~
; ldx #$70
; _p16_set_x $7934
; ~~~
; Results in memory:
; ~~~
; Address   Value
; $7934     $70    ; low byte (LSB)
; $7935     $00    ; high byte (MSB)
; ~~~
.macro _p16_set_x dst
    push a

    stx dst

    lda #$00
    sta dst + 1

    pull a
.endmacro

; @macro      _p16_set_y
; @brief      Sets a 16-bit pointer to the value of register Y (low byte).
;
; @param      dst   Target pointer address (2 bytes) to write to.
;
; @details
; This macro assigns the current value of the Y register to the low byte
; of the 16-bit destination address (`dst`), and sets the high byte to 0.
;
; ### Example
; ~~~
; ldy #$a9
; _p16_set_y $b033
; ~~~
; Results in memory:
; ~~~
; Address   Value
; $b033     $a9    ; low byte (LSB)
; $b034     $00    ; high byte (MSB)
; ~~~
.macro _p16_set_y dst
    push a

    sty dst

    lda #$00
    sta dst + 1

    pull a
.endmacro

; @macro      _p16_set_8
; @brief      Sets a 16-bit pointer to an 8-bit value stored at `src`.
;
; @param      dst   Target pointer address (2 bytes) to write to.
; @param      src   8-bit value or address (immediate or memory) to assign.
;
; @details
; This macro reads an 8-bit value from `src` and stores it in the low byte
; of the 16-bit destination address (`dst`). The high byte is always set to 0.
;
; ### Example
; ~~~
; lda #$44
; sta $03
; _p16_set_8 $8022, $03
; ~~~
; Results in memory:
; ~~~
; Address   Value
; $8022     $44    ; low byte (LSB)
; $8023     $00    ; high byte (MSB)
; ~~~
.macro _p16_set_8 dst, src
    push a

    lda src
    sta dst

    lda #$00
    sta dst + 1

    pull a
.endmacro

; @macro      _p16_set_16i
; @brief      Sets a 16-bit pointer to a 16-bit immediate constant.
;
; @param      dst   Target pointer address (2 bytes) to write to.
; @param      src   16-bit immediate value (e.g. #$1234).
;
; @note
; Even though the value is treated as an immediate constant internally,
; the argument must be passed **without** the '#' prefix (e.g. `$1234`,
; not `#$1234`). This is required so the macro can extract both the low
; and high byte separately.
;
; @details
; This macro writes a 16-bit immediate value to the destination address.
; The low byte (LSB) is written first, followed by the high byte (MSB).
; The A register is preserved using push/pop.
;
; ### Example
; ~~~
; _p16_set_16i $0400, $C080
; ~~~
; Results in memory:
; ~~~
; Address   Value
; $0400     $80    ; low byte (LSB)
; $0401     $C0    ; high byte (MSB)
; ~~~
.macro _p16_set_16i dst, src
    push a

    lda #.lobyte(src)
    sta dst

    lda #.hibyte(src)
    sta dst + 1

    pull a
.endmacro

; @macro      _p16_set_16p
; @brief      Copies a 16-bit value from a memory location to a pointer.
;
; @param      dst   Target pointer address (2 bytes) to write to.
; @param      src   Source memory address containing a 16-bit value.
;
; @details
; This macro reads two bytes starting from `src`: the low byte at `src`
; and the high byte at `src + 1`. It writes them to `dst` and `dst + 1`,
; effectively copying a full 16-bit value.
;
; ### Example
; ~~~
; ; Assume memory:
; ; $0400 = $34
; ; $0401 = $12
;
; _p16_set_16p $8022, $0400
; ~~~
; Results in memory:
; ~~~
; Address   Value
; $8022     $34    ; low byte (LSB)
; $8023     $12    ; high byte (MSB)
; ~~~
.macro _p16_set_16p dst, src
    push a

    lda src
    sta dst

    lda src + 1
    sta dst + 1

    pull a
.endmacro

; @macro      nsk_p16_set
; @brief      Sets a 16-bit pointer from various 8-bit or 16-bit values.
;
; @param      dst     Target 2-byte address (pointer) to write to.
; @param      src     Value to assign. Can be a register, literal, address,
;                     or constant.
; @param      mode    Optional. Specifies how `src` should be interpreted.
;                     Can be `reg`, `8`, or `16`. Often inferred automatically.
;
; @details
; A polymorphic macro that sets the value of a 16-bit pointer (`dst`) using:
; - An 8-bit CPU register (`a`, `x`, `y`)
; - A literal 8-bit or 16-bit value
; - A variable or constant
; - A memory address containing a 16-bit word (little-endian)
;
; @note
; - If `src` and `mode` is omitted, the `src` is assumed as `a`.
; - If `mode` is omitted, it is inferred:
;    - If `src` is `a`, `x`, or `y`, the `mode` is assumed as `reg`
;    - If `src` is a memory address or 8-bit value, the `mode` is assumed as `8`
; - If the `src` should be interpreted as 16 bit value or a pointer to the 16
;   bit value, the `mode` is required to be explicitly set as `16`
;
; ### Example
; ~~~
; ; Assume values:
; ; Registers:
; ;   a   = $12
; ;   x   = $34
; ;   y   = $56
; ;
; ; Memory:
; ;   address: ... $12 $13 ... $7021 $7022 ...
; ;   value  :     $08 $09      $a0   $ff
; ;
; ; Constants:
; ;   ::aaa = $91
; ;   ::bbb = $8121
; ;   .enum ccc
; ;      ddd = $21
; ;      eee = $3456
; ;   .endenum
;
; | Called as..                  | $2140 | $2141 |
; |------------------------------|-------|-------|
; | p16_set $2140                |  $12  |  $00  |
; | p16_set $2140, a             |  $12  |  $00  |
; | p16_set $2140, a, reg        |  $12  |  $00  |
; | p16_set $2140, x             |  $34  |  $00  |
; | p16_set $2140, x, reg        |  $34  |  $00  |
; | p16_set $2140, y             |  $56  |  $00  |
; | p16_set $2140, y, reg        |  $56  |  $00  |
; | p16_set $2140, $12           |  $08  |  $00  |
; | p16_set $2140, $12, 8        |  $08  |  $00  |
; | p16_set $2140, #$ff          |  $ff  |  $00  |
; | p16_set $2140, #$ff,8        |  $ff  |  $00  |
; | p16_set $2140, $7021, 16     |  $a0  |  $ff  |
; | p16_set $2140, #$ba00,16     |  $ba  |  $00  |
; | p16_set $2140, $12, 16       |  $08  |  $09  |
; | p16_set $2140, #::aaa        |  $91  |  $00  |
; | p16_set $2140, #::aaa, 8     |  $91  |  $00  |
; | p16_set $2140, #::bbb, 16    |  $81  |  $21  |
; | p16_set $2140, #ccc::ddd     |  $21  |  $00  |
; | p16_set $2140, #ccc::ddd, 8  |  $21  |  $00  |
; | p16_set $2140, #ccc::eee, 16 |  $34  |  $56  |
; ~~~
.macro nsk_p16_set dst, src, mode
    .ifblank dst
        .error "P16_SET: first argument cannot be blank"
    .endif

    .if     (                               \
                (.blank ({src})) ||         \
                (.xmatch ({src}, a))        \
            ) && (                          \
                (.blank ({mode})) ||        \
                (.xmatch ({mode}, reg))     \
            )
        ; dst = a
        _p16_set_a dst

    .elseif (.xmatch ({src}, x)) &&         \
            (                               \
                (.blank ({mode})) ||        \
                (.xmatch ({mode}, reg))     \
            )
        ; dst = x
        _p16_set_x dst

    .elseif (.xmatch ({src}, y)) &&         \
            (                               \
                (.blank ({mode})) ||        \
                (.xmatch ({mode}, reg))     \
            )
        ; dst = y
        _p16_set_y dst

    .elseif (                               \
                (.blank ({mode})) .or       \
                (.xmatch ({mode}, 8))       \
            )
        ; dst = #$xx
        ; dst = #xx
        ; dst = $xx
        ; dst = $xxxx
        ; dst = 8-bit define
        ; dst = 8-bit enum
        _p16_set_8 dst, src

    .elseif (.xmatch ({mode}, 16))

        .if (.xmatch (.left (1, {src}), #))
            ; dst = #$xx   (implied as 16-bit)
            ; dst = #$xxxx
            ; dst = 16-bit define
            ; dst = 16-bit enum
            _p16_set_16i dst, .right (.tcount ({src}) - 1, {src})

        .else
            ; dst = xx     (zp-address of 16-bit value)
            ; dst = $xx    (zp-address of 16-bit value)
            ; dst = xxxx   (absolute address of 16-bit value)
            ; dst = $xxxx  (absolute address of 16-bit value)
            _p16_set_16p dst, src

        .endif

    .else
        .error "P16_SET: unknown mode"

    .endif
.endmacro

.endif