#include "Drivers/i2c/i2c.h"
#include "ms1/register.h"
#include "soc.h"

// I2C callback storage
static struct {
  i2c_rx_callback_t rx_callback;
  i2c_tx_callback_t tx_callback;
  i2c_error_callback_t error_callback;
} i2c_callbacks[2] = {{NULL, NULL, NULL}, {NULL, NULL, NULL}};

// Get I2C instance index from I2C_TypeDef pointer
static int i2c_get_instance_index(I2C_TypeDef *i2c) {
  if (i2c == I2C_DBG0) {
    return 0;
  } else if (i2c == I2C_DBG1) {
    return 1;
  }
  return -1;
}

// Initialize I2C interrupt system
int i2c_irq_init(void) {
  // Initialize callback arrays
  for (int i = 0; i < 2; i++) {
    i2c_callbacks[i].rx_callback = NULL;
    i2c_callbacks[i].tx_callback = NULL;
    i2c_callbacks[i].error_callback = NULL;
  }

  // Enable I2C interrupts in the system (implementation depends on your
  // interrupt controller) This would be similar to how UART interrupts are
  // enabled

  return 0;
}

// Register I2C callbacks
void i2c_register_callbacks(I2C_TypeDef *i2c, i2c_rx_callback_t rx_cb,
                            i2c_tx_callback_t tx_cb,
                            i2c_error_callback_t err_cb) {
  int index = i2c_get_instance_index(i2c);
  if (index >= 0 && index < 2) {
    i2c_callbacks[index].rx_callback = rx_cb;
    i2c_callbacks[index].tx_callback = tx_cb;
    i2c_callbacks[index].error_callback = err_cb;
  }
}

// Unregister I2C callbacks
void i2c_unregister_callbacks(I2C_TypeDef *i2c) {
  int index = i2c_get_instance_index(i2c);
  if (index >= 0 && index < 2) {
    i2c_callbacks[index].rx_callback = NULL;
    i2c_callbacks[index].tx_callback = NULL;
    i2c_callbacks[index].error_callback = NULL;
  }
}

// Common I2C interrupt handler
static void i2c_handle_interrupt(I2C_TypeDef *i2c, int instance_index) {
  I2C_TypeDef regs =
      (i2c == I2C_DBG0) ? i2c_get_real_regs_dbg0() : i2c_get_real_regs_dbg1();
  uint32_t irq_status = *regs.IRQ_STS;
  uint32_t irq_enable = *regs.IRQ_EN;

  // Only process enabled interrupts
  irq_status &= irq_enable;

  if (irq_status == 0) {
    return; // No enabled interrupts pending
  }

  // Handle PHY error (NACK, etc.)
  if (irq_status & I2C_IRQ_PHY_ERR) {
    if (i2c_callbacks[instance_index].error_callback) {
      i2c_callbacks[instance_index].error_callback(irq_status);
    }
    // Clear the error interrupt
    *regs.IRQ_STS = I2C_IRQ_PHY_ERR;
  }

  // Handle RX FIFO interrupt
  if (irq_status & I2C_IRQ_RX_FIFO_IRQ) {
    if (i2c_callbacks[instance_index].rx_callback) {
      // Read available data from RX FIFO
      while (!(*regs.RX_FIFO & I2C_RX_FIFO_RX_EMPTY)) {
        uint8_t data = (*regs.RX_FIFO & I2C_RX_FIFO_RX_DATA_MASK) >>
                       I2C_RX_FIFO_RX_DATA_POS;
        i2c_callbacks[instance_index].rx_callback(data);
      }
    }
    // Clear the RX FIFO interrupt
    *regs.IRQ_STS = I2C_IRQ_RX_FIFO_IRQ;
  }

  // Handle TX FIFO interrupt
  if (irq_status & I2C_IRQ_TX_FIFO_IRQ) {
    if (i2c_callbacks[instance_index].tx_callback) {
      i2c_callbacks[instance_index].tx_callback();
    }
    // Clear the TX FIFO interrupt
    *regs.IRQ_STS = I2C_IRQ_TX_FIFO_IRQ;
  }

  // Handle RX empty interrupt
  if (irq_status & I2C_IRQ_RX_EMPTY) {
    // Clear the interrupt
    *regs.IRQ_STS = I2C_IRQ_RX_EMPTY;
  }

  // Handle RX full interrupt
  if (irq_status & I2C_IRQ_RX_FULL) {
    if (i2c_callbacks[instance_index].rx_callback) {
      // Read data from RX FIFO
      while (!(*regs.RX_FIFO & I2C_RX_FIFO_RX_EMPTY)) {
        uint8_t data = (*regs.RX_FIFO & I2C_RX_FIFO_RX_DATA_MASK) >>
                       I2C_RX_FIFO_RX_DATA_POS;
        i2c_callbacks[instance_index].rx_callback(data);
      }
    }
    // Clear the interrupt
    *regs.IRQ_STS = I2C_IRQ_RX_FULL;
  }

  // Handle TX empty interrupt
  if (irq_status & I2C_IRQ_TX_EMPTY) {
    if (i2c_callbacks[instance_index].tx_callback) {
      i2c_callbacks[instance_index].tx_callback();
    }
    // Clear the interrupt
    *regs.IRQ_STS = I2C_IRQ_TX_EMPTY;
  }

  // Handle TX full interrupt
  if (irq_status & I2C_IRQ_TX_FULL) {
    // Clear the interrupt
    *regs.IRQ_STS = I2C_IRQ_TX_FULL;
  }

  // Handle PHY done interrupt
  if (irq_status & I2C_IRQ_PHY_DONE) {
    // Clear the interrupt
    *regs.IRQ_STS = I2C_IRQ_PHY_DONE;
  }

  // Handle task done interrupt (DMA)
  if (irq_status & I2C_IRQ_TASK_DONE) {
    // Clear the interrupt
    *regs.IRQ_STS = I2C_IRQ_TASK_DONE;
  }

  // Handle all done interrupt
  if (irq_status & I2C_IRQ_ALL_DONE) {
    // Clear the interrupt
    *regs.IRQ_STS = I2C_IRQ_ALL_DONE;
  }
}

// I2C Debug 0 interrupt handler
void eclic_i2c_dbg0_int_handler(void) { i2c_handle_interrupt(I2C_DBG0, 0); }

// I2C Debug 1 interrupt handler
void eclic_i2c_dbg1_int_handler(void) { i2c_handle_interrupt(I2C_DBG1, 1); }
