/*
 * Copyright (C) 2015 HAW Hamburg
 * Copyright (C) 2015 Lotte Steenbrink <lotte.steenbrink@haw-hamburg.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     aodvv2
 * @{
 *
 * @file        metric.c
 * @brief       create and handle metric values and types
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@haw-hamburg.de>
 */

#ifndef AODVV2_METRIC_H_
#define AODVV2_METRIC_H_

#include "aodvv2/types.h"
#include "routingtable.h"

/**
 * @brief     Check whether route_1 relies on route_2, which may cause routing loops.
 *
 * @param[in] route_1     TODO
 * @param[in] route_2     TODO
 * @param[in] metric_type TODO
 *
 * @return  true  when route_1 is guaranteed to not rely on route_2,
 *                i.e. route_2 is not a subroute of route_1.
 *          NULL  if metric_type is not supported
 *          false otherwise.
 */
bool loop_free(struct aodvv2_routing_entry_t *route_1, struct aodvv2_routing_entry_t *route_2,
               aodvv2_metric_t metric_type);

#endif /* AODVV2_METRIC_H_ */
