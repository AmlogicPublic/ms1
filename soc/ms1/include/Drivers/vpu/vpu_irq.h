// See LICENSE for license details.

#ifndef _VPU_IRQ_H
#define _VPU_IRQ_H

#include <stdint.h>

int vpu_irq_init(void);
void eclic_disp0_int_handler(void);


#endif /* _VPU_IRQ_H */
