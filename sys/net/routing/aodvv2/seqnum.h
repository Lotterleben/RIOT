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
 * @file        seqnum.h
 * @brief       aodvv2 sequence number
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#ifndef SEQNUM_H_
#define SEQNUM_H_
#include <stdint.h>

#include "aodvv2/types.h"

/**
 * @brief Initialize sequence number.
 */
void seqnum_init(void);

/**
 * @brief Get sequence number.
 *
 * @return sequence number
 */
aodvv2_seqnum_t seqnum_get(void);

/**
 * @brief Increment the sequence number by 1.
 */
void seqnum_inc(void);

/**
 * @brief Compare 2 sequence numbers.
 * @param[in] s1  first sequence number
 * @param[in] s2  second sequence number
 * @return        -1 when s1 is smaller, 0 if equal, 1 if s1 is bigger.
 */
int seqnum_cmp(aodvv2_seqnum_t s1, aodvv2_seqnum_t s2);

#endif /* SEQNUM_H_ */
