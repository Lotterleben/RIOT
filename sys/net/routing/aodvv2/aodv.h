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
 * @file        aodv.h
 * @brief       aodvv2 routing protocol
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#ifndef AODV_H_
#define AODV_H_

#include <sixlowpan/ip.h>
#include "sixlowpan.h"
#include "kernel.h"
#include "udp.h"
#include "socket_base/socket.h"
#include "transceiver.h"
#include "net_help.h"
#include "mutex.h"

#include "constants.h"
#include "seqnum.h"
#include "routing.h"
#include "utils.h"
#include "reader.h"
#include "writer.h"
#include "thread.h"

/**
 * @brief   This struct contains data which needs to be put into a RREQ or RREP.
 *          It is used to transport this data in a message to the sender_thread.
 *          Please note that it is for internal use only. To send a RREQ or RREP,
 *          please use the aodv_send_rreq() and aodv_send_rrep() functions.
 */
struct rreq_rrep_data
{
    struct aodvv2_packet_data *packet_data;
    struct netaddr *next_hop;
};

/**
 * @brief   This struct contains data which needs to be put into a RERR.
 *          It is used to transport this data in a message to the sender_thread.
 *          Please note that it is for internal use only. To send a RERR,
 *          please use the aodv_send_rerr() function.
 */
struct rerr_data
{
    struct unreachable_node *unreachable_nodes; /* Beware, this is the start of an array. */
    int len;
    int hoplimit;
    struct netaddr *next_hop;
};


/**
 * @brief   This struct holds the data for a RREQ, RREP or RERR (contained
 *          in a rreq_rrep_data or rerr_data struct) and the next hop the RREQ, RREP
 *          or RERR should be sent to. It used for message communication with
 *          the sender_thread.
 *          Please note that it is for internal use only. To send a RERR,
 *          please use the aodv_send_rerr() function.
 */
struct msg_container
{
    int type;
    void *data;
};

/**
 * @brief   Set the metric type. If metric_type does not match any known metric
 *          types, no changes will be made.
 *
 * @param[in] metric_type       type of new metric
 */
void aodv_set_metric_type(int metric_type);

/**
 * @brief   When set as ipv6_iface_routing_provider, this function is called by
 *          ipv6_sendto() to determine the next hop towards dest. This function
 *          is non-blocking.
 *
 * @param[in] destination of the packet
 * @return  Address of the next hop towards dest if there is any,
 *          NULL if there is none (yet)
 */
static ipv6_addr_t *aodv_get_next_hop(ipv6_addr_t *dest);

/**
 * @brief   Dispatch a RREQ
 *
 * @param[in] packet_data  Payload of the RREQ
 */
void aodv_send_rreq(struct aodvv2_packet_data *packet_data);

/**
 * @brief   Dispatch a RREP
 *
 * @param[in] packet_data  Payload of the RREP
 * @param[in] next_hop     Address of the next hop the RREP should be sent to
 */
void aodv_send_rrep(struct aodvv2_packet_data *packet_data, struct netaddr *next_hop);

/**
 * @brief   Dispatch a RERR
 *
 * @param[in] unreachable_nodes  All nodes that are marked as unreachable
 *                               by this RERR
 * @param[in] len                Number of unreachable nodes
 * @param[in] hoplimit           Hoplimit of RERR
 * @param[in] next_hop           Address of the next hop the RERR should be sent to
 */
void aodv_send_rerr(struct unreachable_node unreachable_nodes[], int len, int hoplimit, struct netaddr *next_hop);

#endif /* AODV_H_ */
