; @file nsk_render_vars.inc
; @brief Include module for map rendering state variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_VARS__
::__NSK_RENDER_VARS__ = 1

.ifndef ::__NSK_RENDER_VARS_ASM__
    ; @brief Address for the selected map data
    .importzp _nsk_render_mapdata

    ; @brief Object address
    .importzp _nsk_render_object

    ; @brief Collision data pointer
    .importzp _nsk_render_collisionptr

    ; @brief Collision map pointer
    .importzp _nsk_render_mapptr

    ; @brief Address for the selected nametable
    .import _nsk_render_nametable

    ; @brief Attributes buffer
    .import _nsk_render_attrbuf

    ; @brief Object X
    .import _nsk_render_objx

    ; @brief Object Y
    .import _nsk_render_objy

    ; @brief Width of the object
    .import _nsk_render_width

    ; @brief Height of the object
    .import _nsk_render_height

    ; @brief Temporary offset address counter
    .import _nsk_render_offs

    ; @brief Palette X quadrant part
    .import _nsk_render_quadrant_posx

    ; @brief Palette Y quadrant part
    .import _nsk_render_quadrant_posy

    ; @brief Palette calculations: shift
    .import _nsk_render_quadrant_shift

    ; @brief Palette calculations: mask
    .import _nsk_render_quadrant_mask

    ; @brief Palette calculations: value
    .import _nsk_render_quadrant_value
.endif

.endif
