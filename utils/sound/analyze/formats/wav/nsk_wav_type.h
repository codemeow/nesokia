#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*!
 * \brief  WAV audio format values
 */
enum nsk_wav_audioformat {
    NSK_WAV_FORMAT_UNKNOWN    = 0x0000,
    NSK_WAV_FORMAT_PCM        = 0x0001,
    NSK_WAV_FORMAT_ADPCM      = 0x0002,
    NSK_WAV_FORMAT_IEEE_FLOAT = 0x0003,
    NSK_WAV_FORMAT_ALAW       = 0x0006,
    NSK_WAV_FORMAT_MULAW      = 0x0007,
    NSK_WAV_FORMAT_EXTENSIBLE = 0xFFFE
};

/*!
 * \brief  WAV data
 */
struct nsk_wav {

    /*! Riff format data */
    struct {
        size_t size; /*!< Size of the Riff data */
    } riff;

    struct {
        size_t size; /*!< Size of the Chunks section */
    } chunks;

    /*! Value samples */
    struct {
        /*! Raw value samples (normalized to -1.0..+1.0)
         *  Note: IEEE_FLOAT samples aren't normalized */
        struct {
            size_t  count; /*!< Number of samples */
            double *value; /*!< List of samples   */
            double  max;   /*!< Maximum value     */
            double  min;   /*!< Minimum value     */
            double  center;/*!< Center value      */
        } raw;
    } samples;

    struct {
        enum nsk_wav_audioformat    audioformat;
        uint16_t                    channels;
        uint32_t                    samplerate;
        uint32_t                    byterate;
        uint16_t                    blockalign;
        uint16_t                    bitspersample;
    } format;

    struct {
        char *title;
        char *artist;
        char *comment;
        char *software;
    } meta;
};

/*!
 * \brief  Converts the enum value of the format to constant string
 *
 * \param[in] format  The format
 * \return  Constant string
 */
const char *nsk_wav_aftostring(enum nsk_wav_audioformat format);

/*!
 * \brief  Frees the WAV data
 *
 * \param[in,out] wav  The WAV data
 */
void nsk_wav_free(struct nsk_wav *wav);
