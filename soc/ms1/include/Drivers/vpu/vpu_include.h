#ifndef _MNT_INC_H_
#define _MNT_INC_H_
#include <stdint.h> // IWYU pragma: export
#include <stdio.h>  // IWYU pragma: export


#include "vpu_regs.h"

#define Wr64(addr, data) *(volatile uint64_t *)(addr) = (data)
#define Rd64(addr) *(volatile uint64_t *)(addr)
#define Wr8(addr, data) *(volatile uint8_t *)(addr) = (data)
#define Rd8(addr) *(volatile uint8_t *)(addr)
#define Wr(addr, data) *(volatile uint32_t *)(addr) = (data)
#define Rd(addr) *(volatile uint32_t *)(addr)
#define Wr_reg_bits(reg, val, start, len) Wr(reg, ((Rd(reg) & ~(((1L << (len)) - 1) << (start))) | ((uint32_t)(val & ((1L << len) - 1)) << (start))))

#define VPU_FPGA_EN 1

#endif //_MNT_INC_H_