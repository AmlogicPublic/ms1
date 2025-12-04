/**
 * @file irq.c
 * @brief Interrupt handlers for MS1 SoC
 *
 * This file contains interrupt service routines (ISRs) that override the weak
 * assembly implementations in startup_from_flash.S and FreeRTOS portasm.S

 * vector base: soc\ms1\source\GCC\startup_from_*.S

 * exc_entry: kernel\freertos\portable\GCC\RISC-V\portasm.S
 * irq_entry: kernel\freertos\portable\GCC\RISC-V\portasm.S

 * core_intexc_handler: soc\ms1\include\system_soc.h

 * Interrupt Handlers (override weak assembly symbols):
 * - RTOS API: eclic_msip_handler: Machine Software Interrupt
               higher priority than HW timer interrupt
               (for freeRTOS inter-task communication and context switching)
               (defined in \kernel\freertos\portable\GCC\RISC-V\portasm.S)
 * - RTOS API: eclic_mtip_handler: Machine Timer Interrupt
               lowest priority of all
               (vector mode, triggered when no other pending)
               (defined in D:\ms1\ms1\kernel\freertos\portable\port.c)
 * - eclic_uart_a_int_handler: UART0 Interrupt
 * - eclic_inter_core_int_handler: Inter-core Interrupt
 * - default_intexc_vector_handler: Default exception handler
 */

#include "Drivers/uart/uart.h"
#include "ms1/register.h" /* For IRQCTRL register definitions */
#include "soc.h"

/* Include NMSIS core header for CSR operations */
#include <nmsis_core.h>
#include <stdio.h>

/* ========== Function Declarations ========== */
extern void eclic_inter_core_int_handler(void);

/**
 * @brief Write to SOC IRQ module register
 * @param addr  Register address
 * @param value Value to write
 */
static inline void soc_irq_write_reg(uint32_t addr, uint32_t value) {
  *((volatile uint32_t *)addr) = value;
}

/**
 * @brief Initialize SOC IRQ module with all interrupts using original sources
 * @note MODE=0 selects original IRQ source (for edge-triggered interrupts)
 *       MODE=1 selects latched IRQ (for level-triggered interrupts)
 */
static void soc_irq_mode_init(void) {
  /* Initialize all MODE bits to 0 (use original IRQ sources, not latched) */
  soc_irq_write_reg(IRQCTRL_IRQ_MODE0, 0x00000000U);
  soc_irq_write_reg(IRQCTRL_IRQ_MODE1, 0x00000000U);
  soc_irq_write_reg(IRQCTRL_IRQ_MODE2, 0x00000000U);
}

/**
 * @brief Clear SOC IRQ module latch for specified interrupt
 * @param irq_num Interrupt number to clear
 * @note Required for pulse interrupts to re-enable edge detection
 */
void soc_clear_irq_latch(uint32_t irq_num) {
  uint32_t reg_offset = 4 * (irq_num / 32);
  uint32_t bit_mask = 1U << (irq_num % 32);
  soc_irq_write_reg(IRQCTRL_IRQ_LATCH_CLR0 + reg_offset, bit_mask);
}

/* ========== Public API Functions ========== */

/**
 * @brief Register interrupt with automatic SOC IRQ module configuration
 * @param irq_num      Interrupt number (IRQn_Type)
 * @param vector_mode  Vector mode
 * (ECLIC_VECTOR_INTERRUPT/ECLIC_NON_VECTOR_INTERRUPT)
 * @param trigger_type Trigger type (ECLIC_POSTIVE_EDGE_TRIGGER, etc.)
 * @param level        Interrupt level (0-255)
 * @param priority     Interrupt priority (0-7 for 3-bit intctlbits)
 * @param handler      Interrupt handler function pointer (NULL for default
 * handler)
 * @return 0 on success, -1 on failure
 */
int soc_register_interrupt(IRQn_Type irq_num, uint8_t vector_mode,
                           uint8_t trigger_type, uint8_t level,
                           uint8_t priority, void (*handler)(void)) {
  int32_t ret;

  /* Configure SOC IRQ module based on trigger type */
  uint32_t mode_reg_idx = irq_num / 32;
  uint32_t mode_bit_idx = irq_num % 32;

  if (mode_reg_idx < 3) { /* We have MODE0, MODE1, MODE2 registers */
    uint32_t mode_reg_addr = IRQCTRL_IRQ_MODE0 + (mode_reg_idx * 4);
    uint32_t current_mode = *((volatile uint32_t *)mode_reg_addr);
    uint32_t bit_mask = 1U << mode_bit_idx;

    if (trigger_type == ECLIC_LEVEL_TRIGGER) {
      /* Level triggered: use latched IRQ (set bit to 1) */
      soc_irq_write_reg(mode_reg_addr, current_mode | bit_mask);
      /* Clear any existing latch state */
      soc_clear_irq_latch(irq_num);
    } else {
      /* Edge triggered: use original IRQ source (set bit to 0) */
      soc_irq_write_reg(mode_reg_addr, current_mode & ~bit_mask);
      /* No need to clear latch for edge-triggered interrupts */
    }
  }

  /* Register with ECLIC */
  ret = ECLIC_Register_IRQ(irq_num, vector_mode, trigger_type, level, priority,
                           handler);
  if (ret != 0) {
    return -1;
  }

  return 0;
}

/**
 * @brief Clear interrupt pending status at ECLIC level
 * @param irq_num Interrupt number to clear
 * @note Called from assembly interrupt handler. Critical for machine-mode-only
 * CPU. SOC IRQ latch clearing is only needed for level-triggered interrupts.
 */
void clear_eclic_interrupt(IRQn_Type irq_num) {
  /* Clear ECLIC pending bit (always required) */
  extern volatile unsigned long CpuIRegionBase;
  volatile uint8_t *intip_reg = (volatile uint8_t *)(CpuIRegionBase + 0x21000 + irq_num * 4);

  /* Use misc/ucode's working approach: write 0 directly to clear all internal state */
  *intip_reg = 0x00;

  /* Also clear SOC IRQ latch to ensure complete clearing for edge-triggered interrupts */
  soc_clear_irq_latch(irq_num);

  /* Additional: ensure write is completed before returning */
  __asm__ volatile("fence" ::: "memory");
}

/* ========== Initialization Functions ========== */

/**
 * @brief Initialize interrupt system
 * @return 0 on success, -1 on failure
 */
int irq_init(void) {
  int32_t ret;

  /* Initialize SOC IRQ module for pulse interrupt handling */
  soc_irq_mode_init();

  /* Enable machine external (peripheral) interrupts in MIE CSR */
  __RV_CSR_SET(CSR_MIE, MIE_MEIE);

  /* Enable global interrupts (MSTATUS.MIE) */
  __RV_CSR_SET(CSR_MSTATUS, MSTATUS_MIE);

  /* Register system interrupts */
  ret = soc_register_interrupt(SysTimerSW_IRQn, ECLIC_VECTOR_INTERRUPT,
                               ECLIC_POSTIVE_EDGE_TRIGGER, 192, 2, NULL);
  if (ret != 0)
    return -1;

  ret = soc_register_interrupt(SysTimer_IRQn, ECLIC_VECTOR_INTERRUPT,
                               ECLIC_POSTIVE_EDGE_TRIGGER, 192, 7, NULL);
  if (ret != 0)
    return -1;

  ret = soc_register_interrupt(InterCore_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                               ECLIC_POSTIVE_EDGE_TRIGGER, 192, 2,
                               eclic_inter_core_int_handler);
  if (ret != 0)
    return -1;

  /* Initialize other peripherals interrupts */
  //ret = uart_irq_init();
  if (ret != 0)
    return -1;

  return 0;
}

/* ========== Interrupt Handlers ========== */

/**
 * @brief Inter-core interrupt handler for Non-Vector Mode
 * @note In Non-Vector mode, the common irq_entry handles context saving.
 * Handler functions are regular C functions called by csrrw ra, CSR_JALMNXTI, ra.
 * NO __attribute__((interrupt)) needed - that causes double context saving!
 */
void eclic_inter_core_int_handler(void) {
  /* Handle inter-core communication here */
  /* Implementation depends on your specific inter-core communication mechanism
   */
}

/**
 * @brief Default handler for unhandled interrupts/exceptions in Vector mode
 * @note This function handles any interrupt or exception that doesn't have a
 * specific handler. It must have __attribute__((interrupt)) for Vector mode.
 * According to Nuclei standard, Vector mode handlers:
 * 1. Must have __attribute__((interrupt))
 * 2. Hardware doesn't save context automatically
 * 3. Compiler generates proper prologue/epilogue with mret
 */
void __attribute__((interrupt)) default_intexc_vector_handler(void) {
  /* Get the cause of the interrupt/exception */
  unsigned long mcause = __RV_CSR_READ(CSR_MCAUSE);
  unsigned long mepc = __RV_CSR_READ(CSR_MEPC);
  unsigned long mtval = __RV_CSR_READ(CSR_MTVAL);

  /* Basic error reporting */
  printf("Unhandled interrupt/exception: MCAUSE=0x%lx, MEPC=0x%lx, MTVAL=0x%lx\n", mcause, mepc, mtval);

  // 0 Instruction address misaligned
  // 1 Instruction access fault
  // 2 Illegal instruction
  // 3 Breakpoint
  // 4 Load address misaligned
  // 5 Load access fault
  // 6 Store/AMO address misaligned
  // 7 Store/AMO access fault
  // 8 Environment call from U-mode (ECALL from U)
  // 9 Environment call from S-mode (ECALL from S)
  // 11 Environment call from M-mode (ECALL from M)
  // 12 Instruction page fault
  // 13 Load page fault
  // 15 Store/AMO page fault

  /* For Vector mode, we should handle the interrupt and return properly */
  /* If this is an interrupt, clear it and return */
  if (mcause & 0x80000000UL) {
    /* This is an interrupt - for debugging, just return */
    /* In a real implementation, you might want to clear specific interrupts */
    return; /* Compiler will generate mret due to __attribute__((interrupt)) */
  } else {
    /* This is an exception - halt the system */
    while (1) {
      __asm__ volatile("wfi"); /* Wait for interrupt */
    }
  }
}
