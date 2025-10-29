// See LICENSE for license details.

#ifndef _EVALSOC_I2C_H
#define _EVALSOC_I2C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Debug I2C Hardware Register Definitions */
// Base addresses for Debug I2C controllers
#define I2C_DBG_BASE_ADDR0 (0xfe016000) // Debug I2C 0 base address
#define I2C_DBG_BASE_ADDR1                                                     \
  (0xfe016000 + 0x1000) // Debug I2C 1 base address (assumed offset)

// FIFO size constant
#define I2C_FIFO_SIZE 128

// Register offsets from base address (from documentation)
#define I2C_REG_RDY_OFFSET (0x00)        // Ready and tee
#define I2C_REG_CONFIG_OFFSET (0x04)     // Set DMA and join
#define I2C_REG_START_OFFSET (0x08)      // Write at last
#define I2C_REG_BUS_OFFSET (0x0C)        // Set slave mode
#define I2C_REG_TX_RD_ADDR_OFFSET (0x10) // tx fifo pull address
#define I2C_REG_TX_WR_ADDR_OFFSET (0x14) // tx fifo push address
#define I2C_REG_RX_RD_ADDR_OFFSET (0x18) // rx fifo pull address
#define I2C_REG_RX_WR_ADDR_OFFSET (0x1C) // rx fifo push address
#define I2C_REG_TX_FIFO_OFFSET (0x20)    // empty, full, tx_data
#define I2C_REG_RX_FIFO_OFFSET (0x24)    // empty, full, rx_data
#define I2C_REG_IRQ_STS_OFFSET (0x30)    // Write 1 to clear irq
#define I2C_REG_IRQ_EN_OFFSET (0x34)     // Irq mask
#define I2C_REG_SHAKE_OFFSET (0x38)      // RO

// Control register bit definitions (I2C_DBG_T_CTRL)
#define I2C_DBG_T_CTRL_DBG1_SAMPLE_DLY_MASK                                    \
  (0x7U << 12) // debug_i2c 1 cntl_sample_dly
#define I2C_DBG_T_CTRL_DBG1_SAMPLE_DLY_POS (12)
#define I2C_DBG_T_CTRL_DBG1_FILTER_SEL_MASK                                    \
  (0x3U << 9) // debug_i2c 1 cntl_filter_sel
#define I2C_DBG_T_CTRL_DBG1_FILTER_SEL_POS (9)
#define I2C_DBG_T_CTRL_DBG0_SAMPLE_DLY_MASK                                    \
  (0x7U << 4) // debug_i2c 0 cntl_sample_dly
#define I2C_DBG_T_CTRL_DBG0_SAMPLE_DLY_POS (4)
#define I2C_DBG_T_CTRL_DBG0_FILTER_SEL_MASK                                    \
  (0x3U << 1) // debug_i2c 0 cntl_filter_sel
#define I2C_DBG_T_CTRL_DBG0_FILTER_SEL_POS (1)

// RDY register bit definitions
#define I2C_RDY_TEE_OPEN (1U << 1) // If 1, tee open; If 0, tee_lock
#define I2C_RDY_I2C_RDY (1U << 0)  // If 0, controller owns i2c

// CONFIG register bit definitions (CFG_I2C)
#define I2C_CFG_RX_THRESH_MASK                                                 \
  (0xFFU << 16) // rx_fifo_irq = rx_count >= rx_thresh
#define I2C_CFG_RX_THRESH_POS (16)
#define I2C_CFG_TX_THRESH_MASK                                                 \
  (0xFFU << 8) // tx_fifo_irq = tx_count <= tx_thresh
#define I2C_CFG_TX_THRESH_POS (8)
#define I2C_CFG_B_DMA (1U << 4)     // Set 1 to use DMA
#define I2C_CFG_B_HW_NEG (1U << 3)  // Use hw_trig's negedge to start
#define I2C_CFG_B_HW_POS (1U << 2)  // Use hw_trig's posedge to start
#define I2C_CFG_B_JOIN_RX (1U << 1) // If 1, whole fifo used by RX
#define I2C_CFG_B_JOIN_TX (1U << 0) // If 1, whole fifo used by TX

// START register bit definitions (CFG_START)
#define I2C_START_B_PENDING (1U << 31)         // Set 1 to start transfer
#define I2C_START_B_LENGTH_MASK (0xFFFU << 12) // Number of blocks
#define I2C_START_B_LENGTH_POS (12)
#define I2C_START_B_10BITS (1U << 11)             // 10 bits slave addr mode
#define I2C_START_B_SLAVE_ADDR_MASK (0x3FFU << 1) // 7bits or 10 bits
#define I2C_START_B_SLAVE_ADDR_POS (1)
#define I2C_START_B_MODE (1U << 0) // Transfer mode: 0=write, 1=read

// BUS register bit definitions (CFG_BUS)
#define I2C_BUS_SPEED_MODE (1U << 17)   // 1: 100k
#define I2C_BUS_B_SLAVE_MODE (1U << 16) // 1: I2C slave mode
#define I2C_BUS_B_FILTER_SEL_MASK                                              \
  (0xFU << 12) // Used in din_filter to avoid glitch
#define I2C_BUS_B_FILTER_SEL_POS (12)
#define I2C_BUS_B_RATIO_MASK (0xFFFU << 0) // SCL = clk/b_ratio
#define I2C_BUS_B_RATIO_POS (0)

// TX_FIFO register bit definitions
#define I2C_TX_FIFO_TX_COUNT_MASK (0x1FFU << 11) // TX FIFO count
#define I2C_TX_FIFO_TX_COUNT_POS (11)
#define I2C_TX_FIFO_APB_TX_ERR (1U << 10)     // apb_tx_fifo & tx_full
#define I2C_TX_FIFO_TX_EMPTY (1U << 9)        // TX FIFO empty
#define I2C_TX_FIFO_TX_FULL (1U << 8)         // TX FIFO full
#define I2C_TX_FIFO_TX_DATA_MASK (0xFFU << 0) // TX data
#define I2C_TX_FIFO_TX_DATA_POS (0)

// RX_FIFO register bit definitions
#define I2C_RX_FIFO_RX_COUNT_MASK (0x1FFU << 11) // RX FIFO count
#define I2C_RX_FIFO_RX_COUNT_POS (11)
#define I2C_RX_FIFO_APB_RX_ERR (1U << 10)     // apb_rx_fifo & rx_empty
#define I2C_RX_FIFO_RX_EMPTY (1U << 9)        // RX FIFO empty
#define I2C_RX_FIFO_RX_FULL (1U << 8)         // RX FIFO full
#define I2C_RX_FIFO_RX_DATA_MASK (0xFFU << 0) // RX data
#define I2C_RX_FIFO_RX_DATA_POS (0)

// IRQ_STS / IRQ_ENABLE register bit definitions
#define I2C_IRQ_ALL_DONE (1U << 9)    // task_done & phy_done
#define I2C_IRQ_TASK_DONE (1U << 8)   // DMA transfer done
#define I2C_IRQ_PHY_DONE (1U << 7)    // I2C transfer done
#define I2C_IRQ_TX_FIFO_IRQ (1U << 6) // TX FIFO interrupt
#define I2C_IRQ_RX_FIFO_IRQ (1U << 5) // RX FIFO interrupt
#define I2C_IRQ_TX_FULL (1U << 4)     // TX FIFO full
#define I2C_IRQ_TX_EMPTY (1U << 3)    // TX FIFO empty
#define I2C_IRQ_RX_FULL (1U << 2)     // RX FIFO full
#define I2C_IRQ_RX_EMPTY (1U << 1)    // RX FIFO empty
#define I2C_IRQ_PHY_ERR (1U << 0)     // NACK error

// SHAKE register bit definitions (read-only)
#define I2C_SHAKE_BLK_CNT_MASK (0x1FFFU << 4) // Number of bytes
#define I2C_SHAKE_BLK_CNT_POS (4)
#define I2C_SHAKE_TX_DATA_VLD (1U << 3) // TX data valid
#define I2C_SHAKE_TX_DATA_RDY (1U << 2) // TX data ready
#define I2C_SHAKE_RX_DATA_VLD (1U << 1) // RX data valid
#define I2C_SHAKE_RX_DATA_RDY (1U << 0) // RX data ready

// I2C register structure for direct hardware access
typedef struct {
  volatile uint32_t *RDY;        // Ready and tee register
  volatile uint32_t *CONFIG;     // Configuration register
  volatile uint32_t *START;      // Start register
  volatile uint32_t *BUS;        // Bus configuration register
  volatile uint32_t *TX_RD_ADDR; // TX FIFO read address
  volatile uint32_t *TX_WR_ADDR; // TX FIFO write address
  volatile uint32_t *RX_RD_ADDR; // RX FIFO read address
  volatile uint32_t *RX_WR_ADDR; // RX FIFO write address
  volatile uint32_t *TX_FIFO;    // TX FIFO register
  volatile uint32_t *RX_FIFO;    // RX FIFO register
  volatile uint32_t *IRQ_STS;    // Interrupt status register
  volatile uint32_t *IRQ_EN;     // Interrupt enable register
  volatile uint32_t *SHAKE;      // Shake register (read-only)
} I2C_TypeDef;

// I2C instance identifiers (fake pointers used as identifiers)
#define I2C_DBG0 ((I2C_TypeDef *)0x3000) // Debug I2C 0
#define I2C_DBG1 ((I2C_TypeDef *)0x4000) // Debug I2C 1

// I2C interrupt callback function types
typedef void (*i2c_rx_callback_t)(uint8_t data);
typedef void (*i2c_tx_callback_t)(void);
typedef void (*i2c_error_callback_t)(uint32_t error);

// I2C enumerations
typedef enum {
  I2C_MODE_WRITE = 0, // Write mode
  I2C_MODE_READ = 1   // Read mode
} I2C_Mode_TypeDef;

typedef enum {
  I2C_SPEED_STANDARD = 0, // Standard mode (400kHz)
  I2C_SPEED_FAST = 1      // Fast mode (100kHz)
} I2C_Speed_TypeDef;

typedef enum {
  I2C_ADDR_7BIT = 0, // 7-bit addressing
  I2C_ADDR_10BIT = 1 // 10-bit addressing
} I2C_AddrMode_TypeDef;

typedef enum {
  I2C_FIFO_MODE_APB_RANDOM = 0,     // APB random mode
  I2C_FIFO_MODE_APB_SEQUENTIAL = 1, // APB sequential mode
  I2C_FIFO_MODE_DMA = 2             // DMA mode
} I2C_FifoMode_TypeDef;

// Main I2C functions
int32_t i2c_init(I2C_TypeDef *i2c);
int32_t i2c_config_bus(I2C_TypeDef *i2c, uint32_t clk_ratio,
                       I2C_Speed_TypeDef speed);
int32_t i2c_config_slave_mode(I2C_TypeDef *i2c, int enable);
int32_t i2c_config_interrupts(I2C_TypeDef *i2c, uint32_t irq_mask);
int32_t i2c_set_fifo_thresholds(I2C_TypeDef *i2c, uint32_t rx_threshold,
                                uint32_t tx_threshold);

// FIFO management functions
int32_t i2c_fifo_init(I2C_TypeDef *i2c, uint32_t tx_rd_addr,
                      uint32_t tx_wr_addr, uint32_t rx_rd_addr,
                      uint32_t rx_wr_addr);
int32_t i2c_fifo_write_random(I2C_TypeDef *i2c, uint32_t fifo_addr,
                              uint32_t data);
int32_t i2c_fifo_write_sequential(I2C_TypeDef *i2c, uint8_t data);
int32_t i2c_fifo_read_random(I2C_TypeDef *i2c, uint32_t fifo_addr);
int32_t i2c_fifo_read_sequential(I2C_TypeDef *i2c);

// Transfer functions
int32_t i2c_start_transfer(I2C_TypeDef *i2c, uint16_t slave_addr,
                           uint32_t length, I2C_Mode_TypeDef mode,
                           I2C_AddrMode_TypeDef addr_mode);
int32_t i2c_write_byte(I2C_TypeDef *i2c, uint16_t slave_addr, uint8_t data);
int32_t i2c_read_byte(I2C_TypeDef *i2c, uint16_t slave_addr);
int32_t i2c_write_block(I2C_TypeDef *i2c, uint16_t slave_addr, uint8_t *data,
                        uint32_t length);
int32_t i2c_read_block(I2C_TypeDef *i2c, uint16_t slave_addr, uint8_t *data,
                       uint32_t length);

// Status and control functions
int32_t i2c_get_status(I2C_TypeDef *i2c);
int32_t i2c_clear_interrupts(I2C_TypeDef *i2c, uint32_t irq_mask);
int32_t i2c_wait_transfer_done(I2C_TypeDef *i2c);
int32_t i2c_is_busy(I2C_TypeDef *i2c);

// Advanced functions
int32_t i2c_acquire_bus(I2C_TypeDef *i2c);
int32_t i2c_release_bus(I2C_TypeDef *i2c);
int32_t i2c_config_dma_mode(I2C_TypeDef *i2c, int enable);

// I2C interrupt functions
int i2c_irq_init(void);
void eclic_i2c_dbg0_int_handler(void);
void eclic_i2c_dbg1_int_handler(void);

// I2C interrupt callback registration functions
void i2c_register_callbacks(I2C_TypeDef *i2c, i2c_rx_callback_t rx_cb,
                            i2c_tx_callback_t tx_cb,
                            i2c_error_callback_t err_cb);
void i2c_unregister_callbacks(I2C_TypeDef *i2c);

// Internal register access functions (used by IRQ handlers)
I2C_TypeDef i2c_get_real_regs_dbg0(void);
I2C_TypeDef i2c_get_real_regs_dbg1(void);

#ifdef __cplusplus
}
#endif

#endif /* _EVALSOC_I2C_H */
