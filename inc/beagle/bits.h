#ifndef _BEAGLE_BITS_H
#define _BEAGLE_BITS_H

#if __riscv_xlen == 64
#define LOAD            ld
#define STORE           sd
#define REGBYTES        8
#elif __riscv_xlen == 32
#define LOAD            lw
#define STORE           sw
#define REGBYTES        4
#else
#error "Unexpected __riscv_xlen"
#endif

#endif /* _BEAGLE_BITS_H */
