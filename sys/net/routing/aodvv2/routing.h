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
 * @file        routing.h
 * @brief       Cobbled-together routing table. 
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */


#ifndef ROUTING_H_
#define ROUTING_H_

#include <string.h>

#include "ipv6.h"
#include "common/netaddr.h"
#include "constants.h"

/*
 * A route table entry (i.e., a route) may be in one of the following states:
 */
enum aodvv2_routing_states {
    ROUTE_STATE_ACTIVE,
    ROUTE_STATE_IDLE,
    ROUTE_STATE_EXPIRED,
    ROUTE_STATE_BROKEN,
    ROUTE_STATE_TIMED
};

/**
 * all fields of a routing table entry 
 */
struct aodvv2_routing_entry_t {
    struct netaddr addr; 
    uint8_t seqnum;
    struct netaddr nextHopAddr;
    timex_t lastUsed;
    timex_t expirationTime;
    uint8_t metricType;
    uint8_t metric;
    uint8_t state; /* see aodvv2_routing_states */
};

/**
 * @brief     Initialize routing table.
 */
void routingtable_init(void);

/**
 * @brief     Get next hop towards dest. 
 *            Returns NULL if dest is not in routing table.
 *
 * @param[in] dest        Destination of the packet
 * @param[in] metricType  Metric Type of the desired route
 * @return                next hop towards dest if it exists, NULL otherwise
 */
struct netaddr* routingtable_get_next_hop(struct netaddr* dest, uint8_t metricType);

/**
 * @brief     Add new entry to routing table, if there is no other entry 
 *            to the same destination.
 *
 * @param[in] entry        The routing table entry to add
 */
void routingtable_add_entry(struct aodvv2_routing_entry_t* entry);

/**
 * @brief     Retrieve pointer to a routing table entry. 
 *            To edit, simply follow the pointer.
 *            Returns NULL if addr is not in routing table.
 *
 * @param[in] addr          The address towards which the route should point
 * @param[in] metricType    Metric Type of the desired route
 * @return                  Routing table entry if it exists, NULL otherwise
 */
struct aodvv2_routing_entry_t* routingtable_get_entry(struct netaddr* addr, uint8_t metricType);

/**
 * @brief     Delete routing table entry towards addr with metric type MetricType,
 *            if it exists.
 *
 * @param[in] addr          The address towards which the route should point
 * @param[in] metricType    Metric Type of the desired route
 */
void routingtable_delete_entry(struct netaddr* addr, uint8_t metricType);

void print_routingtable(void);
void print_routingtable_entry(struct aodvv2_routing_entry_t* rt_entry);
/**
 * Find all routing table entries that use hop as their nextHopAddress, mark them
 * as broken, write the active one into unreachable_nodes[] and increment len 
 * accordingly. (Sorry about the Name.)
 *
 * @param hop                 Address of the newly unreachable next hop
 * @param unreachable_nodes[] array of newlu unreachable nodes to be filled.
 *                            should be empty.
 * @param len                 int* which will contain the length of 
 *                            unreachable_nodes[] after execution
 */
void routingtable_break_and_get_all_hopping_over(struct netaddr* hop, struct unreachable_node unreachable_nodes[], int* len);

#endif /* ROUTING_H_ */ 
