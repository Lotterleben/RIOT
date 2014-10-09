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

#include "common/netaddr.h"
#include "rfc5444/rfc5444_print.h"

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/* RFC5498 */
#define MANET_PORT  269

enum aodvv2_constants {
    AODVV2_MAX_HOPCOUNT = 250,          /* as specified in the AODVv2 draft, section 14.2.*/
    AODVV2_MAX_ROUTING_ENTRIES = 255,
    AODVV2_DEFAULT_METRIC_TYPE = 3,
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
    struct netaddr addr;
    uint8_t metric;
    uint16_t seqnum;
};

/**
 * @brief   all data contained in a RREQ or RREP.
 */
struct aodvv2_packet_data
{
    uint8_t hoplimit;
    struct netaddr sender;
    uint8_t metricType;
    struct node_data origNode;
    struct node_data targNode;
    timex_t timestamp;
};

/**
 * @brief   Address and sequence number an unreachable node to be embedded in a RERR.
 */
struct unreachable_node
{
    struct netaddr addr;
    uint16_t seqnum;
};

#endif /* CONSTANTS_H_ */
