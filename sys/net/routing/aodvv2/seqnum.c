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
 * @file        seqnum.c
 * @brief       aodvv2 sequence number
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#include "debug.h"

#include "seqnum.h"

#define ENABLE_DEBUG (0)

static aodvv2_seqnum_t seqnum;

void seqnum_init(void)
{
    seqnum = 1;
}

void seqnum_inc(void)
{
    if (seqnum == 65535) {
        seqnum = 1;
    }
    else if (seqnum == 0) {
        DEBUG("ERROR: SeqNum shouldn't be 0! \n"); /* TODO handle properly */
    }
    else {
        seqnum++;
    }
}

aodvv2_seqnum_t seqnum_get(void)
{
    return seqnum;
}

int seqnum_cmp(aodvv2_seqnum_t s1, aodvv2_seqnum_t s2)
{
    aodvv2_seqnum_t diff = s1 - s2;
    if (diff == 0) {
        return 0;
    }
    if ((0 < diff) && (diff < 32768)) {
        return 1;
    }
    return -1;
}
