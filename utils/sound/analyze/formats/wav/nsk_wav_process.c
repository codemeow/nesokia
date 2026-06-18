#include <nsk_util_meta.h>

#include "nsk_wav_process.h"
#include "nsk_wav_utils.h"
#include "nsk_wav_read.h"

#include "process/nsk_wav_center.h"
#include "process/nsk_wav_boundaries.h"

/*!
 * \brief  Prints the WAV info
 *
 * \param[in] wav  The wav
 */
static void _wav_print(const struct nsk_wav *wav) {
    static const size_t _samples_theo_limit = 10;
    const size_t _samples_prac_edge = NSK_MIN(
        wav->samples.raw.count,
        _samples_theo_limit
    );

    nsk_inf("WAV\n");
    nsk_inf("    Riff\n");
    nsk_inf("        Size           : %zu\n", wav->riff.size);
    nsk_inf("    Chunks\n");
    nsk_inf("        Size           : %zu\n", wav->chunks.size);
    nsk_inf("    Format\n");
    nsk_inf("        AF             : %s\n", nsk_wav_aftostring(wav->format.audioformat));
    nsk_inf("        Channels       : %u\n", wav->format.channels);
    nsk_inf("        Sample rate    : %u\n", wav->format.samplerate);
    nsk_inf("        Byte rate      : %u\n", wav->format.byterate);
    nsk_inf("        Block align    : %u\n", wav->format.blockalign);
    nsk_inf("        Bits per sample: %u\n", wav->format.bitspersample);
    if (wav->meta.artist || wav->meta.comment || wav->meta.software || wav->meta.title) {
        nsk_inf("    Meta\n");
    }
    if (wav->meta.comment) {
        nsk_inf("        Comment        : %s\n", wav->meta.comment);
    }
    if (wav->meta.software) {
        nsk_inf("        Software       : %s\n", wav->meta.software);
    }
    if (wav->meta.artist) {
        nsk_inf("        Artist         : %s\n", wav->meta.artist);
    }
    if (wav->meta.title) {
        nsk_inf("        Title          : %s\n", wav->meta.title);
    }
    nsk_inf("    Samples\n");
    nsk_inf("        Count          : %zu\n", wav->samples.raw.count);
    nsk_inf("        Max value      : %-15.14f\n", wav->samples.raw.max);
    nsk_inf("        Min value      : %-15.14f\n", wav->samples.raw.min);
    nsk_inf("        Values (0..%zu)\n", _samples_prac_edge);
    for (size_t i = 0; i < _samples_prac_edge; i++) {
        nsk_inf(
            "            Value[%zu]       : %+-15.14f\n",
            i,
            wav->samples.raw.value[i]
        );
    }
}

/*!
 * \brief  Converts the WAV file to music sheets
 *
 * \param[in] filename  WAV file filename
 * \return    True if converted successfully
 */
bool nsk_wav_process(const char *filename) {
    static const struct {
        const char *name;
        bool (*func) (
            struct nsk_wav      *wav
        );
    } _table[] = {
        {
            .name = "Center and peak-normalize",
            .func = nsk_wav_center
        },
        {
            .name = "Find boundaries",
            .func = nsk_wav_boundaries
        }
    };

    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err("Cannot open file: \"%s\"\n", filename);
        return false;
    }

    nsk_auto_wav struct nsk_wav *wav = nsk_wav_read(file);
    if (!wav) {
        nsk_err("Cannot parse file as WAV: \"%s\"\n", filename);
        return false;
    }

    _wav_print(wav);

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        nsk_inf("Step: %s\n", _table[i].name);
        if (!_table[i].func(wav)) {
            return false;
        }
    }

    return true;
}