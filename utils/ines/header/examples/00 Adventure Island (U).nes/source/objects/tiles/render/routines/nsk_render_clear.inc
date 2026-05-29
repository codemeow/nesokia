; @file nsk_render_clear.inc
; @brief Include module for map rendering clear routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_CLEAR__
::__NSK_RENDER_CLEAR__ = 1

.ifndef ::__NSK_RENDER_CLEAR_ASM__
    ; @brief Clears the nametables' attributes buffer
    .import _render_attrbuf_clear

    ; @brief Clears the nametables' tiles
    .import _render_nametable_clear

    ; @brief Resets the collision data
    .import _nsk_render_collision_clear
.endif

.endif
