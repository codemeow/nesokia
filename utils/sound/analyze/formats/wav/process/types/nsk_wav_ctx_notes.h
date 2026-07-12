#pragma once

#include <stdbool.h>
#include <stddef.h>

/*!
 * \brief  Final decoded note or rest event
 */
struct nsk_wav_note {
    bool active; /*!< Whether the event is a note */

    size_t framestart; /*!< First frame in the event */
    size_t frameend;   /*!< One-past-last frame in the event */

    size_t samplestart; /*!< First sample in the event */
    size_t sampleend;   /*!< One-past-last sample in the event */

    int midi;         /*!< MIDI note number, or -1 for rest */
    double frequency; /*!< Fundamental frequency, or 0 for rest */
    double volume;    /*!< Relative volume level, or 0 for rest */
    double duty;      /*!< Pulse duty cycle in percent, or 0 for rest */
};

struct nsk_wav_ctx;

/*!
 * \brief  Final decoded event storage owned by the processing context
 */
struct nsk_wav_ctx_notes {
    size_t count;              /*!< Number of decoded events */
    struct nsk_wav_note *list; /*!< Decoded event list */
};

/*!
 * \brief  Clears decoded event storage in the processing context
 *
 * \param[in,out] ctx  Processing context
 */
void nsk_wav_ctx_notes_clear(
    struct nsk_wav_ctx *ctx
);

/*!
 * \brief  Appends a decoded event to the processing context
 *
 * \param[in,out] ctx   Processing context
 * \param[in]     note  Decoded event to append
 * \return True if the event was appended successfully
 */
__attribute__((warn_unused_result))
bool nsk_wav_ctx_note(
    struct nsk_wav_ctx *ctx,
    struct nsk_wav_note note
);
