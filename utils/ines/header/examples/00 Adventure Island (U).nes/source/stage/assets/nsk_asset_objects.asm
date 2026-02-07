; @file nsk_asset_objects.asm
; @brief Source module for the objects assets
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_ASSET_OBJECTS_ASM__
::__NSK_ASSET_OBJECTS_ASM__ = 1

.include "../../stage/assets/nsk_asset_objects.inc"

.segment "RODATA"

.export nsk_object_skybox0
nsk_object_skybox0:
    ; WxH
    .byte 8, 8
    ; Data (empty)
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    ; Palette (4x4)
    .byte 1,1,1,1
    .byte 1,1,1,1
    .byte 1,1,1,1
    .byte 1,1,1,1

.export nsk_object_skybox1
nsk_object_skybox1:
    ; WxH
    .byte 8, 8
    ; Data (3 stars: $01 x1, $03 x1, $10 x1)
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$01,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$10,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $03,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    ; Palette (4x4)
    .byte 1,1,1,1
    .byte 1,1,1,1
    .byte 1,1,1,1
    .byte 1,1,1,1

.export nsk_object_skybox2
nsk_object_skybox2:
    ; WxH
    .byte 8, 8
    ; Data
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $11,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$01
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$05,$00,  $00,$00,$00,$00
    ; Palette (4x4)
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1

.export nsk_object_skybox3
nsk_object_skybox3:
    ; WxH
    .byte 8, 8
    ; Data
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$12,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$10,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$02,$00,$00,  $00,$00,$00,$00
    ; Palette (4x4)
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1

.export nsk_object_skybox4
nsk_object_skybox4:
    ; WxH
    .byte 8, 8
    ; Data
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$00,  $00,$00,$10,$00
    .byte $00,$00,$00,$00,  $00,$00,$00,$00
    .byte $00,$00,$00,$13,  $00,$01,$00,$00
    ; Palette (4x4)
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1
    .byte 1, 1, 1, 1

.export nsk_object_base_a
nsk_object_base_a:
    .byte 8, 2
    .byte $f3,$e4,$e4,$e3, $e4,$e4,$e4,$e4
    .byte $00,$f6,$e4,$e4, $e4,$e5,$e4,$e4
    .byte 2,2,2,2
.export nsk_object_base_b
nsk_object_base_b:
    .byte 8, 2
    .byte $e4,$e5,$e4,$e4, $e4,$e4,$e4,$f5
    .byte $e4,$e4,$e4,$e4, $e4,$d4,$f7,$00
    .byte 2,2,2,2
.export nsk_object_base_c
nsk_object_base_c:
    .byte 8, 2
    .byte $e4,$e4,$e3,$e4, $e4,$e4,$d4,$e4
    .byte $e4,$e4,$e4,$e5, $e4,$e4,$e4,$e4
    .byte 2,2,2,2

.export nsk_object_tube_4v
nsk_object_tube_4v:
    .byte 4,8
    .byte $e0,$f4,$e4,$e2
    .byte $e0,$e4,$e4,$e2
    .byte $e0,$e4,$d4,$e2
    .byte $e0,$e4,$e4,$e2

    .byte $e0,$e4,$e4,$e2
    .byte $e0,$e3,$e4,$e2
    .byte $e0,$e4,$e4,$e2
    .byte $e0,$e4,$e4,$e2
    .byte 2,2
    .byte 2,2
    .byte 2,2
    .byte 2,2

.export nsk_object_tube_4h
nsk_object_tube_4h:
    .byte 8,4
    .byte $d1,$d1,$d1,$d1, $d1,$d1,$d1,$d1
    .byte $e4,$e3,$e4,$e4, $e4,$e4,$e4,$e4
    .byte $e4,$e4,$e3,$e4, $e4,$e4,$f4,$e4
    .byte $e4,$e4,$e4,$e4, $e5,$e4,$e4,$e4
    .byte 2,2,2,2
    .byte 2,2,2,2

.export nsk_object_tophat
nsk_object_tophat:
    .byte 8,4
    .byte $d3,$d1,$d1,$d1, $d1,$d1,$d1,$d5
    .byte $f3,$e4,$e4,$e3, $e4,$e4,$e4,$f5
    .byte $00,$f6,$e4,$e4, $e4,$e5,$f7,$00
    .byte $00,$00,$e0,$e4, $e4,$e2,$00,$00
    .byte 2,2,2,2
    .byte 2,2,2,2

.export nsk_object_branch_l
nsk_object_branch_l:
    .byte 8,4
    .byte $d3,$d1,$d1,$d1, $d1,$d1,$e4,$e4
    .byte $e0,$e4,$e5,$f1, $f1,$e4,$d4,$e4
    .byte $e0,$e4,$f7,$e8, $00,$f6,$e4,$e4
    .byte $f3,$f5,$00,$f8, $00,$00,$e0,$e4
    .byte 2,2,2,2
    .byte 2,2,2,2

.export nsk_object_branch_r
nsk_object_branch_r:
    .byte 8,4
    .byte $e4,$e4,$d1,$d1, $d1,$d1,$d1,$d5
    .byte $e3,$e4,$e4,$f1, $f1,$e4,$f4,$e2
    .byte $e4,$e4,$f7,$ea, $fa,$f6,$e4,$e2
    .byte $e4,$e2,$00,$00, $00,$00,$f3,$f5
    .byte 2,2,2,2
    .byte 2,2,2,2

.export nsk_object_hills_l
nsk_object_hills_l:
    .byte 8,2
    .byte $d3,$d1,$d1,$d2, $00,$e6,$e4,$e4
    .byte $e0,$e4,$e4,$e4, $d1,$e4,$d4,$e4
    .byte 2,2,2,2

.export nsk_object_hills_r
nsk_object_hills_r:
    .byte 8,2
    .byte $e4,$e4,$e7,$d3, $d1,$d1,$d1,$d5
    .byte $e4,$e4,$e4,$e4, $e3,$e4,$e4,$e2
    .byte 2,2,2,2

.export nsk_object_platform_l
nsk_object_platform_l:
    .byte 4,4
    .byte $d3,$d1,$d1,$d1
    .byte $e0,$e3,$e4,$e5
    .byte $f3,$f1,$f1,$e4
    .byte $00,$00,$00,$f6
    .byte 2,2
    .byte 2,2

.export nsk_object_platform_r
nsk_object_platform_r:
    .byte 8,4
    .byte $d1,$d1,$d1,$d1, $d5,$00,$00,$00
    .byte $e4,$e4,$e4,$e4, $e4,$e7,$00,$00
    .byte $e4,$d4,$e1,$e4, $e4,$e4,$d5,$00
    .byte $e4,$e4,$e4,$e4, $e4,$f4,$e4,$e7
    .byte 2,2,2,2
    .byte 2,2,2,2

.export nsk_object_cave_l
nsk_object_cave_l:
    .byte 4,8
    .byte $00,$d0,$d1,$d1
    .byte $d3,$d4,$e4,$f1
    .byte $e0,$e4,$f7,$ea
    .byte $e0,$e2,$00,$00

    .byte $f3,$e4,$d2,$00
    .byte $00,$e0,$e4,$d2
    .byte $00,$e0,$f4,$e2
    .byte $e6,$e4,$e4,$e2
    .byte 2,2
    .byte 2,2
    .byte 2,2
    .byte 2,2

.export nsk_object_cave_r
nsk_object_cave_r:
    .byte 4,8
    .byte $d1,$d1,$d1,$d5
    .byte $f1,$e4,$e3,$e2
    .byte $00,$f6,$e4,$e2
    .byte $00,$00,$e0,$e2

    .byte $00,$e6,$e4,$f5
    .byte $d3,$e4,$f7,$00
    .byte $e0,$e4,$e7,$00
    .byte $e0,$e4,$e4,$d5
    .byte 2,2
    .byte 2,2
    .byte 2,2
    .byte 2,2

.export nsk_object_cave_c
nsk_object_cave_c:
    .byte 8,4
    .byte $d1,$d1,$d1,$d1, $d1,$d1,$d1,$d1
    .byte $f1,$f1,$db,$dc, $dd,$de,$f1,$f1
    .byte $00,$e8,$f0,$f1, $f1,$f5,$e9,$fa
    .byte $00,$f8,$00,$00, $00,$00,$f9,$00
    .byte 2,2,2,2
    .byte 2,2,2,2

.export nsk_object_cave_b
nsk_object_cave_b:
    .byte 8,2
    .byte $d1,$d1,$d1,$d1, $d1,$d1,$d1,$d1
    .byte $20,$20,$20,$20, $20,$20,$20,$20
    .byte 0,0,0,0

.export nsk_object_glass_8
nsk_object_glass_8:
    .byte 8,2
    .byte $a0,$a1,$a3,$a2, $a3,$a1,$a2,$a4
    .byte $00,$00,$00,$00, $00,$00,$00,$00
    .byte 0,0,0,0

.export nsk_object_glass_4
nsk_object_glass_4:
    .byte 4,2
    .byte $a0,$a1,$a3,$a4
    .byte $00,$00,$00,$00
    .byte 0,0

.export nsk_object_bg_line
nsk_object_bg_line:
    .byte 4,2
    .byte $c2,$c2,$c2,$c2
    .byte $00,$00,$00,$00
    .byte 2,2

.export nsk_object_bg_up
nsk_object_bg_up:
    .byte 2,4
    .byte $00,$b0
    .byte $b2,$c0
    .byte $c0,$00
    .byte $00,$00
    .byte 2
    .byte 2

.export nsk_object_bg_down
nsk_object_bg_down:
    .byte 2,4
    .byte $b1,$00
    .byte $c1,$b3
    .byte $00,$c1
    .byte $00,$00
    .byte 2
    .byte 2

.export nsk_object_void
nsk_object_void:
    .byte 2,2
    .byte $00,$00
    .byte $00,$00
    .byte 2

.endif