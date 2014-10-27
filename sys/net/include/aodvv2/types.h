/*
 * Copyright (C) 2014 Freie Universität Berlin
 * Copyright (C) 2014 Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     aodvv2
 * @{
 *
 * @file        aodvv2/types.h
 * @brief       data types for the aodvv2 routing protocol
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#ifndef AODVV2_TYPES_H
#define AODVV2_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   AODVv2 metric types. Extend to include alternate metrics.
 */
typedef enum {
    HOP_COUNT = 3,              /**< see RFC6551*/
} aodvv2_metric_t;

typedef uint16_t aodvv2_seqnum_t;

#define AODVV2_DEFAULT_METRIC_TYPE HOP_COUNT

#ifdef  __cplusplus
}
#endif

#endif /* AODVV2_TYPES_H */
