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
 * @file        utils.h
 * @brief       client- and RREQ-table, ipv6 address representation converters
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <stdio.h>

#include "ipv6.h"

#include "common/netaddr.h"

#include "constants.h"
#include "seqnum.h"

#define AODVV2_MAX_CLIENTS 1    /* multiple clients are currently not supported. */
#define AODVV2_RREQ_BUF 128     /* should be enough for now... */
#define AODVV2_RREQ_WAIT_TIME 2 /* seconds */

/**
 * @brief   RREQ Table entry
 */
struct aodvv2_rreq_entry
{
    struct netaddr origNode;
    struct netaddr targNode;
    uint8_t metricType;
    uint8_t metric;
    uint16_t seqnum;
    timex_t timestamp;
};

/**
 * Initialize table of clients that the router currently serves.
 */
void clienttable_init(void);

/**
 * Add client to the list of clients that the router currently serves.
 * @param address   address of the client
 *                  (Since the current version doesn't offer support for Client Networks,
 *                  the prefixlen is currently ignored.)
 */
void clienttable_add_client(struct netaddr *addr);

/*
 * Find out if a client is in the list of clients that the router currently serves.
 * @param address   address of the client in question
 *                  (Since the current version doesn't offer support for Client Networks,
 *                  the prefixlen is currently ignored.)
 */
bool clienttable_is_client(struct netaddr *addr);

/*
 * Delete a client from the list of clients that the router currently serves.
 * @param address   address of the client to delete
 *                  (Since the current version doesn't offer support for Client Networks,
 *                  the prefixlen is currently ignored.)
 */
void clienttable_delete_client(struct netaddr *addr);

/*
 * Initialize RREQ table.
 */
void rreqtable_init(void);

/**
 * Check if a RREQ is redundant, i.e. was received from another node already.
 * Behaves as described in Sections 5.7. and 7.6.
 * @param paket_data  data of the RREQ in question
 */
bool rreqtable_is_redundant(struct aodvv2_packet_data *packet_data);

/**
 * Convert an IP stored as an ipv6_addr_t to a netaddr
 * @param src       ipv6_addr_t to convert
 * @param dst       (empty) netaddr to convert into
 */
void ipv6_addr_t_to_netaddr(ipv6_addr_t *src, struct netaddr *dst);

/**
 * Convert an IP stored as a netaddr to an ipv6_addr_t
 * @param src       (empty) netaddr to convert into
 * @param dst       ipv6_addr_t to convert
 */
void netaddr_to_ipv6_addr_t(struct netaddr *src, ipv6_addr_t *dst);

#endif /* UTILS_H_ */
