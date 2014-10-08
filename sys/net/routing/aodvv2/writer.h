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
 * @file        writer.c
 * @brief       writer to create RFC5444 aodvv2 messages
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#ifndef WRITER_H_
#define WRITER_H_

#include "common/common_types.h"
#include "rfc5444/rfc5444_writer.h"
#include "reader.h"

#include "common/common_types.h"
#include "common/netaddr.h"
#include "rfc5444/rfc5444_writer.h"
#include "rfc5444/rfc5444_iana.h"
#include "mutex.h"

#include "constants.h"
#include "seqnum.h"

/**
 * @brief   Wrapper for the rfc5444_writer_target that the _write_packet() callback receives.
 *          _write_packet() needs to know the type, payload and target address
 *          of the RFC5444 message to be sent as well, but the oonf api does not
 *          offer this feature. Having this wrapper enables the use of the
 *          container_of macro to fetch this information. It is hacky, but it does the trick.
 */
struct writer_target
{
    struct rfc5444_writer_target interface;
    struct netaddr target_addr;
    struct aodvv2_packet_data packet_data;
    int type;
};

typedef void (*write_packet_func_ptr)(
    struct rfc5444_writer *wr, struct rfc5444_writer_target *iface, void *buffer, size_t length);

/**
 * Initialize RFC5444 writer
 * @param ptr pointer to "send_packet" callback
 */
void writer_init(write_packet_func_ptr ptr);

/**
 * Clean up after the RFC5444 writer
 */
void writer_cleanup(void);

/**
 * Send a RREQ. DO NOT use this function to dispatch packets from anything else
 * than the sender_thread. To send RREQs, use aodv_send_rreq().
 * @param packet_data parameters of the RREQ
 * @param next_hop Address the RREP is sent to
 */
void writer_send_rreq(struct aodvv2_packet_data *packet_data, struct netaddr *next_hop);

/**
 * Send a RREP. DO NOT use this function to dispatch packets from anything else
 * than the sender_thread. To send RREPs, use aodv_send_rrep().
 * @param packet_data parameters of the RREP
 * @param next_hop Address the RREP is sent to
 */
void writer_send_rrep(struct aodvv2_packet_data *packet_data, struct netaddr *next_hop);

/**
 * Send a RERR. DO NOT use this function to dispatch packets from anything else
 * than the sender_thread. To send RERRs, use aodv_send_rerr().
 * @param unreachable_nodes[] array containing all newly unreachable nodes. each
 *                            in a struct unreachable_node
 * @param len                 length of unreachable_nodes[]
 * @param hoplimit            the message's hop limit
 * @param next_hop            Address the RREP is sent to
 */
void writer_send_rerr(struct unreachable_node unreachable_nodes[], int len, int hoplimit, struct netaddr *next_hop);

#endif /* WRITER_H_ */
