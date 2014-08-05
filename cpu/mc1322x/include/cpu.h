/*
 * cpu.h - mc1322x specific definitions
 * Copyright (C) 2013 Oliver Hahm <oliver.hahm@inria.fr>
 *
 * This source code is licensed under the GNU Lesser General Public License,
 * Version 2.  See the file LICENSE for more details.
 */

/**
 * @defgroup    mc1322x Freescale MC1322x
 * @ingroup     cpu
 * @brief       Freescale MC1322x specific code
 */

#ifndef CPU_H
#define CPU_H

/**
 * @defgroup    mc1322x     Freescale mc1322x
 * @ingroup     cpu
 * @{
 */

#include <stdbool.h>
#include "arm_cpu.h"
#include "mc1322x.h"

extern uintptr_t __stack_start;     ///< end of user stack memory space
bool install_irq(int int_number, void *handler_addr, int priority);

/** @} */
#endif /* CPU_H */
