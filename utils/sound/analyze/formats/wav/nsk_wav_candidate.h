#pragma once

#include <stdbool.h>

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
 * Physicall of musical candidate meaning
 */
enum nsk_wav_cnd_kind {
    /*! Energy rising at this point */
    NSK_WAV_CND_KIND_ENERGY_ONSET,
    /*! Energy fall at this point   */
    NSK_WAV_CND_KIND_ENERGY_OFFSET,
    /*! Edge rise */
    NSK_WAV_CND_KIND_EDGE_RISE,
    /*! Edge fall */
    NSK_WAV_CND_KIND_EDGE_FALL,
    /*! Edge detection */
    NSK_WAV_CND_KIND_EDGE,
    /*! Transition probe */
    NSK_WAV_CND_KIND_TRANSITION_PROBE,
    /*! Legato candidates */
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

    /*!
     * Pitch distance, in semitones, between the stable same-polarity periods
     * measured before and after this candidate.  Positive values mean the right
     * side is higher in pitch; negative values mean the right side is lower.
     *
     * Valid only for period-transition candidates. Optional field.
     */
    double deltast;
};
