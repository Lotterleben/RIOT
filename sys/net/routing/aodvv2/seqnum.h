/*
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
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


/**
 * @brief Initialize sequence number.
 */
void seqnum_init(void);

/**
 * @brief Get sequence number.
 * 
 * @return sequence number
 */
uint16_t seqnum_get(void);

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
int seqnum_cmp(uint16_t s1, uint16_t s2);

#endif /* SEQNUM_H_ */
