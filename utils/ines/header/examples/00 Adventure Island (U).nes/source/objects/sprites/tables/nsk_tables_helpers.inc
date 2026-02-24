; @file nsk_tables_helpers.inc
; @brief Include module for the sprite objects table helpers
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TABLES_HELPERS__
::__NSK_TABLES_HELPERS__ = 1

; @brief Resets the table counters
.macro _NSK_TABLE_START
    _nsk_table_counter .set 0
.endmacro

; @brief Defines the table line and checks it against the enum element
;
; @param[in] enum     The enum element
; @param[in] function The corresponding table function
.macro _NSK_TABLE_LINE enum, function
    .assert enum = _nsk_table_counter, error, "Table index mismatch"
    .addr function
    _nsk_table_counter .set _nsk_table_counter + 1
.endmacro

; @brief Checks the table size against the enum's last element value
;
; @param[in] size Expected size of the table
.macro _NSK_TABLE_COUNT size
    .assert _nsk_table_counter = size, error, "Table size mismatch"
.endmacro

.endif