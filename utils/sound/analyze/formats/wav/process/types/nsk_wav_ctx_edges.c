#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_ctx_edges.h"

/*!
 * \brief  Appends an edge timestamp to an edge list
 *
 * \param[in,out] edge       Edge list
 * \param[in]     timestamp  Edge timestamp in seconds
 * \return True if the timestamp was appended successfully
 */
bool nsk_wav_ctx_edge(
    struct nsk_wav_ctx_edge_list *edge,
    double                        timestamp
) {
    __typeof__(edge->edge) ptr = realloc(
        edge->edge,
        sizeof(*ptr) * (edge->count + 1)
    );

    if (!ptr) {
        nsk_err("Cannot realloc the edge list");
        return false;
    }

    edge->edge = ptr;
    edge->edge[edge->count++] = timestamp;

    return true;
}
