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
 * \brief  Candidate detection methods
 */
enum nsk_wav_cnd_method {
    /*! Detected by energy change, onset  */
    NSK_WAV_CND_METHOD_ENERGY_ONSET,
    /*! Detected by energy change, offset */
    NSK_WAV_CND_METHOD_ENERGY_OFFSET,
};

/*!
 * \brief  Candidate kind
 *
 * Physicall of musical candidate meaning
 */
enum nsk_wav_cnd_kind {
    /*! Energy rising at this point */
    NSK_WAV_CND_KIND_ENERGY_ONSET,
    /*! Energy fall at this point   */
    NSK_WAV_CND_KIND_ENERGY_OFFSET
};

/*!
 * \brief  WAV candidate
 */
struct nsk_wav_candidate {
    /*! Method of detection   */
    enum nsk_wav_cnd_method method;

    /*! Kind of the detection */
    enum nsk_wav_cnd_kind   kind;

    /*! Candidate timestamp */
    double timestamp;

    /*! Detection strength */
    double strength;

    /*! Detection confidence */
    double confidence;
};

/*!
 * \brief  WAV energy span
 */
struct nsk_wav_span {
    /*! Start of the span in sample index */
    size_t start;
    /*! End of the span in sample index */
    size_t end;
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

    /*! List of event candidates */
    struct {
        /*! Number of candidates */
        size_t                    count;
        /*! Array of candidates  */
        struct nsk_wav_candidate *candidate;
    } candidates;

    /*! List of detected energy spans */
    struct {
        /*! Number of spans */
        size_t count;
        /*! Array of detected spans */
        struct nsk_wav_span *span;
    } spans;

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

/*!
 * \brief  Appends new candidate
 *
 * \param[in,out] wav        The wav
 * \param[in]     candidate  The candidate data
 * \return True if successfully appended
 */
__attribute__((warn_unused_result))
bool nsk_wav_candidate(
    struct nsk_wav          *wav,
    struct nsk_wav_candidate candidate
);

/*!
 * \brief  Appends new detected energy span
 *
 * \param[in,out] wav   The wav
 * \param[in]     span  The span
 * \return  True if successfully appended
 */
__attribute__((warn_unused_result))
bool nsk_wav_span(
    struct nsk_wav     *wav,
    struct nsk_wav_span span
);
