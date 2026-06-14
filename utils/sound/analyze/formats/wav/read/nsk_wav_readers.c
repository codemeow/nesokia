#include <string.h>

#include <nsk_util_meta.h>

#include "nsk_wav_readers.h"

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
) {
    if (fread(value, sizeof(*value), 1, file) != 1) {
        nsk_err(
            "Cannot read value of size %zu from WAV file\n",
            sizeof(*value)
        );
        return false;
    }

    if (NSK_END_IS_BE) {
        *value = nsk_swap_uint64(*value);
    }

    return true;
}

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
) {
    if (fread(value, sizeof(*value), 1, file) != 1) {
        nsk_err(
            "Cannot read value of size %zu from WAV file\n",
            sizeof(*value)
        );
        return false;
    }

    if (NSK_END_IS_BE) {
        *value = nsk_swap_uint32(*value);
    }

    return true;
}

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
) {
    if (fread(value, sizeof(*value), 1, file) != 1) {
        nsk_err(
            "Cannot read value of size %zu from WAV file\n",
            sizeof(*value)
        );
        return false;
    }

    if (NSK_END_IS_BE) {
        *value = nsk_swap_int32(*value);
    }

    return true;
}

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
) {
    uint8_t buf[3];
    if (fread(buf, sizeof(buf), 1, file) != 1) {
        nsk_err(
            "Cannot read value of size %u from WAV file\n",
            3
        );
        return false;
    }

    uint32_t uvalue = 0;
    uvalue =
        (buf[2] << 16) |
        (buf[1] << 8 ) |
         buf[0];

    /* Sign extension */
    if (uvalue &  0x00800000u) {
        uvalue |= 0xff000000u;
    }

    *value = uvalue;

    return true;
}

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
) {
    if (fread(value, sizeof(*value), 1, file) != 1) {
        nsk_err(
            "Cannot read value of size %zu from WAV file\n",
            sizeof(*value)
        );
        return false;
    }

    if (NSK_END_IS_BE) {
        *value = nsk_swap_uint16(*value);
    }

    return true;
}

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
) {
    if (fread(value, sizeof(*value), 1, file) != 1) {
        nsk_err(
            "Cannot read value of size %zu from WAV file\n",
            sizeof(*value)
        );
        return false;
    }

    if (NSK_END_IS_BE) {
        *value = nsk_swap_int16(*value);
    }

    return true;
}

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
) {
    if (fread(value, sizeof(*value), 1, file) != 1) {
        nsk_err(
            "Cannot read value of size %zu from WAV file\n",
            sizeof(*value)
        );
        return false;
    }

    return true;
}

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
) {
    uint8_t buf[size];
    if (fread(buf, size, 1, file) != 1) {
        nsk_err("Cannot read data of size %zu from WAV file\n", size);
        return false;
    }

    return memcmp(buf, data, size) == 0;
}
