#include <nsk_util_meta.h>

#include "nsk_wav_select.h"
#include "../nsk_wav_utils.h"
#include "../../../arguments/nsk_args_options.h"
#include "select/nsk_wav_sl_bydenseperiod.h"
#include "select/nsk_wav_sl_byenergy.h"
#include "select/nsk_wav_sl_byperiodtransition.h"
#include "select/nsk_wav_sl_byprobes.h"
#include "select/nsk_wav_sl_bysinglemixedperiod.h"
#include "select/nsk_wav_sl_bystrength.h"
#include "select/nsk_wav_sl_bytrusted.h"

/*!
 * \brief  Mark selector table entry.
 */
struct _selector_step {
    const char *name;   /*!< Human-readable selector name */
    double elapsed;     /*!< Accumulated selector runtime in milliseconds */
    bool (*selector)(   /*!< Selector callback */
        const struct nsk_wav_mark *mark
    );
};

/*!
 * \brief  Runs one selector and accumulates its runtime.
 *
 * \param[in,out] step  Selector entry
 * \param[in]     mark  Mark to test
 * \return Selector decision
 */
static bool _selector_step_accept(
    struct _selector_step *step,
    const struct nsk_wav_mark *mark
) {
    const double start = nsk_time_stamp();
    const bool accepted = step->selector(mark);
    const double end = nsk_time_stamp();

    step->elapsed += end - start;
    return accepted;
}

/*!
 * \brief  Logs accumulated selector timings.
 *
 * \param[in] steps  Selector table
 * \param[in] count  Number of selector entries
 */
static void _selector_steps_log(
    const struct _selector_step *steps,
    size_t count
) {
    for (size_t i = 0; i < count; i++) {
        nsk_inf("    ## %s\n", steps[i].name);
        nsk_time_log(0.0, steps[i].elapsed, 8);
    }
}

/*!
 * \brief  Checks whether a mark is inside a locally dense accepted region.
 *
 * \param[in] ctx    Processing context
 * \param[in] index  Mark index to classify
 * \return True if the mark is in a locally dense region
 */
static bool _mark_isdense(
    const struct nsk_wav_ctx *ctx,
    size_t index
) {
    const size_t radius = nsk_options_program.profile.select.denseradius;

    if (index == 0 || ctx->marks.count <= 1) {
        return false;
    }

    const size_t imin = index > radius ? index - radius : 1;
    const size_t imax = NSK_MIN(
        ctx->marks.count - 1,
        index + radius
    );
    const size_t icount = imax - imin + 1;

    size_t accepted = 0;
    for (size_t i = imin; i <= imax; i++) {
        if (ctx->marks.list[i].accepted) {
            accepted++;
        }
    }

    return
        (double)accepted / icount >=
            nsk_options_program.profile.select.denseratiothreshold &&
        accepted >= nsk_options_program.profile.select.densecountthreshold;
}

/*!
 * \brief  Precomputes dense-region flags for all marks.
 *
 * \param[in,out] ctx  Processing context
 */
static void _marks_calcdense(
    struct nsk_wav_ctx *ctx
)  {
    for (size_t i = 0; i < ctx->marks.count; i++) {
        ctx->marks.list[i].indense = _mark_isdense(ctx, i);
    }
}

/*!
 * \brief  Accepts the time marks that pass the selection
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context containing quantized marks
 * \return True if selection completed successfully
 */
bool nsk_wav_select(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    (void)wav;

    struct _selector_step selectors_primary[] = {
        {
            .name = "By energy",
            .selector = nsk_wav_sl_byenergy
        },
        {
            .name = "By strength",
            .selector = nsk_wav_sl_bystrength
        },
        {
            .name = "By trusted",
            .selector = nsk_wav_sl_bytrusted
        },
        {
            .name = "By period transition",
            .selector = nsk_wav_sl_byperiodtransition
        },
        {
            .name = "By single mixed",
            .selector = nsk_wav_sl_bysinglemixedperiod
        }
    };

    struct _selector_step selectors_secondary[] = {
        {
            .name = "By probes",
            .selector = nsk_wav_sl_byprobes
        },
        {
            .name = "By dense period",
            .selector = nsk_wav_sl_bydenseperiod
        }
    };

    for (size_t marki = 0; marki < ctx->marks.count; marki++) {
        struct nsk_wav_mark *mark = &ctx->marks.list[marki];

        if (mark->count == 0) {
            continue;
        }

        for (size_t i = 0; i < NSK_SIZE(selectors_primary); i++) {
            if (_selector_step_accept(&selectors_primary[i], mark)) {
                mark->accepted = true;
                break;
            }
        }
    }

    _selector_steps_log(selectors_primary, NSK_SIZE(selectors_primary));

    _marks_calcdense(ctx);

    for (size_t marki = 0; marki < ctx->marks.count; marki++) {
        struct nsk_wav_mark *mark = &ctx->marks.list[marki];

        if (
            mark->count == 0 ||
            mark->accepted
        ) {
            continue;
        }

        for (size_t i = 0; i < NSK_SIZE(selectors_secondary); i++) {
            if (_selector_step_accept(&selectors_secondary[i], mark)) {
                mark->accepted = true;
                break;
            }
        }
    }

    _selector_steps_log(selectors_secondary, NSK_SIZE(selectors_secondary));

    return true;
}
