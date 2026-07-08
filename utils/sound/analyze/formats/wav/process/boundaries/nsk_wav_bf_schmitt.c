#include <math.h>

#include <nsk_util_meta.h>

#include "../../../../arguments/nsk_args_options.h"
#include "nsk_wav_bf_schmitt.h"

/*!
 * \brief  States, used in the trigger detection
 */
enum _schmitt_trigger_state {
    _SCHMITT_TRIG_LO, /*!< Low signal  */
    _SCHMITT_TRIG_HI  /*!< High signal */
};

/*!
 * \brief  Shared rolling-window context for one Schmitt span
 */
struct _schmitt_roll_common {
    const struct nsk_wav      *wav;      /*!< WAV being processed           */
    const struct nsk_wav_ctx  *ctx;      /*!< Processing context            */
    const struct nsk_wav_span *span;     /*!< Active span being scanned     */
    size_t                     count;    /*!< Number of real span samples   */
    size_t                     lpad;     /*!< Virtual samples before span   */
    size_t                     window;   /*!< Rolling window size in samples*/
    size_t                     capacity; /*!< Deque ring-buffer capacity    */
};

/*!
 * \brief  State for one rolling extremum, either minimum or maximum
 */
struct _schmitt_roll_extreme {
    size_t *deque; /*!< Ring-buffer storage for virtual padded-span indexes */
    size_t  head;  /*!< Logical deque head position                         */
    size_t  tail;  /*!< Logical deque tail position                         */
    double *value; /*!< Output rolling extremum array                       */
    bool (*keeptail)(double tailvalue, double value); /*!< Tail policy */
};

/*!
 * \brief  Allocates memory for the rolling min or max
 *
 * \param[in]  count    The number of elements
 * \param[in]  elemsize Element size
 * \param[out] array    The allocated array
 * \param[in]  name     The name of the array
 * \return    True if allocated */
static bool _bf_schmitt_rollalloc(
    size_t        count,
    size_t        elemsize,
    void        **array,
    const char   *name
) {
    *array = calloc(count, elemsize);
    if (!*array) {
        nsk_err(
            "Cannot allocate memory for span's rolling %s",
            name
        );
    }

    return *array != NULL;
}

/*!
 * \brief  Returns a sample from a virtually edge-padded span
 *
 * The Schmitt rolling min/max stage evaluates a centered window for every
 * sample in the span.  Near span boundaries, part of that centered window would
 * fall outside the real samples.  Those out-of-range positions are handled with
 * edge padding: positions before the span reuse the first real sample, and
 * positions after the span reuse the last real sample.  This helper exposes
 * that virtual padded array without allocating it.
 *
 * The virtual layout is:
 *
 * \code
 * [first sample x lpad times][span samples][last sample x rpad times]
 * \endcode
 *
 * The \p index parameter is an index in that virtual padded layout, not in the
 * original WAV sample array.  The returned value always comes from
 * ctx->samples.value and is clamped to the nearest real span endpoint when
 * \p index points into the virtual padding.
 *
 * \param[in] ctx    Processing context containing the working samples
 * \param[in] span   Active span whose samples are being virtually padded
 * \param[in] count  Number of real samples in the span
 * \param[in] lpad   Number of virtual samples before the span
 * \param[in] index  Index in the virtual padded span
 * \return Sample value at \p index after edge-padding rules are applied
 */
static double _bf_schmitt_span_padded_value(
    const struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_span *span,
    size_t count,
    size_t lpad,
    size_t index
) {
    if (index < lpad) {
        return ctx->samples.value[span->start];
    }

    const size_t local = index - lpad;
    if (local >= count) {
        return ctx->samples.value[span->end - 1];
    }

    return ctx->samples.value[span->start + local];
}

/*!
 * \brief  Reads a value from a fixed-capacity logical deque
 *
 * The rolling min/max implementation stores deque entries in a ring buffer of
 * \p capacity elements while its logical head and tail counters grow
 * monotonically.  This helper maps a logical deque position to the physical
 * ring-buffer slot with modulo arithmetic.
 *
 * The deque values are padded-span indexes, not sample values.  Keeping indexes
 * makes it possible to discard entries that have moved out of the current
 * rolling window.
 *
 * \param[in] deque     Ring-buffer storage for deque entries
 * \param[in] capacity  Number of physical slots in \p deque
 * \param[in] pos       Logical deque position to read
 * \return Value stored at logical position \p pos
 */
static size_t _bf_schmitt_deque_get(
    const size_t *deque,
    size_t capacity,
    size_t pos
) {
    return deque[pos % capacity];
}

/*!
 * \brief  Writes a value to a fixed-capacity logical deque
 *
 * Stores \p value at logical deque position \p pos.  The physical storage is a
 * ring buffer, so \p pos is reduced modulo \p capacity before writing.  The
 * caller is responsible for maintaining the monotonic-deque invariants and for
 * ensuring that the number of live entries never exceeds \p capacity.
 *
 * In this detector the deque capacity is the rolling window size, which is
 * enough because entries older than one window are removed before new entries
 * are appended.
 *
 * \param[in,out] deque     Ring-buffer storage for deque entries
 * \param[in]     capacity  Number of physical slots in \p deque
 * \param[in]     pos       Logical deque position to write
 * \param[in]     value     Padded-span index to store
 */
static void _bf_schmitt_deque_set(
    size_t *deque,
    size_t capacity,
    size_t pos,
    size_t value
) {
    deque[pos % capacity] = value;
}

/*!
 * \brief  Checks whether the front deque entry left the rolling window
 *
 * The monotonic deque stores indexes in the virtual edge-padded span.  As the
 * rolling window moves to the right, the oldest deque entry may no longer
 * belong to the current window.  Such an entry must be removed before the deque
 * front is used as the current rolling minimum or maximum.
 *
 * The front entry is expired when:
 *
 * \code
 * front_index + window <= index
 * \endcode
 *
 * where \p index is the current virtual padded-span index.  In other words, the
 * front index is strictly to the left of the current rolling window.
 *
 * \param[in] deque     Ring-buffer storage for deque entries
 * \param[in] capacity  Number of physical slots in \p deque
 * \param[in] window    Rolling window size in samples
 * \param[in] head      Logical deque head position
 * \param[in] tail      Logical deque tail position
 * \param[in] index     Current virtual padded-span index
 * \return True if the front entry exists and is outside the current window
 */
static bool _bf_schmitt_deque_front_expired(
    const size_t *deque,
    size_t capacity,
    size_t window,
    size_t head,
    size_t tail,
    size_t index
) {
    return
        head < tail &&
        _bf_schmitt_deque_get(
            deque,
            capacity,
            head
        ) + window <= index;
}

/*!
 * \brief  Checks whether a max-deque tail should remain in the deque
 *
 * For a rolling maximum, tail entries less than or equal to the new value can
 * never become the maximum while the new value is still inside the window.
 *
 * \param[in] tailvalue  Value stored at the current deque tail
 * \param[in] value      New value being inserted
 * \return True if the tail entry should be kept
 */
static bool _bf_schmitt_keep_max_tail(
    double tailvalue,
    double value
) {
    return tailvalue > value;
}

/*!
 * \brief  Checks whether a min-deque tail should remain in the deque
 *
 * For a rolling minimum, tail entries greater than or equal to the new value can
 * never become the minimum while the new value is still inside the window.
 *
 * \param[in] tailvalue  Value stored at the current deque tail
 * \param[in] value      New value being inserted
 * \return True if the tail entry should be kept
 */
static bool _bf_schmitt_keep_min_tail(
    double tailvalue,
    double value
) {
    return tailvalue < value;
}

/*!
 * \brief  Removes the expired front entry from one deque
 *
 * At most one front entry can expire on each iteration because the rolling
 * window advances by one virtual sample.  If the current front index is outside
 * the current window, the logical head is advanced past it.
 *
 * \param[in]     common   Shared rolling-window context
 * \param[in,out] extreme  Deque state for one rolling extremum
 * \param[in]     index    Current virtual padded-span index
 */
static void _bf_schmitt_deque_drop_expired_front(
    const struct _schmitt_roll_common  *common,
    struct _schmitt_roll_extreme       *extreme,
    size_t index
) {
    if (_bf_schmitt_deque_front_expired(
        extreme->deque,
        common->capacity,
        common->window,
        extreme->head,
        extreme->tail,
        index
    )) {
        extreme->head++;
    }
}

/*!
 * \brief  Removes tail entries dominated by the current value
 *
 * Maintains the monotonic ordering of one deque before the current index is
 * appended.  The supplied \p keep_tail predicate defines whether this is a
 * maximum deque or a minimum deque.
 *
 * \param[in]     common   Shared rolling-window context
 * \param[in,out] extreme  Deque state for one rolling extremum
 * \param[in]     value    Sample value at the current padded index
 */
static void _bf_schmitt_deque_prune_tail(
    const struct _schmitt_roll_common  *common,
    struct _schmitt_roll_extreme       *extreme,
    double value
) {
    while (extreme->head < extreme->tail) {
        const size_t tailindex = _bf_schmitt_deque_get(
            extreme->deque,
            common->capacity,
            extreme->tail - 1
        );
        const double tailvalue = _bf_schmitt_span_padded_value(
            common->ctx,
            common->span,
            common->count,
            common->lpad,
            tailindex
        );

        if (extreme->keeptail(tailvalue, value)) {
            break;
        }

        extreme->tail--;
    }
}

/*!
 * \brief  Updates one monotonic deque with the current padded-span index
 *
 * This helper performs the per-index deque maintenance shared by rolling
 * minimum and rolling maximum calculation.  It removes an expired front entry,
 * discards tail entries dominated by the current value, and appends the current
 * virtual padded-span index.
 *
 * The \c keeptail callback is the only policy difference between min and max
 * deques.  For max it keeps strictly larger tail values; for min it keeps
 * strictly smaller tail values.
 *
 * \param[in]     common   Shared rolling-window context
 * \param[in,out] extreme  Deque state for one rolling extremum
 * \param[in]     index    Current virtual padded-span index
 * \param[in]     value    Sample value at \p index
 */
static void _bf_schmitt_deque_process(
    const struct _schmitt_roll_common *common,
    struct _schmitt_roll_extreme      *extreme,
    size_t index,
    double value
) {
    _bf_schmitt_deque_drop_expired_front(
        common,
        extreme,
        index
    );
    _bf_schmitt_deque_prune_tail(
        common,
        extreme,
        value
    );

    _bf_schmitt_deque_set(
        extreme->deque,
        common->capacity,
        extreme->tail++,
        index
    );
}

/*!
 * \brief  Writes one rolling min/max value when a full window is available
 *
 * The first rolling result can be emitted only after \p window virtual samples
 * have been scanned.  Before that point this helper returns without touching
 * \p target.  Once a full window exists, the deque front identifies the padded
 * index that holds the current rolling extremum, and that sample value is
 * written to the corresponding output position.
 *
 * \param[in]     common   Shared rolling-window context
 * \param[in]     extreme  Deque state for one rolling extremum
 * \param[in]     index    Current virtual padded-span index
 */
static void _bf_schmitt_deque_write_value(
    const struct _schmitt_roll_common  *common,
    const struct _schmitt_roll_extreme *extreme,
    size_t index
) {
    if (index + 1 < common->window) {
        return;
    }

    const size_t out = index + 1 - common->window;
    const size_t paddedindex = _bf_schmitt_deque_get(
        extreme->deque,
        common->capacity,
        extreme->head
    );

    extreme->value[out] = _bf_schmitt_span_padded_value(
        common->ctx,
        common->span,
        common->count,
        common->lpad,
        paddedindex
    );
}

/*!
 * \brief  Calculates the rolling min and max of the span
 *
 * \param[in]  wav      Source WAV data
 * \param[in]  ctx      Processing context containing the working samples
 * \param[in]  span     The span
 * \param[in]  count    The span size
 * \param[in]  window   Rolling window size
 * \param[out] min      Rolling min array
 * \param[out] max      Rolling max array
 *
 * \return True bf successfully allocated and filled
 */
static bool _bf_schmitt_rollminmax(
    const struct nsk_wav      *wav,
    const struct nsk_wav_ctx  *ctx,
    const struct nsk_wav_span *span,
    size_t count,
    size_t window,
    double **min,
    double **max
) {
    const size_t capacity   = window;
    const size_t lpad       = window / 2;
    const size_t rpad       = window - 1 - lpad;
    const size_t virtcount  = lpad + count + rpad;

    nsk_auto_free size_t *maxdq = NULL;
    nsk_auto_free size_t *mindq = NULL;

    const struct {
        size_t        count;
        size_t        size;
        void        **ptr;
        const char   *name;
    } _table[] = {
        {
            .count = count,
            .size  = sizeof(**min),
            .ptr   = (void **)min,
            .name  = "min"
        },{
            .count = count,
            .size  = sizeof(**max),
            .ptr   = (void **)max,
            .name  = "max"
        },{
            .count = capacity,
            .size  = sizeof(*mindq),
            .ptr   = (void **)&mindq,
            .name  = "min dequeue"
        },{
            .count = capacity,
            .size  = sizeof(*maxdq),
            .ptr   = (void **)&maxdq,
            .name  = "max dequeue"
        },
    };

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        bool res = _bf_schmitt_rollalloc(
            _table[i].count,
            _table[i].size,
            _table[i].ptr,
            _table[i].name
        );
        if (!res) {
            return false;
        }
    }

    const struct _schmitt_roll_common common = {
        .wav      = wav,
        .ctx      = ctx,
        .span     = span,
        .count    = count,
        .lpad     = lpad,
        .window   = window,
        .capacity = capacity
    };
    struct _schmitt_roll_extreme maxctx = {
        .deque    = maxdq,
        .head     = 0,
        .tail     = 0,
        .value    = *max,
        .keeptail = _bf_schmitt_keep_max_tail
    };
    struct _schmitt_roll_extreme minctx = {
        .deque    = mindq,
        .head     = 0,
        .tail     = 0,
        .value    = *min,
        .keeptail = _bf_schmitt_keep_min_tail
    };

    for (size_t i = 0; i < virtcount; i++) {
        const double value = _bf_schmitt_span_padded_value(
            ctx,
            span,
            count,
            lpad,
            i
        );

        _bf_schmitt_deque_process(
            &common,
            &maxctx,
            i,
            value
        );
        _bf_schmitt_deque_process(
            &common,
            &minctx,
            i,
            value
        );

        _bf_schmitt_deque_write_value(
            &common,
            &maxctx,
            i
        );
        _bf_schmitt_deque_write_value(
            &common,
            &minctx,
            i
        );
    }

    return true;
}

/*!
 * \brief  Interpolates the edge timestamp at the local midline crossing
 *
 * Schmitt thresholds decide whether a transition is reliable, but the edge
 * timestamp is placed at the estimated crossing of the local midline between
 * the previous and current samples.  The supplied \p index is the global sample
 * index of \p sample, so the returned timestamp is in whole-WAV coordinates.
 *
 * \param[in] wav      The WAV being processed
 * \param[in] sample   Current sample value
 * \param[in] prev     Previous sample value
 * \param[in] index    Global sample index of \p sample
 * \param[in] midline  Local waveform midline at \p index
 * \return Interpolated edge timestamp in seconds
 */
static double _bf_schmitt_timebymidline(
    const struct nsk_wav        *wav,
    double                       sample,
    double                       prev,
    size_t                       index,
    double                       midline
) {
    const double denominator = sample - prev;
    double fraction = 0.0;

    if (denominator != 0.0) {
        fraction = (midline - prev) / denominator;
    }

    fraction = NSK_MIN(
        NSK_MAX(
            fraction,
            0.0
        ),
        1.0
    );

    return ((double)index - 1.0 + fraction) / wav->format.samplerate;
}

/*!
 * \brief  Adds a rising-edge probe candidate
 *
 * The candidate timestamp is refined by midline interpolation.  The candidate
 * is intentionally weak: it records a physical waveform edge for diagnostics
 * and later evidence aggregation, but it should not decide a boundary by itself.
 *
 * \param[in]     wav      Source WAV data
 * \param[in,out] ctx      Processing context receiving edges and candidates
 * \param[in]     sample   Current sample value
 * \param[in]     prev     Previous sample value
 * \param[in]     index    Global sample index of \p sample
 * \param[in]     midline  Local waveform midline at \p index
 * \return True if the candidate was appended
 */
static bool _bf_schmitt_addrising(
    const struct nsk_wav        *wav,
    struct nsk_wav_ctx          *ctx,
    double                       sample,
    double                       prev,
    size_t                       index,
    double                       midline
) {
    const double timestamp = _bf_schmitt_timebymidline(
        wav,
        sample,
        prev,
        index,
        midline
    );

    if (!nsk_wav_ctx_edge(
        &ctx->edges.rise,
        timestamp
    )) {
        return false;
    }

    return nsk_wav_ctx_candidate(
        ctx,
        (struct nsk_wav_candidate) {
            .method     = NSK_WAV_CND_METHOD_EDGE_PROBE,
            .kind       = NSK_WAV_CND_KIND_EDGE_RISE,
            .timestamp  = timestamp,
            .strength   = nsk_options_program.profile.boundary.schmitt.strength,
            .confidence = nsk_options_program.profile.boundary.schmitt.confidence
        }
    );
}

/*!
 * \brief  Adds a falling-edge probe candidate
 *
 * This is the falling-polarity counterpart of
 * _bf_schmitt_addrising().  It uses the same weak edge-probe scoring but marks
 * the candidate as a falling edge.
 *
 * \param[in]     wav      Source WAV data
 * \param[in,out] ctx      Processing context receiving edges and candidates
 * \param[in]     sample   Current sample value
 * \param[in]     prev     Previous sample value
 * \param[in]     index    Global sample index of \p sample
 * \param[in]     midline  Local waveform midline at \p index
 * \return True if the candidate was appended
 */
static bool _bf_schmitt_addfalling(
    const struct nsk_wav        *wav,
    struct nsk_wav_ctx          *ctx,
    double                       sample,
    double                       prev,
    size_t                       index,
    double                       midline
) {
    const double timestamp = _bf_schmitt_timebymidline(
        wav,
        sample,
        prev,
        index,
        midline
    );

    if (!nsk_wav_ctx_edge(
        &ctx->edges.fall,
        timestamp
    )) {
        return false;
    }

    return nsk_wav_ctx_candidate(
        ctx,
        (struct nsk_wav_candidate) {
            .method     = NSK_WAV_CND_METHOD_EDGE_PROBE,
            .kind       = NSK_WAV_CND_KIND_EDGE_FALL,
            .timestamp  = timestamp,
            .strength   = nsk_options_program.profile.boundary.schmitt.strength,
            .confidence = nsk_options_program.profile.boundary.schmitt.confidence
        }
    );
}

/*!
 * \brief  Processes one sample through the Schmitt trigger state machine
 *
 * The rolling extrema at this sample define a local midline and amplitude.
 * Those values produce high and low thresholds around the midline.  A rising
 * edge is accepted only when the current state is low and the sample crosses the
 * high threshold; a falling edge is accepted only when the state is high and
 * the sample crosses the low threshold.
 *
 * \param[in]     wav      Source WAV data
 * \param[in,out] ctx      Processing context receiving edges and candidates
 * \param[in]     sample   Current sample value
 * \param[in]     prev     Previous sample value
 * \param[in]     rollmax  Rolling maximum at this sample
 * \param[in]     rollmin  Rolling minimum at this sample
 * \param[in,out] state    Current Schmitt state
 * \param[in]     index    Global sample index of \p sample
 * \return True if any generated candidate was appended successfully
 */
static bool _bf_schmitt_span_sample(
    const struct nsk_wav        *wav,
    struct nsk_wav_ctx          *ctx,
    double                       sample,
    double                       prev,
    double                       rollmax,
    double                       rollmin,
    enum _schmitt_trigger_state *state,
    size_t                       index
) {
    static const double amplitudemin = 1e-9;

    const double midline  = (rollmax + rollmin) / 2.0;
    const double amplitude = NSK_MAX(
        (rollmax - rollmin) / 2.0,
        amplitudemin
    );

    const double hi =
        midline +
        amplitude *
            nsk_options_program.profile.boundary.schmitt.hysteresis;
    const double lo =
        midline -
        amplitude *
            nsk_options_program.profile.boundary.schmitt.hysteresis;

    if (*state == _SCHMITT_TRIG_LO && sample > hi) {
        if (!_bf_schmitt_addrising(
            wav,
            ctx,
            sample,
            prev,
            index,
            midline
        )) {
            return false;
        }

        *state = _SCHMITT_TRIG_HI;

    } else if (*state == _SCHMITT_TRIG_HI && sample < lo) {
        if (!_bf_schmitt_addfalling(
            wav,
            ctx,
            sample,
            prev,
            index,
            midline
        )) {
            return false;
        }

        *state = _SCHMITT_TRIG_LO;
    }

    return true;
}

/*!
 * \brief  Runs the Schmitt state machine over one active span
 *
 * The initial state is derived from the first sample relative to its local
 * midline.  Processing then starts at the second sample because edge timing
 * uses the interval between previous and current samples.
 *
 * \param[in]     wav      Source WAV data
 * \param[in,out] ctx      Processing context receiving edges and candidates
 * \param[in]     span     Active span being scanned
 * \param[in]     count    Number of samples in \p span
 * \param[in]     rollmax  Rolling maximum array for \p span
 * \param[in]     rollmin  Rolling minimum array for \p span
 * \return True if the span was processed successfully
 */
static bool _bf_schmitt_span_process(
    const struct nsk_wav       *wav,
    struct nsk_wav_ctx         *ctx,
    const struct nsk_wav_span  *span,
    size_t                      count,
    const               double *rollmax,
    const               double *rollmin
) {
    const double mid0 = (rollmax[0] + rollmin[0]) / 2.0;

    enum _schmitt_trigger_state state =
        ctx->samples.value[span->start] > mid0 ?
        _SCHMITT_TRIG_HI :
        _SCHMITT_TRIG_LO;

    for (size_t i = 1; i < count; i++) {
        if (!_bf_schmitt_span_sample(
            wav,
            ctx,
            ctx->samples.value[span->start + i],
            ctx->samples.value[span->start + i - 1],
            rollmax[i],
            rollmin[i],
            &state,
            span->start + i
        )) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Processes single span by Schmitt trigger analysis
 *
 * A span is an active audio region detected by the RMS envelope stage.  Schmitt
 * processing is performed per span so silence does not generate false waveform
 * edges and so edge trains are not connected across rests.
 *
 * This function prepares the local waveform geometry for one span by computing
 * centered rolling minimum and maximum arrays.  Those arrays describe the local
 * lower and upper waveform plateaus.  The following Schmitt-trigger step uses
 * them to derive a local midline, local amplitude, and high/low hysteresis
 * thresholds for each sample.
 *
 * \param[in]       wav     Source WAV data
 * \param[in,out]   ctx     Processing context containing samples and receiving candidates
 * \param[in]       span    The span data
 * \param[in]       window  Window size in samples
 * \return    True if processed
 */
static bool _bf_schmitt_span(
    const struct nsk_wav       *wav,
    struct nsk_wav_ctx         *ctx,
    const struct nsk_wav_span  *span,
    size_t                      window
) {
    /* Minimal sane span size */
    static const size_t countmin = 3;

    const size_t count = span->end - span->start;
    if (count < countmin) {
        return true;
    }

    nsk_auto_free double *rollmax = NULL;
    nsk_auto_free double *rollmin = NULL;

    if (!_bf_schmitt_rollminmax(
        wav,
        ctx,
        span,
        count,
        window,
        &rollmin,
        &rollmax
    )) {
        return false;
    }

    if (!_bf_schmitt_span_process(
        wav,
        ctx,
        span,
        count,
        rollmax,
        rollmin
    )) {
        return false;
    }

    return true;
}

/*!
 * \brief  Finds the boundaries by the Schmitt trigger
 *
 * Runs Schmitt-trigger edge preparation over all active spans collected by the
 * RMS envelope detector.  The configured Schmitt window is converted from
 * seconds to samples and clamped to a small minimum size so rolling extrema have
 * enough context to describe a local waveform neighborhood.
 *
 * \param[in]      wav   Source WAV data
 * \param[in,out]  ctx   Processing context containing spans and receiving candidates
 * \return True if all spans were processed successfully
 */
bool nsk_wav_bf_schmitt(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    /* Minimal samples count per window */
    static const size_t windowmin = 3;

    const size_t window = NSK_MAX(
        round(
            wav->format.samplerate *
            nsk_options_program.profile.boundary.schmitt.window
        ),
        windowmin
    );

    for (size_t i = 0; i < ctx->spans.count; i++) {
        if (!_bf_schmitt_span(wav, ctx, &ctx->spans.span[i], window)) {
            return false;
        }
    }

    return true;
}
