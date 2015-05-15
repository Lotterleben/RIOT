#include "metric.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

bool loop_free(struct aodvv2_routing_entry_t *route_1, struct aodvv2_routing_entry_t *route_2)
{
    DEBUG("%s()\n", __func__);

    if (route_1.metricType != route_2.metricType) {
        DEBUG("Loopfreeness unclear: Metric types must be the same.\n");
        return NULL;
    }

    if (route_1.metricType == HOP_COUNT) {
        /* LoopFree (AdvRte, Route) is TRUE when Cost(AdvRte) <= Cost(Route) */
        /* TODO: use cost() function */
        return route_1->metric <= route_2->metric;
    }

    DEBUG("Metric type %i not supported.\n", route_1.metricType);
    return NULL;
}

