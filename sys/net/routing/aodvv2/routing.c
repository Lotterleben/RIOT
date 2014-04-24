/*
 * Cobbled-together routing table.
 * This is neither efficient nor elegant, but until RIOT gets their own native
 * RT, this will have to do.
 */

#include "routing.h" 

#define ENABLE_DEBUG (1)
#include "debug.h"

/* helper functions */
static void _reset_entry_if_stale(uint8_t i);

static struct aodvv2_routing_entry_t routing_table[AODVV2_MAX_ROUTING_ENTRIES];
timex_t now, null_time, max_seqnum_lifetime, active_interval, max_idletime;
struct netaddr_str nbuf;

void routingtable_init(void)
{   
    null_time = timex_set(0,0);
    max_seqnum_lifetime = timex_set(AODVV2_MAX_SEQNUM_LIFETIME,0);
    active_interval = timex_set(AODVV2_ACTIVE_INTERVAL, 0);
    max_idletime = timex_set(AODVV2_MAX_IDLETIME, 0);

    for (uint8_t i = 0; i < AODVV2_MAX_ROUTING_ENTRIES; i++) {
        memset(&routing_table[i], 0, sizeof(routing_table[i]));
    }
    DEBUG("[aodvv2] routing table initialized.\n");
}

struct netaddr* routingtable_get_next_hop(struct netaddr* addr, uint8_t metricType)
{
    struct aodvv2_routing_entry_t* entry = routingtable_get_entry(addr, metricType);
    if (!entry)
        return NULL;
    return(&entry->nextHopAddr);
}


void routingtable_add_entry(struct aodvv2_routing_entry_t* entry)
{
    /* only update if we don't already know the address
     * TODO: does this always make sense?
     */
    if (!(routingtable_get_entry(&(entry->addr), entry->metricType))){
        /*find free spot in RT and place rt_entry there */
        for (uint8_t i = 0; i < AODVV2_MAX_ROUTING_ENTRIES; i++){
            if (routing_table[i].addr._type == AF_UNSPEC) {
                /* TODO: sanity check? */
                memcpy(&routing_table[i], entry, sizeof(struct aodvv2_routing_entry_t));
                return;
            }
        }
    }
}

/*
 * retrieve pointer to a routing table entry. To edit, simply follow the pointer.
 */
struct aodvv2_routing_entry_t* routingtable_get_entry(struct netaddr* addr, uint8_t metricType)
{   
    for (uint8_t i = 0; i < AODVV2_MAX_ROUTING_ENTRIES; i++) {
        _reset_entry_if_stale(i);

        if (!netaddr_cmp(&routing_table[i].addr, addr)
            && routing_table[i].metricType == metricType) {
            return &routing_table[i];
        }
    }
    return NULL;
}

void routingtable_delete_entry(struct netaddr* addr, uint8_t metricType)
{
    for (uint8_t i = 0; i < AODVV2_MAX_ROUTING_ENTRIES; i++) {
        _reset_entry_if_stale(i);

        if (!netaddr_cmp(&routing_table[i].addr, addr)
            && routing_table[i].metricType == metricType) {
            memset(&routing_table[i], 0, sizeof(routing_table[i]));
            return;
        }
    }
}

/**
 * Find all routing table entries that use hop as their nextHopAddress, mark them
 * as broken, write the active one into unreachable_nodes[] and increment len 
 * accordingly. (Sorry about the Name.)
 * TODO test/debug somehow
 *
 * @param hop
 * @param unreachable_nodes[] array to be filled. should be empty.
 * @param len int where the future length of unreachable_nodes[] should be noted
 */
void routingtable_break_and_get_all_hopping_over(struct netaddr* hop, struct unreachable_node unreachable_nodes[], int* len) 
{
    *len = 0; // to be sure

    for (uint8_t i = 0; i < AODVV2_MAX_ROUTING_ENTRIES; i++) {
        _reset_entry_if_stale(i);

        if (netaddr_cmp(&routing_table[i].nextHopAddr, hop) == 0) {
            if (routing_table[i].state == ROUTE_STATE_ACTIVE &&
                *len < AODVV2_MAX_UNREACHABLE_NODES) {
                // when the max number of unreachable nodes is reached we're screwed.
                // the above check is just damage control. TODO use autobuf?
                unreachable_nodes[*len].addr = routing_table[i].addr;
                unreachable_nodes[*len].seqnum = routing_table[i].seqnum;
                
                (*len)++;
                DEBUG("\t[routing] unreachable node found: %s\n", netaddr_to_string(&routing_table[i].nextHopAddr, &nbuf));
            }
            routing_table[i].state = ROUTE_STATE_BROKEN;
            DEBUG("\t[routing] number of unreachable nodes: %i\n", *len);
        }
    }
}

/* 
 * Check if entry at index i is stale as described in Section 6.3. 
 * and clear the struct it fills if it is
 */
static void _reset_entry_if_stale(uint8_t i)
{
    vtimer_now(&now);
    int state;
    timex_t lastUsed, expirationTime;

    if (timex_cmp(routing_table[i].expirationTime, null_time) != 0) {
        state = routing_table[i].state;
        lastUsed = routing_table[i].lastUsed;
        expirationTime = routing_table[i].expirationTime;

        /* an Active route is considered to remain active as long as it is used at least once
           during every ACTIVE_INTERVAL. When a route is no longer Active, it becomes an Idle route. */
        
        // if the node is younger than the active interval, don't bother
        if (timex_cmp(now, active_interval) < 0)
            return;

        if (state == ROUTE_STATE_ACTIVE &&
            timex_cmp(timex_sub(now, active_interval), lastUsed) == 1) {
            
            /*
            printf("\tnow:\n");
            timex_print(now);
            printf("\tactive_interval:\n");
            timex_print(active_interval);
            printf("\ttimex_sub(now, active_interval):\n");
            timex_print(timex_sub(now, active_interval));
            printf("\tlastUsed:\n");
            timex_print(lastUsed);
            */

            DEBUG("\t[routing] route towards %s Idle\n", netaddr_to_string(&nbuf, &routing_table[i].addr), i);
            routing_table[i].state = ROUTE_STATE_IDLE;
            routing_table[i].lastUsed = now; // mark the time entry was set to Idle
        }
        /* After an idle route remains Idle for MAX_IDLETIME, it becomes an Expired route. 
           A route MUST be considered Expired if Current_Time >= Route.ExpirationTime
        */


        // if the node is younger than the expiration time, don't bother
        if (timex_cmp(now, expirationTime) < 0)
            return;

        if (state == ROUTE_STATE_IDLE &&
                (timex_cmp(timex_sub(now, max_idletime), lastUsed) == 1  || 
                timex_cmp(expirationTime, now) < 1)) {
            /*
            printf("\tnow:\n");
            timex_print(now);
            printf("\tmax_idletime:\n");
            timex_print(max_idletime);
            printf("\ttimex_sub(now, max_idletime):\n");
            timex_print(timex_sub(now, max_idletime));
            printf("\tlastUsed:\n");
            timex_print(lastUsed);
            printf("\expirationTime:\n");
            timex_print(expirationTime);
            printf("\ttimex_cmp(expirationTime, now):\n");
            printf("\t%i\n",timex_cmp(expirationTime, now));
            */

            DEBUG("\t[routing] route towards %s Expired\n", netaddr_to_string(&nbuf, &routing_table[i].addr), i);
            routing_table[i].state = ROUTE_STATE_EXPIRED;
            routing_table[i].lastUsed = now; // mark the time entry was set to Expired
        }
        /* After that time, old sequence number information is considered no longer 
           valuable and the Expired route MUST BE expunged */
        if(timex_cmp(timex_sub(now, lastUsed), max_seqnum_lifetime) >= 0) {
            DEBUG("\t[routing] reset routing table entry for %s at %i\n", netaddr_to_string(&nbuf, &routing_table[i].addr), i);
            memset(&routing_table[i], 0, sizeof(routing_table[i]));
        }
    }
}

void print_routingtable(void)
{   
    printf("===== BEGIN ROUTING TABLE ===================\n");
    for(int i = 0; i < AODVV2_MAX_ROUTING_ENTRIES; i++) {
        // route has been used before => non-empty entry
        if (routing_table[i].lastUsed.seconds || routing_table[i].lastUsed.microseconds) {
            print_routingtable_entry(&routing_table[i]);
        }
    }
    printf("===== END ROUTING TABLE =====================\n");
}

void print_routingtable_entry(struct aodvv2_routing_entry_t* rt_entry)
{   
    struct netaddr_str nbuf;

    printf(".................................\n");
    printf("\t address: %s\n", netaddr_to_string(&nbuf, &(rt_entry->addr))); 
    printf("\t seqnum: %i\n", rt_entry->seqnum);
    printf("\t nextHopAddress: %s\n", netaddr_to_string(&nbuf, &(rt_entry->nextHopAddr)));
    printf("\t lastUsed: %"PRIu32":%"PRIu32"\n", rt_entry->lastUsed.seconds, rt_entry->lastUsed.microseconds);
    printf("\t expirationTime: %"PRIu32":%"PRIu32"\n", rt_entry->expirationTime.seconds, rt_entry->expirationTime.microseconds);
    printf("\t metricType: %i\n", rt_entry->metricType);
    printf("\t metric: %d\n", rt_entry->metric);
    printf("\t state: %d\n", rt_entry->state);
}
