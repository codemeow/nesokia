#pragma once

#include <stdbool.h>
#include <stddef.h>

/*!
 * \brief  One polarity edge list
 */
struct nsk_wav_ctx_edge_list {
    size_t count; /*!< Number of edges */
    double *edge; /*!< Array of edge timestamps */
};

/*!
 * \brief  Edge storage owned by the processing context
 */
struct nsk_wav_ctx_edges {
    struct nsk_wav_ctx_edge_list rise; /*!< Rising edges */
    struct nsk_wav_ctx_edge_list fall; /*!< Falling edges */
};

/*!
 * \brief  Appends an edge timestamp to an edge list
 *
 * \param[in,out] edge       Edge list
 * \param[in]     timestamp  Edge timestamp in seconds
 * \return True if the timestamp was appended successfully
 */
__attribute__((warn_unused_result))
bool nsk_wav_ctx_edge(
    struct nsk_wav_ctx_edge_list *edge,
    double                        timestamp
);
