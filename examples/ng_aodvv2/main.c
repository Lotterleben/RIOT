/*
 * Copyright (C) 2015
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief
 *
 * @author
 *
 * @}
 */

#include <stdio.h>
#include "net/ng_ipv6.h"
#include "net/ng_netbase.h"
#include "net/ng_netif.h"
#include "transceiver.h"
#include "aodvv2/aodvv2.h"

int main(void)
{
    puts("Hello ng_aodvv2!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    /** We take the first available IF */
    kernel_pid_t ifs[NG_NETIF_NUMOF];
    size_t numof = ng_netif_get(ifs);
    if(numof <= 0) {
        return 1;
    }
puts("done. get the IF");

#ifndef BOARD_NATIVE
    /** We set our channel */
    uint16_t data = 17;
    if (ng_netapi_set(ifs[0], NETCONF_OPT_CHANNEL, 0, &data, sizeof(uint16_t)) < 0) {
        return 1;
    }
puts("done. set NETCONF_OPT_CHANNEL");
    /** We set our pan ID */
    data = 0xabcd;
    if (ng_netapi_set(ifs[0], NETCONF_OPT_NID, 0, &data, sizeof(uint16_t)) < 0) {
        return 1;
    }
puts("done. set NETCONF_OPT_NID");
#endif

ng_ipv6_addr_t addr;
uint8_t prefix_len = 128;
char* addr_str = "2001::1234";
if (ng_ipv6_addr_from_str(&addr, addr_str) == NULL) {
    puts("error: unable to parse IPv6 address.");
    return 1;
}

if (ng_ipv6_netif_add_addr(ifs[0], &addr, prefix_len, NG_IPV6_NETIF_ADDR_FLAGS_UNICAST) == NULL) {
    puts("error: unable to add IPv6 address\n");
    return 1;
}

    aodv_init();

puts("done all.");
    return 0;
}
