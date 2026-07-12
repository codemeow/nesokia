#include <nsk_util_meta.h>

#include "nsk_wav_process.h"
#include "nsk_wav_utils.h"
#include "nsk_wav_read.h"

#include "process/nsk_wav_center.h"
#include "process/nsk_wav_boundaries.h"
#include "process/nsk_wav_decoder.h"
#include "process/nsk_wav_output.h"
#include "process/nsk_wav_quantization.h"
#include "process/nsk_wav_select.h"

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
            const struct nsk_wav *wav,
            struct nsk_wav_ctx  *ctx
        );
    } _table[] = {
        {
            .name = "Center and peak-normalize",
            .func = nsk_wav_center
        },
        {
            .name = "Find boundaries",
            .func = nsk_wav_boundaries
        },
        {
            .name = "Grid quantization",
            .func = nsk_wav_quantization
        },
        {
            .name = "Marks selection",
            .func = nsk_wav_select
        },
        {
            .name = "Decode notes",
            .func = nsk_wav_decoder
        },
        {
            .name = "Output notes",
            .func = nsk_wav_output
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

    struct nsk_wav_ctx ctx = {0};

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        nsk_inf("# %s\n", _table[i].name);
        const double start = nsk_time_stamp();
        if (!_table[i].func(wav, &ctx)) {
            nsk_wav_ctx_free(&ctx);
            return false;
        }
        const double end = nsk_time_stamp();
        nsk_time_log(start, end, 4);
    }

    nsk_wav_ctx_free(&ctx);
    return true;
}
