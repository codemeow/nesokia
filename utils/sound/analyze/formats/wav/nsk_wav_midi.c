#include <math.h>
#include <stddef.h>

#include "nsk_wav_midi.h"

/*! Standard MIDI base note */
static const int    a4midi = 69;

/*! Base note's frequency */
static const double a4freq = 440.0;

/*!
 * Number of semitones per octave
 */
static const size_t semitones_peroctave = 12;

/*!
 * \brief  Converts a MIDI note number to frequency
 *
 * \param[in] midi  MIDI note number
 * \return    Frequency in Hz
 */
double nsk_midi_tofreq(int midi) {
    return
        a4freq *
        pow(
            2.0,
            (double)(midi - a4midi) / (double)semitones_peroctave
        );
}

/*!
 * \brief  Converts frequency to the nearest MIDI note number
 *
 * \param[in] freq  Frequency in Hz
 * \return    Nearest MIDI note number
 */
int nsk_midi_fromfreq(double freq) {
    return
        lround(
            a4midi +
            (double)semitones_peroctave *
            log2(freq / a4freq)
        );
}
