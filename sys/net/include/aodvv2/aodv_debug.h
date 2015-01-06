/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @addtogroup  core_util
 * @{
 *
 * @ingroup     aodvv2
 * @brief       Debug-header for aodvv2 debug messages
 *
 *
 * @author      Lotte Steenbrink <lotte.steenbrink@fu-berlin.de>
 */

#ifndef AODV_DEBUG_H_
#define AODV_DEBUG_H_

#include <stdio.h>
#include "sched.h"

#ifdef __cplusplus
 extern "C" {
#endif

void (*eval_callback)(char *eval_output);

static void set_eval_callback(void (*function)(char *eval_output))
{
    eval_callback = function;
}

#if ENABLE_DEBUG
#define ENABLE_AODV_DEBUG (1)
#endif

/**
 * @brief Print aodvv2 specific debug information to std-out with [aodvv2] prefix
 *
 */
#if ENABLE_AODV_DEBUG
#include "tcb.h"
#define AODV_DEBUG(...) \
 do { \
        printf("[aodvv2] "); \
        printf(__VA_ARGS__); \
    } while (0)
#else
#define AODV_DEBUG(...)
#endif

/**
 * @brief If a callback function eval_callback() is set, pass it the parameters of
 *        EVAL_DEBUG(). Otherwise, just treat it like regular debug output and print it.
 *
 */
#if ENABLE_EVAL_DEBUG
#include "tcb.h"
#define EVAL_DEBUG(...) \
 do { \
        if (eval_callback) { \
            eval_callback(__VA_ARGS__); \
        } \
        else { \
            printf(__VA_ARGS__); \
        } \
    } while (0)
#else
#define EVAL_DEBUG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif /* AODVV2_DEBUG_H_*/
/** @} */
