#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*!
 * \brief  Reads the U64 little endian data from file
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the value is successfully read
 */
bool nsk_wav_read_u64le(
    FILE     *file,
    uint64_t *value
);

/*!
 * \brief  Reads the U32 little endian data from file
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the value is successfully read
 */
bool nsk_wav_read_u32le(
    FILE     *file,
    uint32_t *value
);

/*!
 * \brief  Reads the i32 little endian data from file
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the value is successfully read
 */
bool nsk_wav_read_i32le(
    FILE    *file,
    int32_t *value
);

/*!
 * \brief  Reads the i24 little endian data from file
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the value is successfully read
 */
bool nsk_wav_read_i24le(
    FILE    *file,
    int32_t *value
);

/*!
 * \brief  Reads the u16 little endian data from file
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the value is successfully read
 */
bool nsk_wav_read_u16le(
    FILE     *file,
    uint16_t *value
);

/*!
 * \brief  Reads the i16 little endian data from file
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the value is successfully read
 */
bool nsk_wav_read_i16le(
    FILE     *file,
    int16_t *value
);

/*!
 * \brief  Reads the u8 data from file
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the value is successfully read
 */
bool nsk_wav_read_u8(
    FILE     *file,
    uint8_t *value
);

/*!
 * \brief  Reads the wav data and compares it with the provided data
 *
 * \param[in,out]  file  The file descriptor
 * \param[in]      size  Constant data size
 * \param[in]      data  Constant data
 * \return    True if the read data equals the provided
 */
bool nsk_wav_read_compare(
    FILE          *file,
    size_t         size,
    const uint8_t *data
);