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
 * @file        constants.h
 * @brief       constants for the aodvv2 routing protocol
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#ifndef AODVV2_CONSTANTS_H_
#define AODVV2_CONSTANTS_H_

#include "aodvv2/types.h"

#include "common/netaddr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RFC5498 */
#define MANET_PORT  269

enum aodvv2_constants {
    AODVV2_MAX_HOPCOUNT = 250,          /* as specified in the AODVv2 draft, section 14.2.*/
    AODVV2_MAX_ROUTING_ENTRIES = 255,
    AODVV2_ACTIVE_INTERVAL = 5,         /* seconds */
    AODVV2_MAX_IDLETIME = 250,          /* seconds */
    AODVV2_MAX_SEQNUM_LIFETIME = 300,   /* seconds */
    AODVV2_RIOT_PREFIXLEN = 128,
    AODVV2_MAX_UNREACHABLE_NODES = 15,  /* TODO: choose value (wisely) */
};

/**
 * @brief   AODVv2 message types
 */
enum rfc5444_msg_type
{
    RFC5444_MSGTYPE_RREQ = 10,
    RFC5444_MSGTYPE_RREP = 11,
    RFC5444_MSGTYPE_RERR = 12,
};

/**
 * @brief   AODVv2 TLV types
 */
enum rfc5444_tlv_type
{
    RFC5444_MSGTLV_ORIGSEQNUM,
    RFC5444_MSGTLV_TARGSEQNUM,
    RFC5444_MSGTLV_UNREACHABLE_NODE_SEQNUM,
    RFC5444_MSGTLV_METRIC,
};

/**
 * @brief   TLV type array indices
 */
enum tlv_index
{
    TLV_ORIGSEQNUM,
    TLV_TARGSEQNUM,
    TLV_UNREACHABLE_NODE_SEQNUM,
    TLV_METRIC,
};

/* my multicast address */
struct netaddr na_mcast;

/**
 * @brief   Data about an OrigNode or TargNode, typically embedded in an
 *          aodvv2_packet_data struct.
 */
struct node_data
{
    struct netaddr addr;                        /**< IP address of the node */
    uint8_t metric;                             /**< Metric value */
    aodvv2_seqnum_t seqnum;                     /**< Sequence Number */
};

/**
 * @brief   all data contained in a RREQ or RREP.
 */
struct aodvv2_packet_data
{
    uint8_t hoplimit;                           /**< Hop limit */
    struct netaddr sender;                      /**< IP address of the neighboring router which sent the RREQ/RREP*/
    aodvv2_metric_t metricType;                 /**< Metric type */
    struct node_data origNode;                  /**< Data about the originating node */
    struct node_data targNode;                  /**< Data about the originating node */
    timex_t timestamp;                          /**< point at which the packet was (roughly) received. Note that this
                                                     timestamp will be set after the packet has been successfully parsed. */
};

/**
 * @brief   Data about an unreachable node to be embedded in a RERR.
 */
struct unreachable_node
{
    struct netaddr addr;                        /**< IP address */
    aodvv2_seqnum_t seqnum;                     /**< Sequence Number */
};

#ifdef  __cplusplus
}
#endif

#endif /* AODVV2_CONSTANTS_H_ */
