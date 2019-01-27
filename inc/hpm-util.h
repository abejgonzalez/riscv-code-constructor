// ----------------------------------------------------------------
// HPM Architectural Event Tracking
// ----------------------------------------------------------------
// Follows the structure laid out by:
//   https://static.dev.sifive.com/U54-MC-RVCoreIP.pdf - Sec. 3.11
// and:
//   docs.boom-core.org - Sec. Micro-arch Event Tracking
// ----------------------------------------------------------------
#ifndef HPM_UTIL_H
#define HPM_UTIL_H

#include "encoding.h"

// TODO:
//     use read_csr_safe?

/**
 * Quick description of HPM's and their use
 *
 * According to RISC-V Priv. 1.10, HPMs are used to 
 * keep track of architectural events.
 *
 * To use:
 *     If using in user mode:
 *         HPM_EN_USER();
 *         HPM_SETUP_EVENTS(Use HPMCOUNTER here)
 *         HPM_READ(...)
 *     If using in supervisor mode:
 *         HPM_EN_USER();
 *         HPM_SETUP_EVENTS(Use HPMCOUNTER here)
 *         HPM_READ(...)
 *     If using in machine mode:
 *         HPM_EN_USER();
 *         HPM_SETUP_EVENTS(Use HPMCOUNTER or
 *                              MHPMCOUNTER here)
 *         HPM_READ(...)
 */

/**
 * Enable user use of HPM counters
 */
#define HPM_EN_USER() ({ \
            write_csr(mcounteren, -1); \
            write_csr(scounteren, -1); \
        })

/**
 * Enable supervisor use of HPM counters
 */
#define HPM_EN_SUPER() ({ \
            write_csr(mcounteren, -1); \
        })

/**
 * Read the HPM specified
 *
 * @param name reg name of the HPM to read
 * @return unsigned long result
 */
#define HPM_READ(name) ({ \
            read_csr(name); \
        })

/**
 * Setup the HPM specified
 *
 * @param name reg name of the HPM to setup
 * @param eventset what eventset to look at
 *                 8b number
 * @param events event mask to enable tracking those events
 *               8b number
 * @return none
 */
#define HPM_SETUP_EVENTS(name, eventset, events) ({ \
            write_csr(name, (events << 8) & eventset); \
        })

#endif
