#include "metric.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

bool loop_free(struct aodvv2_routing_entry_t *route_1, struct aodvv2_routing_entry_t *route_2,
               aodvv2_metric_t metric_type)
{
    DEBUG("%s()\n", __func__);

    if (metric_type == HOP_COUNT) {
        /* LoopFree (AdvRte, Route) is TRUE when Cost(AdvRte) <= Cost(Route) */
        /* TODO: use cost() function */
        return route_1->metric <= route_2->metric;
    }

    return NULL;
    DEBUG("Metric type %i not supported.\n", metric_type);
}
