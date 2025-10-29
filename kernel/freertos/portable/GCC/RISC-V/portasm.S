#include "riscv_encoding.h"

#ifndef __riscv_32e
#define portRegNum          30
#else
#define portRegNum          14
#endif

#define portCONTEXT_SIZE    ( portRegNum * REGBYTES )

.section    .text.entry
.align 8

#ifndef configNUMBER_OF_CORES
#define configNUMBER_OF_CORES   1
#endif

.extern xPortTaskSwitch
#if ( configNUMBER_OF_CORES == 1 )
.extern pxCurrentTCB
#else
.extern pxCurrentTCBs
#endif
.global prvPortStartFirstTask

.macro DISABLE_MIE
    csrc CSR_MSTATUS, MSTATUS_MIE
.endm

.macro SAVE_CONTEXT
    /* Simplified: Save directly on current stack (no swap) */
#ifndef __riscv_32e
    addi sp, sp, -32*REGBYTES
#else
    addi sp, sp, -14*REGBYTES
#endif

    STORE x1, 0*REGBYTES(sp)
    STORE x4, 1*REGBYTES(sp)
    STORE x5, 2*REGBYTES(sp)
    STORE x6, 3*REGBYTES(sp)
    STORE x7, 4*REGBYTES(sp)
    STORE x10, 5*REGBYTES(sp)
    STORE x11, 6*REGBYTES(sp)
    STORE x12, 7*REGBYTES(sp)
    STORE x13, 8*REGBYTES(sp)
    STORE x14, 9*REGBYTES(sp)
    STORE x15, 10*REGBYTES(sp)
#ifndef __riscv_32e
    /* 保存所有寄存器（除了x0, x2=sp, x3=gp）- 彻底确保没有寄存器被破坏 */
    STORE x8,  14*REGBYTES(sp)
    STORE x9,  15*REGBYTES(sp)
    STORE x16, 16*REGBYTES(sp)
    STORE x17, 17*REGBYTES(sp)
    STORE x18, 18*REGBYTES(sp)
    STORE x19, 19*REGBYTES(sp)
    STORE x20, 20*REGBYTES(sp)
    STORE x21, 21*REGBYTES(sp)
    STORE x22, 22*REGBYTES(sp)
    STORE x23, 23*REGBYTES(sp)
    STORE x24, 24*REGBYTES(sp)
    STORE x25, 25*REGBYTES(sp)
    STORE x26, 26*REGBYTES(sp)
    STORE x27, 27*REGBYTES(sp)
    STORE x28, 28*REGBYTES(sp)
    STORE x29, 29*REGBYTES(sp)
    STORE x30, 30*REGBYTES(sp)
    STORE x31, 31*REGBYTES(sp)
#endif
.endm

.macro RESTORE_CONTEXT
    LOAD x1, 0*REGBYTES(sp)
    LOAD x4, 1*REGBYTES(sp)
    LOAD x5, 2*REGBYTES(sp)
    LOAD x6, 3*REGBYTES(sp)
    LOAD x7, 4*REGBYTES(sp)
    LOAD x10, 5*REGBYTES(sp)
    LOAD x11, 6*REGBYTES(sp)
    LOAD x12, 7*REGBYTES(sp)
    LOAD x13, 8*REGBYTES(sp)
    LOAD x14, 9*REGBYTES(sp)
    LOAD x15, 10*REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x8,  14*REGBYTES(sp)
    LOAD x9,  15*REGBYTES(sp)
    LOAD x16, 16*REGBYTES(sp)
    LOAD x17, 17*REGBYTES(sp)
    LOAD x18, 18*REGBYTES(sp)
    LOAD x19, 19*REGBYTES(sp)
    LOAD x20, 20*REGBYTES(sp)
    LOAD x21, 21*REGBYTES(sp)
    LOAD x22, 22*REGBYTES(sp)
    LOAD x23, 23*REGBYTES(sp)
    LOAD x24, 24*REGBYTES(sp)
    LOAD x25, 25*REGBYTES(sp)
    LOAD x26, 26*REGBYTES(sp)
    LOAD x27, 27*REGBYTES(sp)
    LOAD x28, 28*REGBYTES(sp)
    LOAD x29, 29*REGBYTES(sp)
    LOAD x30, 30*REGBYTES(sp)
    LOAD x31, 31*REGBYTES(sp)
    addi sp, sp, 32*REGBYTES
#else
    addi sp, sp, 14*REGBYTES
#endif
    /* Simplified: No stack swap needed */
.endm

#if defined(__TEE_PRESENT) && (__TEE_PRESENT == 1)
.macro DISABLE_SIE
    csrc CSR_SSTATUS, MSTATUS_SIE
.endm
#endif

.section .text.trap
.align 6
.global exc_entry
.type exc_entry, @function
exc_entry:
    /* Save all general-purpose registers */
    SAVE_CONTEXT
    
    /* Get exception cause and call handler */
    csrr a0, mcause
    mv a1, sp
    call core_intexc_handler
    
    /* Restore all general-purpose registers */
    RESTORE_CONTEXT
    
    /* Return from exception (hardware restores mepc, mstatus) */
    mret
    .size exc_entry, . - exc_entry

.section .text.irq
.align 2
.global irq_entry
.type irq_entry, @function
irq_entry:
    /* Save all general-purpose registers */
    SAVE_CONTEXT
    
    /* Disable global interrupts during handler execution */
    DISABLE_MIE

    /* Get interrupt cause and dispatch to handler */
    csrr a0, mcause            /* read mcause */
    mv   t0, a0                /* save full mcause to t0 */
    li   a1, 0xfff             /* load mask for low 12 bits */
    and  a0, a0, a1            /* extract interrupt number to a0 */
    
    /* Clear interrupt first (save/restore irq number on stack) */
    addi sp, sp, -REGBYTES
    STORE a0, 0*REGBYTES(sp)   /* save interrupt number */
    call clear_eclic_interrupt /* clear interrupt (a0 = irq number) */
    LOAD  a0, 0*REGBYTES(sp)   /* restore interrupt number */
    addi sp, sp, REGBYTES      /* restore sp before next call */
    
    /* Call handler with full mcause and sp */
    mv    a0, t0               /* restore full mcause to a0 */
    mv    a1, sp               /* pass stack pointer (not used by interrupt handlers) */
    call  core_intexc_handler  /* process irq */
    
    /* Restore all general-purpose registers */
    RESTORE_CONTEXT
    
    /* Return from interrupt (hardware restores mepc, mstatus) */
    mret
    .size irq_entry, . - irq_entry

#if defined(__TEE_PRESENT) && (__TEE_PRESENT == 1)

.section .text.trap_s
.align 6
.weak exc_entry_s
.type exc_entry_s, @function
exc_entry_s:
    /* Save all general-purpose registers */
    SAVE_CONTEXT
    
    /* Get exception cause and call handler */
    csrr a0, scause
    mv a1, sp
    call core_exception_handler_s
    
    /* Restore all general-purpose registers */
    RESTORE_CONTEXT
    
    /* Return from supervisor exception (hardware restores sepc, sstatus) */
    sret
    .size exc_entry_s, . - exc_entry_s

.section .text.irq_s
.align 2
.weak irq_entry_s
.type irq_entry_s, @function
irq_entry_s:
    /* Save all general-purpose registers */
    SAVE_CONTEXT
    
    /* Disable supervisor interrupts during handler execution */
    DISABLE_SIE

    /* Get interrupt cause and dispatch to handler */
    csrr a0, scause            /* read scause */
    
    /* Save full scause to stack (must preserve across calls) */
    addi sp, sp, -REGBYTES
    STORE a0, 0*REGBYTES(sp)   /* save full scause */
    
    /* Call handler */
    mv   a1, sp                /* pass stack pointer as second argument */
    addi a1, a1, REGBYTES      /* adjust sp to point to saved context */
    call core_exception_handler_s /* process supervisor irq */
    
    /* Clean up stack */
    addi sp, sp, REGBYTES

    /* Restore all general-purpose registers */
    RESTORE_CONTEXT
    
    /* Return from supervisor interrupt (hardware restores sepc, sstatus) */
    sret
    .size irq_entry_s, . - irq_entry_s

#endif


.type prvPortStartFirstTask, @function
.align 3
prvPortStartFirstTask:
    /* Simplified: No mscratch setup needed - interrupts use current stack directly */
#if defined(SMP_CPU_CNT) && (SMP_CPU_CNT > 1)
    /* For SMP, each core still needs its own interrupt handling setup */
    lui t1, %hi(__STACK_SIZE)
    addi t1, t1, %lo(__STACK_SIZE)
    la t0, _sp
    csrr a0, CSR_MHARTID
    andi a0, a0, 0xFF
    li a1, 0
1:
    beq a0, a1, 2f
    sub t0, t0, t1
    addi a1, a1, 1
    j 1b
2:
    /* Note: mscratch not used in simplified design */
#endif

#if ( configNUMBER_OF_CORES == 1 )
    LOAD t0, pxCurrentTCB
#else
    la t0, pxCurrentTCBs
    csrr t1, CSR_MHARTID
    slli t1, t1, LOG_REGBYTES
    add t0, t0, t1
    LOAD t0, 0(t0)
#endif
    LOAD sp, 0x0(t0)

    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0
    LOAD t0,  (portRegNum - 1)  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    LOAD x1,  1  * REGBYTES(sp)
    LOAD x5,  2  * REGBYTES(sp)
    LOAD x6,  3  * REGBYTES(sp)
    LOAD x7,  4  * REGBYTES(sp)
    LOAD x8,  5  * REGBYTES(sp)
    LOAD x9,  6  * REGBYTES(sp)
    LOAD x10, 7  * REGBYTES(sp)
    LOAD x11, 8  * REGBYTES(sp)
    LOAD x12, 9  * REGBYTES(sp)
    LOAD x13, 10 * REGBYTES(sp)
    LOAD x14, 11 * REGBYTES(sp)
    LOAD x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 13 * REGBYTES(sp)
    LOAD x17, 14 * REGBYTES(sp)
    LOAD x18, 15 * REGBYTES(sp)
    LOAD x19, 16 * REGBYTES(sp)
    LOAD x20, 17 * REGBYTES(sp)
    LOAD x21, 18 * REGBYTES(sp)
    LOAD x22, 19 * REGBYTES(sp)
    LOAD x23, 20 * REGBYTES(sp)
    LOAD x24, 21 * REGBYTES(sp)
    LOAD x25, 22 * REGBYTES(sp)
    LOAD x26, 23 * REGBYTES(sp)
    LOAD x27, 24 * REGBYTES(sp)
    LOAD x28, 25 * REGBYTES(sp)
    LOAD x29, 26 * REGBYTES(sp)
    LOAD x30, 27 * REGBYTES(sp)
    LOAD x31, 28 * REGBYTES(sp)
#endif
    addi sp, sp, portCONTEXT_SIZE
    mret
    .size prvPortStartFirstTask, . - prvPortStartFirstTask

.align 2
.global eclic_msip_handler
.type eclic_msip_handler, @function
eclic_msip_handler:
    addi sp, sp, -portCONTEXT_SIZE
    STORE x1,  1  * REGBYTES(sp)    /* RA */
    STORE x5,  2  * REGBYTES(sp)
    STORE x6,  3  * REGBYTES(sp)
    STORE x7,  4  * REGBYTES(sp)
    STORE x8,  5  * REGBYTES(sp)
    STORE x9,  6  * REGBYTES(sp)
    STORE x10, 7  * REGBYTES(sp)
    STORE x11, 8  * REGBYTES(sp)
    STORE x12, 9  * REGBYTES(sp)
    STORE x13, 10 * REGBYTES(sp)
    STORE x14, 11 * REGBYTES(sp)
    STORE x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    STORE x16, 13 * REGBYTES(sp)
    STORE x17, 14 * REGBYTES(sp)
    STORE x18, 15 * REGBYTES(sp)
    STORE x19, 16 * REGBYTES(sp)
    STORE x20, 17 * REGBYTES(sp)
    STORE x21, 18 * REGBYTES(sp)
    STORE x22, 19 * REGBYTES(sp)
    STORE x23, 20 * REGBYTES(sp)
    STORE x24, 21 * REGBYTES(sp)
    STORE x25, 22 * REGBYTES(sp)
    STORE x26, 23 * REGBYTES(sp)
    STORE x27, 24 * REGBYTES(sp)
    STORE x28, 25 * REGBYTES(sp)
    STORE x29, 26 * REGBYTES(sp)
    STORE x30, 27 * REGBYTES(sp)
    STORE x31, 28 * REGBYTES(sp)
#endif
    csrr t0, CSR_MSTATUS
    STORE t0,  (portRegNum - 1)  * REGBYTES(sp)
#if ( configNUMBER_OF_CORES == 1 )
    LOAD t0, pxCurrentTCB           /* Load pxCurrentTCB. */
#else
    la t0, pxCurrentTCBs            /* Load pxCurrentTCBs[core] */
    csrr t1, CSR_MHARTID
    slli t1, t1, LOG_REGBYTES
    add t0, t0, t1
    LOAD t0, 0(t0)
#endif
    STORE sp, 0(t0)

    csrr t0, CSR_MEPC
    STORE t0, 0(sp)
    jal xPortTaskSwitch
#if ( configNUMBER_OF_CORES == 1 )
    LOAD t0, pxCurrentTCB           /* Load pxCurrentTCB. */
#else
    la t0, pxCurrentTCBs            /* Load pxCurrentTCBs[core] */
    csrr t1, CSR_MHARTID
    slli t1, t1, LOG_REGBYTES
    add t0, t0, t1
    LOAD t0, 0(t0)
#endif
    LOAD sp, 0x0(t0)                /* Read sp from first TCB member */

    LOAD t0,  0  * REGBYTES(sp)
    csrw CSR_MEPC, t0
    LOAD t0,  (portRegNum - 1)  * REGBYTES(sp)
    csrw CSR_MSTATUS, t0
    LOAD x1,  1  * REGBYTES(sp)    /* RA */
    LOAD x5,  2  * REGBYTES(sp)
    LOAD x6,  3  * REGBYTES(sp)
    LOAD x7,  4  * REGBYTES(sp)
    LOAD x8,  5  * REGBYTES(sp)
    LOAD x9,  6  * REGBYTES(sp)
    LOAD x10, 7  * REGBYTES(sp)
    LOAD x11, 8  * REGBYTES(sp)
    LOAD x12, 9  * REGBYTES(sp)
    LOAD x13, 10 * REGBYTES(sp)
    LOAD x14, 11 * REGBYTES(sp)
    LOAD x15, 12 * REGBYTES(sp)
#ifndef __riscv_32e
    LOAD x16, 13 * REGBYTES(sp)
    LOAD x17, 14 * REGBYTES(sp)
    LOAD x18, 15 * REGBYTES(sp)
    LOAD x19, 16 * REGBYTES(sp)
    LOAD x20, 17 * REGBYTES(sp)
    LOAD x21, 18 * REGBYTES(sp)
    LOAD x22, 19 * REGBYTES(sp)
    LOAD x23, 20 * REGBYTES(sp)
    LOAD x24, 21 * REGBYTES(sp)
    LOAD x25, 22 * REGBYTES(sp)
    LOAD x26, 23 * REGBYTES(sp)
    LOAD x27, 24 * REGBYTES(sp)
    LOAD x28, 25 * REGBYTES(sp)
    LOAD x29, 26 * REGBYTES(sp)
    LOAD x30, 27 * REGBYTES(sp)
    LOAD x31, 28 * REGBYTES(sp)
#endif

    addi sp, sp, portCONTEXT_SIZE
    mret

    .size eclic_msip_handler, . - eclic_msip_handler


