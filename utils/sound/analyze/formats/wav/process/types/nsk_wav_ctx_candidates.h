#pragma once

#include <stdbool.h>
#include <stddef.h>

/*!
 * \brief  Candidate detection methods
 */
enum nsk_wav_cnd_method {
    /*! Detected by energy change, onset  */
    NSK_WAV_CND_METHOD_ENERGY_ONSET,
    /*! Detected by energy change, offset */
    NSK_WAV_CND_METHOD_ENERGY_OFFSET,
    /*! Detected by edge probing */
    NSK_WAV_CND_METHOD_EDGE_PROBE,
    /*! Detected by edge change */
    NSK_WAV_CND_METHOD_EDGE_CHANGE,
    /*! Transitional probes in suspicious interval */
    NSK_WAV_CND_METHOD_TRANSITION_PROBES,
    /*! Mixed period solve candidates */
    NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE,
    /*! Short period edge candidates */
    NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE,
    /*! Span start period probe */
    NSK_WAV_CND_METHOD_SPAN_START_PERIOD_PROBE,
    /*! Span end period probe */
    NSK_WAV_CND_METHOD_SPAN_END_PERIOD_PROBE,
    /*! Energy rise on grid */
    NSK_WAV_CND_METHOD_GRID_ENERGY_RISE,

    /*! Automatic counter */
    NSK_WAV_CND_METHODS_COUNT
};

/*!
 * \brief  Candidate kind
 *
 * Physical or musical candidate meaning.
 */
enum nsk_wav_cnd_kind {
    /*! Energy rising at this point */
    NSK_WAV_CND_KIND_ENERGY_ONSET,
    /*! Energy fall at this point */
    NSK_WAV_CND_KIND_ENERGY_OFFSET,
    /*! Edge rise */
    NSK_WAV_CND_KIND_EDGE_RISE,
    /*! Edge fall */
    NSK_WAV_CND_KIND_EDGE_FALL,
    /*! Edge detection */
    NSK_WAV_CND_KIND_EDGE,
    /*! Transition probe */
    NSK_WAV_CND_KIND_TRANSITION_PROBE,
    /*! Legato candidate */
    NSK_WAV_CND_KIND_LEGATO,
    /*! Waveform reset detection */
    NSK_WAV_CND_KIND_WAVEFORM_RESET,
    /*! Span start probe */
    NSK_WAV_CND_KIND_SPAN_START_PROBE,
    /*! Span end probe */
    NSK_WAV_CND_KIND_SPAN_END_PROBE,
    /*! Energy rise on grid */
    NSK_WAV_CND_KIND_GRID_ENERGY_RISE
};

/*!
 * \brief  Boundary candidate
 */
struct nsk_wav_candidate {
    enum nsk_wav_cnd_method method; /*!< Detection method */
    enum nsk_wav_cnd_kind   kind;   /*!< Candidate kind */

    double timestamp;  /*!< Candidate timestamp in seconds */
    double strength;   /*!< Detection strength */
    double confidence; /*!< Detection confidence */

    /*!
     * Pitch distance, in semitones, between stable same-polarity periods
     * measured before and after this candidate. Positive values mean the right
     * side is higher in pitch; negative values mean the right side is lower.
     */
    double deltast;
};

struct nsk_wav_ctx;

/*!
 * \brief  Boundary candidate storage owned by the processing context
 */
struct nsk_wav_ctx_candidates {
    size_t count;                         /*!< Number of candidates */
    struct nsk_wav_candidate *candidate;  /*!< Array of candidates */
};

/*!
 * \brief  Appends a boundary candidate to the processing context
 *
 * \param[in,out] ctx        Processing context
 * \param[in]     candidate  Candidate to append
 * \return True if the candidate was appended successfully
 */
__attribute__((warn_unused_result))
bool nsk_wav_ctx_candidate(
    struct nsk_wav_ctx      *ctx,
    struct nsk_wav_candidate candidate
);
