#pragma once

/*!
 * \brief  Converts a MIDI note number to frequency
 *
 * \param[in] midi  MIDI note number
 * \return    Frequency in Hz
 */
double nsk_midi_tofreq(int midi);

/*!
 * \brief  Converts frequency to the nearest MIDI note number
 *
 * \param[in] freq  Frequency in Hz
 * \return    Nearest MIDI note number
 */
int nsk_midi_fromfreq(double freq);
