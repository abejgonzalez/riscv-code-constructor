#ifndef _BEAGLE_BOOT_H
#define _BEAGLE_BOOT_H

#ifndef __ASSEMBLY__
#include <beagle/sections.h>
#endif /* !__ASSEMBLY__ */

#ifndef PMU_HARTID
#define PMU_HARTID      8
#endif

#ifndef PAYLOAD_BASE
#define PAYLOAD_BASE    _spad
#endif

#endif /* _BEAGLE_BOOT_H */
