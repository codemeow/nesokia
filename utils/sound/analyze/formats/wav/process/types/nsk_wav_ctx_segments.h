#pragma once

#include <stdbool.h>
#include <stddef.h>

/*!
 * \brief  Classified frame interval
 */
struct nsk_wav_segment {
    size_t framestart; /*!< First frame in the interval */
    size_t frameend;   /*!< Last frame in the interval */

    size_t samplestart; /*!< First sample in the interval */
    size_t sampleend;   /*!< One-past-last sample in the interval */

    bool active; /*!< Whether the interval contains a note */

    double p2p;    /*!< Peak-to-peak amplitude */
    double volume; /*!< Relative volume level */

    int midi;             /*!< MIDI note number, or -1 for rest */
    double frequency;     /*!< Fundamental frequency */
    double confidence;    /*!< Pitch classification confidence */
    double harmonicratio; /*!< Harmonic energy ratio */
    double duty;          /*!< Pulse duty cycle in percent */
};

/*!
 * \brief  Segment storage owned by the processing context
 */
struct nsk_wav_ctx_segments {
    size_t count;                 /*!< Number of segments */
    struct nsk_wav_segment *list; /*!< Segment list */
    double silence;               /*!< Global silence level */
};
