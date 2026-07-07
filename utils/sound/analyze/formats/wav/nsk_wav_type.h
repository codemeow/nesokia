#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "nsk_wav_candidate.h"

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
 * \brief  WAV energy span
 */
struct nsk_wav_span {
    /*! Start of the span in sample index */
    size_t start;
    /*! End of the span in sample index */
    size_t end;
};

/*!
 * \brief  Detected edges
 */
struct nsk_wav_edges {
    /*! Number of edges */
    size_t count;
    /*! Array of edged */
    double *edge;
};

/*!
 * \brief  Quantized time marks
 */
struct nsk_wav_mark {
    size_t frame;                                 /*!< Frame index          */
    size_t count;                                 /*!< Number of candidates */
    const struct nsk_wav_candidate **candidates;  /*!< Candidates list      */

    /*!
     * Is this time mark accepted as the note change?
     */
    bool accepted;

    /*!
     * Maximum strength of all the candidates
     */
    double maxstrength;

    /*!
     * Maximum absolute deltast value of the candidates
     */
    double maxabsdeltast;

    /*!
     * Number of candidates of each kind contained in #candidates
     */
    size_t types[NSK_WAV_CND_METHODS_COUNT];

    /*!
     * Is this mark is located in the dense accepted area?
     */
    bool indense;
};

struct nsk_wav_segment {
    size_t framestart;
    size_t frameend;

    size_t samplestart;
    size_t sampleend;

    bool active; /*!< Rough check - active or rest */

    double p2p; /*!< Peak-to-peak amplitude */
    double volume; /*!< Relative volume level */

    int midi;
    double frequency;
    double confidence;
    double harmonicratio;
    double duty;
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

    /*! Lists of detected edges */
    struct {
        struct nsk_wav_edges rise; /*!< Rising edges  */
        struct nsk_wav_edges fall; /*!< Falling edges */
    } edges;

    /*! Quantized time marks */
    struct {
        size_t count;              /*!< Number of marks */
        struct nsk_wav_mark *list; /*!< Marks */
    } marks;

    struct {
        size_t count;
        struct nsk_wav_segment *list;
        double silence; /*!< Global silence level */
    } segments;
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

/*!
 * \brief  Appends new detected egde
 *
 * \param[in,out]  edge       The edge
 * \param[in]      timestamp  The timestamp
 * \return True if successfully appended
 */
__attribute__((warn_unused_result))
bool nsk_wav_edge(
    struct nsk_wav_edges *edge,
    double                timestamp
);
