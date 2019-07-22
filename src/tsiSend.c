#include <stdio.h>
#include "hpm-util.h"

#define SAI_CMD_READ 0
#define SAI_CMD_WRITE 1

#define TSI_HOST_MMIO_BASE 0x10017000
#define mmio_tx_tsi (TSI_HOST_MMIO_BASE + 0x00)
#define mmio_rx_tsi (TSI_HOST_MMIO_BASE + 0x04)

#define write(addr, data) { *((volatile uint32_t *)(addr)) = data; }
#define read(addr) (*((volatile uint32_t *)(addr)))

#define UNUSED 0xdeadbeef
#define TARGET_MEM_START_ADDR 0x40000000
#define MAX_SEQ_LEN 7
#define TOT_SEQ_AMT 10
#define HEADER_WORDS 5

uint16_t tsi_len[] = { 6, 6, 6, 6, 7, 5, 5, 5, 5, 5 };
//                                                 cmd,               addrLow, addrHigh, lenLow, lenHigh,      [data]
uint32_t tsi_seq[TOT_SEQ_AMT][MAX_SEQ_LEN] = { { SAI_CMD_WRITE, TARGET_MEM_START_ADDR,        0,      0,       0, 0x33221100,     UNUSED },
                                               { SAI_CMD_WRITE, TARGET_MEM_START_ADDR,        0,      0,       0, 0x77665544,     UNUSED },
                                               { SAI_CMD_WRITE, TARGET_MEM_START_ADDR,        0,      0,       0, 0xFFEEDDCC,     UNUSED },
                                               { SAI_CMD_WRITE, TARGET_MEM_START_ADDR,        0,      0,       0, 0x89ABCDEF,     UNUSED },
                                               { SAI_CMD_WRITE, TARGET_MEM_START_ADDR,        0,      1,       0, 0xFEDCBA98, 0x76543210 },
                                               {  SAI_CMD_READ, TARGET_MEM_START_ADDR,        0,      0,       0,     UNUSED,     UNUSED },
                                               {  SAI_CMD_READ, TARGET_MEM_START_ADDR,        0,      0,       0,     UNUSED,     UNUSED },
                                               {  SAI_CMD_READ, TARGET_MEM_START_ADDR,        0,      0,       0,     UNUSED,     UNUSED },
                                               {  SAI_CMD_READ, TARGET_MEM_START_ADDR,        0,      0,       0,     UNUSED,     UNUSED },
                                               {  SAI_CMD_READ, TARGET_MEM_START_ADDR,        0,      1,       0,     UNUSED,     UNUSED } };


int main (void){
    printf("TSIHost: Start integration test\n");
    for ( uint16_t seqIdx = 0; seqIdx < TOT_SEQ_AMT; seqIdx++ ) {
        // write the cmd, addr, and len
        for ( uint16_t inSeqIdx = 0; inSeqIdx < HEADER_WORDS; inSeqIdx++ ) {
            printf("SEND: Req(%d) Data(%x)\n", seqIdx, tsi_seq[seqIdx][inSeqIdx]);
            write(mmio_tx_tsi, tsi_seq[seqIdx][inSeqIdx]);
        }

        if ( tsi_seq[seqIdx][0] == SAI_CMD_READ ) {
            // done start reading values out of MMIO
            printf("READ: %x\n", read(mmio_rx_tsi));
        }
        else {
            // finish writing rest of data
            for ( uint16_t inSeqIdx = HEADER_WORDS; inSeqIdx < tsi_len[seqIdx]; inSeqIdx++ ) {
                printf("SEND: Req(%d) Data(%x)\n", seqIdx, tsi_seq[seqIdx][inSeqIdx]);
                write(mmio_tx_tsi, tsi_seq[seqIdx][inSeqIdx]);
            }
        }
    }
    printf("TSIHost: End integration test\n");
}
