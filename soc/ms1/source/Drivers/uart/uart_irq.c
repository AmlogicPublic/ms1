/**
 * @file uart_irq.c
 * @brief UART interrupt implementation for MS1 SoC real hardware
 *
 * UART INTERRUPT PHILOSOPHY:
 *
 * This driver implements efficient interrupt-driven UART I/O using FIFO
 * thresholds:
 *
 * RX INTERRUPTS (Receive):
 * - Hardware triggers when: RX FIFO count > RX threshold (default: xx bytes)
 * - Purpose: Batch processing - notify when sufficient data accumulates
 * - Benefit: Reduces interrupt overhead, enables efficient data processing
 * - Handler: Reads all available data (may exceed threshold)
 *
 * TX INTERRUPTS (Transmit):
 * - Hardware triggers when: TX FIFO count < TX threshold (default: xx bytes)
 * - Purpose: Flow control - notify when FIFO has space for more data
 * - Benefit: Prevents blocking writes, enables continuous data streaming
 * - Handler: Notifies user to queue more data
 *
 * THRESHOLD SETTINGS (configurable via uart_set_watermarks):
 * - RX threshold (xx): Interrupt when ≥xx bytes received (efficient batching)
 * - TX threshold (xx): Interrupt when <xx bytes in FIFO (good flow control)
 *
 * This file contains interrupt handlers and callback management for UART_A and
 * UART_B.
 */

#include "Drivers/uart/uart.h"
#include "irq.h" // For soc_register_interrupt
#include "ms1/register.h"
#include "soc.h"

/* Include NMSIS core header for CSR operations */
#include <nmsis_core.h>
#include <stdio.h>

// UART interrupt callbacks (static - internal to this module)
static uart_rx_callback_t uart_a_rx_callback = NULL;
static uart_tx_callback_t uart_a_tx_callback = NULL;
static uart_rx_callback_t uart_b_rx_callback = NULL;
static uart_tx_callback_t uart_b_tx_callback = NULL;

/**
 * @brief Initialize UART interrupts
 * @return 0 on success, -1 on failure
 */
int uart_irq_init(void) {
  int32_t ret;

  /* Register UART interrupts using clean API (automatically handles SOC IRQ
   * module) */
  ret = soc_register_interrupt(UART0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                               ECLIC_POSTIVE_EDGE_TRIGGER, 192, 1,
                               eclic_uart_a_int_handler);
  if (ret != 0)
    return -1;

  ret = soc_register_interrupt(UART1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                               ECLIC_POSTIVE_EDGE_TRIGGER, 192, 1,
                               eclic_uart_b_int_handler);
  if (ret != 0)
    return -1;

  return 0; /* Success */
}

// UART interrupt callback registration functions

/**
 * @brief Register UART interrupt callbacks
 * @param uart UART instance (UART0 or UART1)
 * @param rx_cb RX callback function (can be NULL)
 * @param tx_cb TX callback function (can be NULL)
 */
void uart_register_callbacks(UART_TypeDef *uart, uart_rx_callback_t rx_cb,
                             uart_tx_callback_t tx_cb) {
  if (uart == UART0) {
    uart_a_rx_callback = rx_cb;
    uart_a_tx_callback = tx_cb;
  } else if (uart == UART1) {
    uart_b_rx_callback = rx_cb;
    uart_b_tx_callback = tx_cb;
  }
}

/**
 * @brief Unregister UART callbacks
 * @param uart UART instance (UART0 or UART1)
 */
void uart_unregister_callbacks(UART_TypeDef *uart) {
  if (uart == UART0) {
    uart_a_rx_callback = NULL;
    uart_a_tx_callback = NULL;
  } else if (uart == UART1) {
    uart_b_rx_callback = NULL;
    uart_b_tx_callback = NULL;
  }
}

/**
 * @brief UART_A Interrupt Handler for Non-Vector Mode
 *
 * CORRECT INTERRUPT BEHAVIOR:
 * - RX Interrupt: Hardware triggers when RX FIFO count > RX threshold (data
 * available to read) Purpose: Notify when sufficient data has arrived for
 * efficient batch processing
 * - TX Interrupt: Hardware triggers when TX FIFO count < TX threshold (space
 * available to write) Purpose: Notify when FIFO has space to accept more data
 * without blocking
 *
 * This handler processes the interrupt conditions and calls user callbacks.
 *
 * @note In Non-Vector mode, the common irq_entry handles context saving.
 * Handler functions are regular C functions called by csrrw ra, CSR_JALMNXTI, ra.
 * NO __attribute__((interrupt)) needed - that causes double context saving!
 */
void eclic_uart_a_int_handler(void) {

  // UART_A register addresses
  uint32_t status_reg = UART_A_STATUS;
  uint32_t control_reg = UART_A_CONTROL;
  uint32_t rfifo_reg = UART_A_RFIFO;
  uint32_t wfifo_reg = UART_A_WFIFO;

  uint32_t status = Rd(status_reg);
  uint32_t control = Rd(control_reg);

  Wr(AO_PADCTRL_PIN_MUX_REG3, 0);
  //printf("\r\n========================== IRQ(UART_A):Start ==========================\r\n");

  // Handle RX interrupt
  // Hardware triggers this when: RX FIFO count > RX threshold (typically 44
  // bytes) Meaning: Sufficient data has accumulated for efficient processing
  if ((control & UART_CTRL_RX_INT_EN) && !(status & UART_STAT_RX_EMPTY)) {
    // Process all available received data (may be more than threshold)
    while (!(status & UART_STAT_RX_EMPTY)) {
      uint8_t received_char = Rd(rfifo_reg) & 0xFF;

      // Call user callback if registered
      if (uart_a_rx_callback) {
        uart_a_rx_callback(received_char);
      }

      // Update status for next iteration
      status = Rd(status_reg);
    }
  }

  // Handle TX interrupt
  // Hardware triggers this when: TX FIFO count < TX threshold (typically 10
  // bytes) Meaning: FIFO has space available, safe to write more data without
  // blocking
  if ((control & UART_CTRL_TX_INT_EN) && !(status & UART_STAT_TX_FULL)) {
    // Notify user that TX FIFO has space for more data
    if (uart_a_tx_callback) {
      uart_a_tx_callback();
    }
  }

  // Handle error conditions
  if (status &
      (UART_STAT_RX_OVERFLOW | UART_STAT_FRAME_ERR | UART_STAT_PARITY_ERR)) {
    // Clear errors by pulsing clear error bit
    Wr(control_reg, Rd(control_reg) | UART_CTRL_CLR_ERR);
    Wr(control_reg, Rd(control_reg) & ~UART_CTRL_CLR_ERR);

    printf("UART_A Error: Status=0x%08x\n", (unsigned int)status);
  }

  Wr(AO_PADCTRL_PIN_MUX_REG3, 0);
  //printf("========================== IRQ(UART_A):End   ==========================\r\n");
}

/**
 * @brief UART_B Interrupt Handler for Non-Vector Mode
 *
 * CORRECT INTERRUPT BEHAVIOR:
 * - RX Interrupt: Hardware triggers when RX FIFO count > RX threshold (data
 * available to read) Purpose: Notify when sufficient data has arrived for
 * efficient batch processing
 * - TX Interrupt: Hardware triggers when TX FIFO count < TX threshold (space
 * available to write) Purpose: Notify when FIFO has space to accept more data
 * without blocking
 *
 * This handler processes the interrupt conditions and calls user callbacks.
 *
 * @note In Non-Vector mode, the common irq_entry handles context saving.
 * Handler functions are regular C functions called by csrrw ra, CSR_JALMNXTI, ra.
 * NO __attribute__((interrupt)) needed - that causes double context saving!
 */
void eclic_uart_b_int_handler(void) {
  // UART_B register addresses
  uint32_t status_reg = UART_B_STATUS;
  uint32_t control_reg = UART_B_CONTROL;
  uint32_t rfifo_reg = UART_B_RFIFO;
  uint32_t wfifo_reg = UART_B_WFIFO;

  uint32_t status = Rd(status_reg);
  uint32_t control = Rd(control_reg);

  // Handle RX interrupt
  // Hardware triggers this when: RX FIFO count > RX threshold (typically 44
  // bytes) Meaning: Sufficient data has accumulated for efficient processing
  if ((control & UART_CTRL_RX_INT_EN) && !(status & UART_STAT_RX_EMPTY)) {
    // Process all available received data (may be more than threshold)
    while (!(status & UART_STAT_RX_EMPTY)) {
      uint8_t received_char = Rd(rfifo_reg) & 0xFF;

      // Call user callback if registered
      if (uart_b_rx_callback) {
        uart_b_rx_callback(received_char);
      }

      // Update status for next iteration
      status = Rd(status_reg);
    }
  }

  // Handle TX interrupt
  // Hardware triggers this when: TX FIFO count < TX threshold (typically 10
  // bytes) Meaning: FIFO has space available, safe to write more data without
  // blocking
  if ((control & UART_CTRL_TX_INT_EN) && !(status & UART_STAT_TX_FULL)) {
    // Notify user that TX FIFO has space for more data
    if (uart_b_tx_callback) {
      uart_b_tx_callback();
    }
  }

  // Handle error conditions
  if (status &
      (UART_STAT_RX_OVERFLOW | UART_STAT_FRAME_ERR | UART_STAT_PARITY_ERR)) {
    // Clear errors by pulsing clear error bit
    Wr(control_reg, Rd(control_reg) | UART_CTRL_CLR_ERR);
    Wr(control_reg, Rd(control_reg) & ~UART_CTRL_CLR_ERR);

    printf("UART_B Error: Status=0x%08x\n", (unsigned int)status);
  }
}
