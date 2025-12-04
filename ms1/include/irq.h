/**
 * @file irq.h
 * @brief Interrupt configuration and handler declarations for MS1 SoC
 *
 * This header file contains declarations for interrupt initialization functions
 * and interrupt service routines (ISRs) used in the MS1 SoC.
 */

#ifndef __IRQ_H__
#define __IRQ_H__

// #include <stdint.h>
#include "soc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========== SOC IRQ Controller Register Definitions ========== */

/**
 * SOC IRQ Controller (IRQ_CTRL) Module Overview:
 * 
 * The IRQ_CTRL module performs the following functions:
 * (A) Collect all IRQ sources together and synchronize them
 * (B) Add invert or latch functionality if needed  
 * (C) Send to different destinations after masking
 *
 * Key Features:
 * - Supports 96 interrupt sources (0-95) across 3 register banks
 * - Each interrupt can be configured as original source or latched
 * - Individual masking, inversion, and software trigger capabilities
 */

/**
 * IRQ Mode Control Registers (IRQCTRL_IRQ_MODE0~2):
 * - IRQCTRL_IRQ_MODE0: IRQ[31:0] mode control
 * - IRQCTRL_IRQ_MODE1: IRQ[63:32] mode control  
 * - IRQCTRL_IRQ_MODE2: IRQ[95:64] mode control
 * 
 * Register Fields - Bits[95:0] (R/W, Default: 0):
 * - 0: Select original IRQ source (for edge-triggered interrupts)
 * - 1: Select latched IRQ (for level-triggered interrupts)
 */

/**
 * IRQ Latch Status Registers (IRQCTRL_IRQ_LATCH0~2) - Read-Only:
 * - IRQCTRL_IRQ_LATCH0: IRQ[31:0] latch status
 * - IRQCTRL_IRQ_LATCH1: IRQ[63:32] latch status
 * - IRQCTRL_IRQ_LATCH2: IRQ[95:64] latch status
 *
 * Register Fields - Bits[95:0] (R, Default: 0):
 * - Shows the current latched IRQ value
 * - Only meaningful when corresponding MODE bit is set to 1
 */

/**
 * IRQ Latch Clear Control Registers (IRQCTRL_IRQ_LATCH_CLR0~2):
 * - IRQCTRL_IRQ_LATCH_CLR0: IRQ[31:0] latch clear
 * - IRQCTRL_IRQ_LATCH_CLR1: IRQ[63:32] latch clear
 * - IRQCTRL_IRQ_LATCH_CLR2: IRQ[95:64] latch clear
 *
 * Register Fields - Bits[95:0] (R/W, Default: 0):
 * - 0: Normal operation
 * - 1: Keep latched IRQ low (clear the latch)
 * - Write 1 to clear, automatically returns to 0
 */

/**
 * IRQ Mask Control Registers for AOCPU (IRQCTRL_IRQ_MASK0~2_AOCPU):
 * - IRQCTRL_IRQ_MASK0_AOCPU: IRQ[31:0] mask for AOCPU
 * - IRQCTRL_IRQ_MASK1_AOCPU: IRQ[63:32] mask for AOCPU
 * - IRQCTRL_IRQ_MASK2_AOCPU: IRQ[95:64] mask for AOCPU
 *
 * Register Fields - Bits[95:0] (R/W, Default: all 1):
 * - 0: Keep IRQ as low (masked)
 * - 1: Allow IRQ to pass through (unmasked)
 */

/**
 * IRQ Invert Control Registers (IRQCTRL_IRQ_INV0~2):
 * - IRQCTRL_IRQ_INV0: IRQ[31:0] invert control
 * - IRQCTRL_IRQ_INV1: IRQ[63:32] invert control
 * - IRQCTRL_IRQ_INV2: IRQ[95:64] invert control
 *
 * Register Fields - Bits[95:0] (R/W, Default: 0):
 * - 0: Normal polarity
 * - 1: Invert IRQ level (active low becomes active high)
 */

/**
 * Software IRQ Control Register (IRQCTRL_SOFT_IRQ):
 * Register Fields:
 * - Bits[31:8] (R/W, Default: 0x0) - Reserved
 * - Bits[7:0] (R/W, Default: 0x0) - IRQ can be set by software
 */

/* Note: Register definitions are in ms1/register.h */

// UART interrupt handling is now in Drivers/uart/uart_irq.h

/**
 * @brief Initialize and configure interrupts according to vector table
 *
 * Configures ECLIC interrupts based on the vector table defined in
 * startup_from_flash.S Sets proper interrupt levels, priorities, and handlers
 * for:
 * - Machine Software Interrupt (FreeRTOS inter-task communication)
 * - Machine Timer Interrupt (FreeRTOS task scheduling)
 * - Inter-core Interrupt (multi-core communication)
 * - UART0 Interrupt (serial communication)
 *
 * @return 0 on success, -1 on failure
 */
int irq_init(void);

// UART interrupt handlers are now declared in Drivers/uart/uart_irq.h

/**
 * @brief Inter-core Interrupt Handler
 *
 * Handles interrupts between CPU cores for multi-core systems.
 * Used for inter-processor communication and synchronization.
 */
void eclic_inter_core_int_handler(void);

/**
 * @brief Helper function for assembly code to clear ECLIC interrupts
 * @param irq_num IRQ number to clear
 * 
 * This function provides a callable wrapper for clearing ECLIC pending 
 * interrupts from assembly code, which cannot call inline functions.
 * Critical for machine-mode only CPUs that don't auto-clear interrupts.
 */
void clear_eclic_interrupt(IRQn_Type irq_num);

/**
 * @brief Clear SOC IRQ module latch for specified interrupt
 * @param irq_num Interrupt number to clear
 * @note Required for pulse interrupts to re-enable edge detection
 */
void soc_clear_irq_latch(uint32_t irq_num);

/**
 * @brief Register interrupt with automatic SOC IRQ module configuration
 * @param irq_num      Interrupt number (IRQn_Type)
 * @param vector_mode  Vector mode (ECLIC_VECTOR_INTERRUPT/ECLIC_NON_VECTOR_INTERRUPT)
 * @param trigger_type Trigger type (ECLIC_POSTIVE_EDGE_TRIGGER, etc.)
 * @param level        Interrupt level (0-255)
 * @param priority     Interrupt priority (0-7 for 3-bit intctlbits)
 * @param handler      Interrupt handler function pointer (NULL for default handler)
 * @return 0 on success, -1 on failure
 */
int soc_register_interrupt(IRQn_Type irq_num, 
                          uint8_t vector_mode,
                          uint8_t trigger_type, 
                          uint8_t level,
                          uint8_t priority, 
                          void (*handler)(void));

/**
 * @brief Default Interrupt/Exception Handler
 *
 * Fallback handler for any unhandled interrupts or exceptions.
 * Provides debugging information and halts the system safely.
 */
void default_intexc_vector_handler(void);

// All UART interrupt callback functions are now in Drivers/uart/uart_irq.h

#ifdef __cplusplus
}
#endif

#endif /* __IRQ_H__ */
