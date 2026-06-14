#pragma once

#include "nsk_wav_type.h"

/*!
 * Automatically free the allocated WAV data on block exit
 */
#define nsk_auto_wav __attribute__((cleanup(_nsk_auto_wav)))

/*!
 * Automatically free the allocated WAV candidates on block exit
 */
#define nsk_auto_cnds __attribute__((cleanup(_nsk_auto_cnds)))

/*!
 * \brief  Autocleaner for allocated WAV data
 *
 * \param[in, out]  wav  The WAV data
 */
void _nsk_auto_wav(struct nsk_wav **wav);

/*!
 * \brief  Autocleaner for allocated WAV candidates data
 *
 * \param[in, out]  cnds  The WAV candidates data
 */
void _nsk_auto_cnds(struct nsk_wav_cnds **cnds);