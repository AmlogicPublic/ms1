#include "Drivers/i2c/i2c.h"
#include "ms1/register.h"
#include "soc.h"

// Get real I2C register structure for I2C_DBG0
I2C_TypeDef i2c_get_real_regs_dbg0(void) {
  I2C_TypeDef i2c_regs = {
      .RDY = (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_RDY_OFFSET),
      .CONFIG =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_CONFIG_OFFSET),
      .START = (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_START_OFFSET),
      .BUS = (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_BUS_OFFSET),
      .TX_RD_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_TX_RD_ADDR_OFFSET),
      .TX_WR_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_TX_WR_ADDR_OFFSET),
      .RX_RD_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_RX_RD_ADDR_OFFSET),
      .RX_WR_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_RX_WR_ADDR_OFFSET),
      .TX_FIFO =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_TX_FIFO_OFFSET),
      .RX_FIFO =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_RX_FIFO_OFFSET),
      .IRQ_STS =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_IRQ_STS_OFFSET),
      .IRQ_EN =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_IRQ_EN_OFFSET),
      .SHAKE =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR0 + I2C_REG_SHAKE_OFFSET)};
  return i2c_regs;
}

// Get real I2C register structure for I2C_DBG1
I2C_TypeDef i2c_get_real_regs_dbg1(void) {
  I2C_TypeDef i2c_regs = {
      .RDY = (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_RDY_OFFSET),
      .CONFIG =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_CONFIG_OFFSET),
      .START = (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_START_OFFSET),
      .BUS = (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_BUS_OFFSET),
      .TX_RD_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_TX_RD_ADDR_OFFSET),
      .TX_WR_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_TX_WR_ADDR_OFFSET),
      .RX_RD_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_RX_RD_ADDR_OFFSET),
      .RX_WR_ADDR =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_RX_WR_ADDR_OFFSET),
      .TX_FIFO =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_TX_FIFO_OFFSET),
      .RX_FIFO =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_RX_FIFO_OFFSET),
      .IRQ_STS =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_IRQ_STS_OFFSET),
      .IRQ_EN =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_IRQ_EN_OFFSET),
      .SHAKE =
          (volatile uint32_t *)(I2C_DBG_BASE_ADDR1 + I2C_REG_SHAKE_OFFSET)};
  return i2c_regs;
}

// Get real I2C registers based on fake I2C_TypeDef pointer
static I2C_TypeDef i2c_get_regs(I2C_TypeDef *i2c) {
  // Map fake I2C pointers to real hardware registers
  if (i2c == I2C_DBG0) {
    return i2c_get_real_regs_dbg0();
  } else if (i2c == I2C_DBG1) {
    return i2c_get_real_regs_dbg1();
  } else {
    // Default to I2C_DBG0 if invalid pointer
    return i2c_get_real_regs_dbg0();
  }
}

// Initialize I2C controller
int32_t i2c_init(I2C_TypeDef *i2c) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  // Step 1: Acquire I2C bus access
  i2c_acquire_bus(i2c);

  // Step 2: Configure default bus settings (167MHz / 1668 ≈ 100kHz)
  i2c_config_bus(i2c, 1668, I2C_SPEED_FAST);

  // Step 3: Set default FIFO thresholds
  i2c_set_fifo_thresholds(i2c, 32, 32);

  // Step 4: Initialize FIFO addresses (assuming 128-byte FIFO)
  i2c_fifo_init(i2c, 0, 0, 0, 0);

  // Step 5: Enable default interrupts
  uint32_t default_irq_mask = I2C_IRQ_PHY_DONE | I2C_IRQ_PHY_ERR;
  i2c_config_interrupts(i2c, default_irq_mask);

  // Step 6: Clear any pending interrupts
  i2c_clear_interrupts(i2c, 0xFFFFFFFF);

  return 0;
}

// Configure I2C bus settings
int32_t i2c_config_bus(I2C_TypeDef *i2c, uint32_t clk_ratio,
                       I2C_Speed_TypeDef speed) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  uint32_t bus_val = *regs.BUS;

  // Clear ratio and speed fields
  bus_val &= ~(I2C_BUS_B_RATIO_MASK | I2C_BUS_SPEED_MODE);

  // Set clock ratio (minimum 8 as per documentation)
  if (clk_ratio < 8) {
    clk_ratio = 8;
  }
  bus_val |= (clk_ratio & I2C_BUS_B_RATIO_MASK);

  // Set speed mode
  if (speed == I2C_SPEED_FAST) {
    bus_val |= I2C_BUS_SPEED_MODE;
  }

  *regs.BUS = bus_val;
  return 0;
}

// Configure slave mode
int32_t i2c_config_slave_mode(I2C_TypeDef *i2c, int enable) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  uint32_t bus_val = *regs.BUS;

  if (enable) {
    bus_val |= I2C_BUS_B_SLAVE_MODE;
  } else {
    bus_val &= ~I2C_BUS_B_SLAVE_MODE;
  }

  *regs.BUS = bus_val;
  return 0;
}

// Configure interrupts
int32_t i2c_config_interrupts(I2C_TypeDef *i2c, uint32_t irq_mask) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  *regs.IRQ_EN = irq_mask;
  return 0;
}

// Set FIFO thresholds
int32_t i2c_set_fifo_thresholds(I2C_TypeDef *i2c, uint32_t rx_threshold,
                                uint32_t tx_threshold) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  uint32_t config_val = *regs.CONFIG;

  // Clear threshold fields
  config_val &= ~(I2C_CFG_RX_THRESH_MASK | I2C_CFG_TX_THRESH_MASK);

  // Set new thresholds
  config_val |= ((rx_threshold & 0xFF) << I2C_CFG_RX_THRESH_POS);
  config_val |= ((tx_threshold & 0xFF) << I2C_CFG_TX_THRESH_POS);

  *regs.CONFIG = config_val;
  return 0;
}

// Initialize FIFO addresses
int32_t i2c_fifo_init(I2C_TypeDef *i2c, uint32_t tx_rd_addr,
                      uint32_t tx_wr_addr, uint32_t rx_rd_addr,
                      uint32_t rx_wr_addr) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  *regs.TX_RD_ADDR = tx_rd_addr;
  *regs.TX_WR_ADDR = tx_wr_addr;
  *regs.RX_RD_ADDR = rx_rd_addr;
  *regs.RX_WR_ADDR = rx_wr_addr;

  return 0;
}

// Write to FIFO in random access mode
int32_t i2c_fifo_write_random(I2C_TypeDef *i2c, uint32_t fifo_addr,
                              uint32_t data) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  // Set FIFO address with bit[7]=1 for random access mode
  volatile uint32_t *fifo_reg =
      (volatile uint32_t *)((uint32_t)regs.TX_FIFO +
                            (0x80 | (fifo_addr & 0x7F)));
  *fifo_reg = data;

  return 0;
}

// Write to FIFO in sequential mode
int32_t i2c_fifo_write_sequential(I2C_TypeDef *i2c, uint8_t data) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  // Wait if TX FIFO is full
  while (*regs.TX_FIFO & I2C_TX_FIFO_TX_FULL)
    ;

  // Write data, tx_wr_addr increases automatically
  *regs.TX_FIFO = (*regs.TX_FIFO & ~I2C_TX_FIFO_TX_DATA_MASK) |
                  (data << I2C_TX_FIFO_TX_DATA_POS);

  return 0;
}

// Read from FIFO in random access mode
int32_t i2c_fifo_read_random(I2C_TypeDef *i2c, uint32_t fifo_addr) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  // Set FIFO address with bit[7]=1 for random access mode
  volatile uint32_t *fifo_reg =
      (volatile uint32_t *)((uint32_t)regs.RX_FIFO +
                            (0x80 | (fifo_addr & 0x7F)));
  return *fifo_reg;
}

// Read from FIFO in sequential mode
int32_t i2c_fifo_read_sequential(I2C_TypeDef *i2c) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  // Wait if RX FIFO is empty
  while (*regs.RX_FIFO & I2C_RX_FIFO_RX_EMPTY)
    ;

  // Read data, rx_rd_addr increases automatically
  return (*regs.RX_FIFO & I2C_RX_FIFO_RX_DATA_MASK) >> I2C_RX_FIFO_RX_DATA_POS;
}

// Start I2C transfer
int32_t i2c_start_transfer(I2C_TypeDef *i2c, uint16_t slave_addr,
                           uint32_t length, I2C_Mode_TypeDef mode,
                           I2C_AddrMode_TypeDef addr_mode) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  uint32_t start_val = 0;

  // Set transfer length
  start_val |= ((length & 0xFFF) << I2C_START_B_LENGTH_POS);

  // Set addressing mode
  if (addr_mode == I2C_ADDR_10BIT) {
    start_val |= I2C_START_B_10BITS;
  }

  // Set slave address
  start_val |= ((slave_addr & 0x3FF) << I2C_START_B_SLAVE_ADDR_POS);

  // Set transfer mode
  if (mode == I2C_MODE_READ) {
    start_val |= I2C_START_B_MODE;
  }

  *regs.START = start_val;

  // Start the transfer by setting pending bit
  *regs.START |= I2C_START_B_PENDING;

  return 0;
}

// Write a single byte
int32_t i2c_write_byte(I2C_TypeDef *i2c, uint16_t slave_addr, uint8_t data) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  // Initialize FIFO addresses for single byte transfer
  i2c_fifo_init(i2c, 0, 1, 0, 0);

  // Write data to TX FIFO
  i2c_fifo_write_sequential(i2c, data);

  // Start transfer
  i2c_start_transfer(i2c, slave_addr, 1, I2C_MODE_WRITE, I2C_ADDR_7BIT);

  // Wait for transfer to complete
  return i2c_wait_transfer_done(i2c);
}

// Read a single byte
int32_t i2c_read_byte(I2C_TypeDef *i2c, uint16_t slave_addr) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  // Initialize FIFO addresses for single byte transfer
  i2c_fifo_init(i2c, 0, 0, 0, 1);

  // Start transfer
  i2c_start_transfer(i2c, slave_addr, 1, I2C_MODE_READ, I2C_ADDR_7BIT);

  // Wait for transfer to complete
  int32_t result = i2c_wait_transfer_done(i2c);
  if (result != 0) {
    return result;
  }

  // Read data from RX FIFO
  return i2c_fifo_read_sequential(i2c);
}

// Write a block of data
int32_t i2c_write_block(I2C_TypeDef *i2c, uint16_t slave_addr, uint8_t *data,
                        uint32_t length) {
  if (__RARELY(i2c == NULL) || __RARELY(data == NULL) || length == 0) {
    return -1;
  }

  // Initialize FIFO addresses
  i2c_fifo_init(i2c, 0, length, 0, 0);

  // Write data to TX FIFO
  for (uint32_t i = 0; i < length; i++) {
    i2c_fifo_write_sequential(i2c, data[i]);
  }

  // Start transfer
  i2c_start_transfer(i2c, slave_addr, length, I2C_MODE_WRITE, I2C_ADDR_7BIT);

  // Wait for transfer to complete
  return i2c_wait_transfer_done(i2c);
}

// Read a block of data
int32_t i2c_read_block(I2C_TypeDef *i2c, uint16_t slave_addr, uint8_t *data,
                       uint32_t length) {
  if (__RARELY(i2c == NULL) || __RARELY(data == NULL) || length == 0) {
    return -1;
  }

  // Initialize FIFO addresses
  i2c_fifo_init(i2c, 0, 0, 0, length);

  // Start transfer
  i2c_start_transfer(i2c, slave_addr, length, I2C_MODE_READ, I2C_ADDR_7BIT);

  // Wait for transfer to complete
  int32_t result = i2c_wait_transfer_done(i2c);
  if (result != 0) {
    return result;
  }

  // Read data from RX FIFO
  for (uint32_t i = 0; i < length; i++) {
    int32_t byte = i2c_fifo_read_sequential(i2c);
    if (byte < 0) {
      return byte;
    }
    data[i] = (uint8_t)byte;
  }

  return 0;
}

// Get I2C status
int32_t i2c_get_status(I2C_TypeDef *i2c) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  return *regs.IRQ_STS;
}

// Clear interrupts
int32_t i2c_clear_interrupts(I2C_TypeDef *i2c, uint32_t irq_mask) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  *regs.IRQ_STS = irq_mask; // Write 1 to clear
  return 0;
}

// Wait for transfer to complete
int32_t i2c_wait_transfer_done(I2C_TypeDef *i2c) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  uint32_t status;

  // Wait for PHY_DONE or error
  do {
    status = *regs.IRQ_STS;

    // Check for error
    if (status & I2C_IRQ_PHY_ERR) {
      i2c_clear_interrupts(i2c, I2C_IRQ_PHY_ERR);
      return -2; // NACK or other error
    }
  } while (!(status & I2C_IRQ_PHY_DONE));

  // Clear the done interrupt
  i2c_clear_interrupts(i2c, I2C_IRQ_PHY_DONE);
  return 0;
}

// Check if I2C is busy
int32_t i2c_is_busy(I2C_TypeDef *i2c) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  uint32_t start_val = *regs.START;

  // Check if transfer is pending
  return (start_val & I2C_START_B_PENDING) ? 1 : 0;
}

// Acquire I2C bus
int32_t i2c_acquire_bus(I2C_TypeDef *i2c) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  // Set I2C ready to acquire the bus
  *regs.RDY &= ~I2C_RDY_I2C_RDY;
  return 0;
}

// Release I2C bus
int32_t i2c_release_bus(I2C_TypeDef *i2c) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);

  // Release the bus
  *regs.RDY |= I2C_RDY_I2C_RDY;
  return 0;
}

// Configure DMA mode
int32_t i2c_config_dma_mode(I2C_TypeDef *i2c, int enable) {
  if (__RARELY(i2c == NULL)) {
    return -1;
  }

  I2C_TypeDef regs = i2c_get_regs(i2c);
  uint32_t config_val = *regs.CONFIG;

  if (enable) {
    config_val |= I2C_CFG_B_DMA;
  } else {
    config_val &= ~I2C_CFG_B_DMA;
  }

  *regs.CONFIG = config_val;
  return 0;
}
