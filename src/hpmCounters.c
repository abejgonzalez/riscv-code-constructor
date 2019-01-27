#include <stdio.h>
#include "hpm-util.h"

int main (void){
    // enable priv
    HPM_EN_USER();

    // read rocket exception (should be false)
    HPM_SETUP_EVENTS(CSR_HPMCOUNTER3, 0, 0);

    printf("cycle(%lu)\n", read_csr(cycle));
    printf("instr(%lu)\n", read_csr(instret));
    printf("hpm0(%lu)\n", HPM_READ(CSR_HPMCOUNTER3));
    printf("hpm0(%lu)\n", HPM_READ(hpmcounter3));
}
