// See LICENSE for license details.

#ifndef _EVALSOC_UART_H
#define _EVALSOC_UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Real UART Hardware Register Definitions */
// FIFO size constant
#define UART_FIFO_SIZE 64

// Control register bit definitions
#define UART_CTRL_RTS_INV (1U << 31)       // Invert RTS signal
#define UART_CTRL_MASK_ERR (1U << 30)      // Mask errors
#define UART_CTRL_CTS_INV (1U << 29)       // Invert CTS signal
#define UART_CTRL_TX_INT_EN (1U << 28)     // Transmit interrupt enable
#define UART_CTRL_RX_INT_EN (1U << 27)     // Receive interrupt enable
#define UART_CTRL_TX_INV (1U << 26)        // Invert TX pin
#define UART_CTRL_RX_INV (1U << 25)        // Invert RX pin
#define UART_CTRL_CLR_ERR (1U << 24)       // Clear error
#define UART_CTRL_RST_RX (1U << 23)        // Reset receive state machine
#define UART_CTRL_RST_TX (1U << 22)        // Reset transmit state machine
#define UART_CTRL_CHAR_LEN_MASK (3U << 20) // Character length mask
#define UART_CTRL_CHAR_LEN_POS (20)        // Character length position
#define UART_CTRL_PARITY_EN (1U << 19)     // Parity enable
#define UART_CTRL_PARITY_TYPE (1U << 18)   // Parity type
#define UART_CTRL_STOP_MASK (3U << 16)     // Stop bit mask
#define UART_CTRL_STOP_POS (16)            // Stop bit position
#define UART_CTRL_TWO_WIRE (1U << 15)      // Two wire mode
#define UART_CTRL_RX_EN (1U << 13)         // Receive enable
#define UART_CTRL_TX_EN (1U << 12)         // Transmit enable
#define UART_CTRL_BAUD_MASK (0xFFFU)       // Baud rate mask

// Status register bit definitions
#define UART_STAT_RX_BUSY (1U << 26)      // Receive busy
#define UART_STAT_TX_BUSY (1U << 25)      // Transmit busy
#define UART_STAT_RX_OVERFLOW (1U << 24)  // Receive FIFO overflow
#define UART_STAT_CTS_LEVEL (1U << 23)    // CTS level
#define UART_STAT_TX_EMPTY (1U << 22)     // Transmit FIFO empty
#define UART_STAT_TX_FULL (1U << 21)      // Transmit FIFO full
#define UART_STAT_RX_EMPTY (1U << 20)     // Receive FIFO empty
#define UART_STAT_RX_FULL (1U << 19)      // Receive FIFO full
#define UART_STAT_FIFO_WR_FULL (1U << 18) // FIFO write full flag
#define UART_STAT_FRAME_ERR (1U << 17)    // Frame error
#define UART_STAT_PARITY_ERR (1U << 16)   // Parity error
#define UART_STAT_TX_CNT_MASK                                                  \
  (0xFFU << 8) // Transmit FIFO count mask (also used for MISC TX threshold)
#define UART_STAT_TX_CNT_POS                                                   \
  (8) // Transmit FIFO count position (also used for MISC TX threshold)
#define UART_STAT_RX_CNT_MASK                                                  \
  (0xFFU) // Receive FIFO count mask (also used for MISC RX threshold)
#define UART_STAT_RX_CNT_POS                                                   \
  (0) // Receive FIFO count position (also used for MISC RX threshold)

// Combined mask for MISC register threshold fields (reuses STATUS register
// masks)
#define UART_MISC_THRESH_MASK                                                  \
  (UART_STAT_TX_CNT_MASK | UART_STAT_RX_CNT_MASK) // Combined threshold mask

// REG5 register bit definitions
#define UART_REG5_XTAL2_SEL (1U << 27)      // XTAL2 clock select
#define UART_REG5_XTAL_SEL (1U << 26)       // XTAL clock select
#define UART_REG5_USE_XTAL (1U << 24)       // Use XTAL clock
#define UART_REG5_USE_NEW_BAUD (1U << 23)   // Use new baud rate
#define UART_REG5_NEW_BAUD_MASK (0x7FFFFFU) // New baud rate mask

// Real UART register structure for direct hardware access
typedef struct {
  volatile uint32_t *WFIFO;   // Write FIFO
  volatile uint32_t *RFIFO;   // Read FIFO
  volatile uint32_t *CONTROL; // Control register
  volatile uint32_t *STATUS;  // Status register
  volatile uint32_t *MISC;    // Misc register
  volatile uint32_t *REG5;    // REG5 register
} UART_TypeDef;

// UART instance identifiers (fake pointers used as identifiers)
#define UART0 ((UART_TypeDef *)0x1000)
#define UART1 ((UART_TypeDef *)0x2000)

// UART interrupt callback function types
typedef void (*uart_rx_callback_t)(uint8_t data);
typedef void (*uart_tx_callback_t)(void);

// Real UART enumerations based on actual hardware
typedef enum {
  UART_CLK_SYS = 0,   // System clock (167MHz)
  UART_CLK_XTAL = 1,  // XTAL clock (24MHz)
  UART_CLK_XTAL2 = 2, // XTAL2 clock (12MHz)
  UART_CLK_XTAL3 = 3, // XTAL3 clock (8MHz)
  UART_NOT_USE_XTAL = 4  // Not use XTAL clock(24Mhz)
} UART_ClkSrc_TypeDef;

typedef enum {
  UART_WIRE_3 = 0, // 3-wire mode with CTS/RTS
  UART_WIRE_2 = 1  // 2-wire mode TX/RX only
} UART_WireMode_TypeDef;

typedef enum {
  UART_CHAR_8BIT = 0, // 8 bits
  UART_CHAR_7BIT = 1, // 7 bits
  UART_CHAR_6BIT = 2, // 6 bits
  UART_CHAR_5BIT = 3  // 5 bits
} UART_CharLen_TypeDef;

typedef enum {
  UART_PARITY_EVEN = 0, // Even parity
  UART_PARITY_ODD = 1   // Odd parity
} UART_Parity_TypeDef;

typedef enum {
  UART_STOP_1BIT = 0, // 1 stop bit
  UART_STOP_2BIT = 1  // 2 stop bits
} UART_StopBit_TypeDef;

// UART functions based on real hardware
int32_t uart_init(UART_TypeDef *uart, uint32_t baudrate);
int32_t uart_config_stopbit(UART_TypeDef *uart, UART_StopBit_TypeDef stopbit);
int32_t uart_write(UART_TypeDef *uart, uint8_t val);
uint8_t uart_read(UART_TypeDef *uart);
int32_t uart_config_interrupts(UART_TypeDef *uart, int enable_rx,
                               int enable_tx);
int32_t uart_set_watermarks(UART_TypeDef *uart, uint32_t rx_watermark,
                            uint32_t tx_watermark);
int32_t uart_clear_status(UART_TypeDef *uart, uint32_t mask);

// Hardware-specific UART functions
int32_t uart_config_wire_mode(UART_TypeDef *uart,
                              UART_WireMode_TypeDef wire_mode);
int32_t uart_config_char_length(UART_TypeDef *uart,
                                UART_CharLen_TypeDef char_len);
int32_t uart_config_parity(UART_TypeDef *uart, int enable,
                           UART_Parity_TypeDef parity_type);
int32_t uart_config_clock_source(UART_TypeDef *uart,
                                 UART_ClkSrc_TypeDef clk_src,
                                 uint32_t baudrate);

// Status function (users can extract FIFO counts and busy flags from status
// register directly) TX FIFO count: (status & UART_STAT_TX_CNT_MASK) >>
// UART_STAT_TX_CNT_POS RX FIFO count: (status & UART_STAT_RX_CNT_MASK) TX busy:
// (status & UART_STAT_TX_BUSY) RX busy: (status & UART_STAT_RX_BUSY)
int32_t uart_get_status(UART_TypeDef *uart);

// Non-blocking I/O functions
int32_t uart_write_nb(UART_TypeDef *uart, uint8_t val);
int32_t uart_read_nb(UART_TypeDef *uart, uint8_t *val);

// UART interrupt functions
// Note: Interrupts are threshold-based:
// - RX interrupt: triggers when RX FIFO count > RX threshold (data ready for
// batch processing)
// - TX interrupt: triggers when TX FIFO count < TX threshold (space available
// for more data)
int uart_irq_init(void);
void eclic_uart_a_int_handler(void);
void eclic_uart_b_int_handler(void);

// UART interrupt callback registration functions
// rx_cb: called for each received byte when RX interrupt triggers
// tx_cb: called when TX FIFO has space (user should queue more data)
void uart_register_callbacks(UART_TypeDef *uart, uart_rx_callback_t rx_cb,
                             uart_tx_callback_t tx_cb);
void uart_unregister_callbacks(UART_TypeDef *uart);
#ifdef __cplusplus
}
#endif
#endif /* _EVALSOC_UART_H */
