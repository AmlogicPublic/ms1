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

#include "Drivers/vpu/vpu_irq.h"
#include "irq.h" // For soc_register_interrupt
#include "ms1/register.h"
#include "soc.h"

/* Include NMSIS core header for CSR operations */
#include <nmsis_core.h>
#include <stdio.h>

// UART interrupt callbacks (static - internal to this module)
//static uart_rx_callback_t uart_a_rx_callback = NULL;
//static uart_tx_callback_t uart_a_tx_callback = NULL;
//static uart_rx_callback_t uart_b_rx_callback = NULL;
//static uart_tx_callback_t uart_b_tx_callback = NULL;

/**
 * @brief Initialize UART interrupts
 * @return 0 on success, -1 on failure
 */
int vpu_irq_init(void) {
  int32_t ret;

  /* Register UART interrupts using clean API (automatically handles SOC IRQ
   * module) */
// ret = soc_register_interrupt(UART0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
//                              ECLIC_POSTIVE_EDGE_TRIGGER, 192, 1,
//                              eclic_uart_a_int_handler);
// if (ret != 0)
//   return -1;

 ret = soc_register_interrupt(DISP0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                              ECLIC_POSTIVE_EDGE_TRIGGER, 192, 1,
                              eclic_disp0_int_handler);
 if (ret != 0)
   return -1;

  return 0; /* Success */
}

uint32_t frm_num;
//uint32_t frm_num_div100;
void eclic_disp0_int_handler(void){
	printf("Vsync...\r\n");
//	frm_num_div100 = frm_num/100;
	
//	if(frm_num%100 == 0){
		if(frm_num%2 == 0) Wr(0xfe4048cc,0x00f00438);
		else Wr(0xfe4048cc,0x00a00438);
//	}
	frm_num++;
}
	


