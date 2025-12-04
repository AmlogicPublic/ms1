#include "Drivers/uart/uart.h"
#include "ms1/register.h"
#include "soc.h"

// All register bit definitions are now in uart.h header file

// Get real UART register structure for UART_A
static inline UART_TypeDef uart_get_real_regs_a(void) {
  UART_TypeDef uart_regs = {.WFIFO = (volatile uint32_t *)UART_A_WFIFO,
                            .RFIFO = (volatile uint32_t *)UART_A_RFIFO,
                            .CONTROL = (volatile uint32_t *)UART_A_CONTROL,
                            .STATUS = (volatile uint32_t *)UART_A_STATUS,
                            .MISC = (volatile uint32_t *)UART_A_MISC,
                            .REG5 = (volatile uint32_t *)UART_A_REG5};
  return uart_regs;
}

// Get real UART register structure for UART_B
static inline UART_TypeDef uart_get_real_regs_b(void) {
  UART_TypeDef uart_regs = {.WFIFO = (volatile uint32_t *)UART_B_WFIFO,
                            .RFIFO = (volatile uint32_t *)UART_B_RFIFO,
                            .CONTROL = (volatile uint32_t *)UART_B_CONTROL,
                            .STATUS = (volatile uint32_t *)UART_B_STATUS,
                            .MISC = (volatile uint32_t *)UART_B_MISC,
                            .REG5 = (volatile uint32_t *)UART_B_REG5};
  return uart_regs;
}

// Clear all pinmux registers
static void clear_all_regs(void) {
  Wr(AO_PADCTRL_PIN_MUX_REG0, 0);
  Wr(AO_PADCTRL_PIN_MUX_REG1, 0);
  //Wr(AO_PADCTRL_PIN_MUX_REG2, 0);
  Wr(AO_PADCTRL_PIN_MUX_REG3, 0);
  Wr(EE_PADCTRL_PIN_MUX_REG0, 0);
  Wr(EE_PADCTRL_PIN_MUX_REG1, 0);
  Wr(EE_PADCTRL_PIN_MUX_REG2, 0);
  Wr(EE_PADCTRL_PIN_MUX_REG3, 0);
}

// Set UART_A pinmux configuration
static void set_uart_a_pinmux(void) {
  clear_all_regs();
  // Configure GPIOW_2 as UART_A_TX and GPIOW_3 as UART_A_RX
  Wr_reg_bits(AO_PADCTRL_PIN_MUX_REG2, 2, 8, 4);  // UART_A_TX
  Wr_reg_bits(AO_PADCTRL_PIN_MUX_REG2, 2, 12, 4); // UART_A_RX

  // Alternative pin configurations (commented out):
  // Wr_reg_bits(AO_PADCTRL_PIN_MUX_REG1, 2, 16, 4); // GPIOAO_12: UART_A_TX
  // Wr_reg_bits(AO_PADCTRL_PIN_MUX_REG1, 2, 20, 4); // GPIOAO_13: UART_A_RX
  // Wr_reg_bits(AO_PADCTRL_PIN_MUX_REG2, 4, 24, 4); // GPIOW_6: UART_A_TX
  // Wr_reg_bits(AO_PADCTRL_PIN_MUX_REG2, 4, 28, 4); // GPIOW_7: UART_A_RX
}

// Set UART_B pinmux configuration
static void set_uart_b_pinmux(void) { clear_all_regs(); }

// Get real UART registers based on fake UART_TypeDef pointer
static UART_TypeDef uart_get_regs(UART_TypeDef *uart) {
  // Map fake UART pointers to real hardware registers
  if (uart == UART0) {
    return uart_get_real_regs_a();
  } else if (uart == UART1) {
    return uart_get_real_regs_b();
  } else {
    // Default to UART_A if invalid pointer
    return uart_get_real_regs_a();
  }
}

// Configure UART clock source and baud rate
static int32_t uart_config_clock(UART_TypeDef regs, UART_ClkSrc_TypeDef clk_src,
                                 uint32_t baudrate) {
  uint32_t reg5_val = *regs.REG5;
  uint32_t ctrl_val = *regs.CONTROL;
  uint32_t clk_freq;
  uint32_t baud_div;

  // Clear previous clock settings
  reg5_val &= ~(UART_REG5_XTAL2_SEL | UART_REG5_XTAL_SEL | UART_REG5_USE_XTAL |
                UART_REG5_USE_NEW_BAUD);

  switch (clk_src) {
  case UART_CLK_XTAL3:
    reg5_val |= 0;      //| UART_REG5_USE_XTAL;
    clk_freq = 8000000; // 8MHz
    baud_div = (clk_freq / baudrate) - 1;
    ctrl_val =
        (ctrl_val & ~UART_CTRL_BAUD_MASK) | (baud_div & UART_CTRL_BAUD_MASK);
    break;

  case UART_CLK_XTAL2:
    reg5_val |= UART_REG5_XTAL2_SEL; // | UART_REG5_USE_XTAL;
    clk_freq = 12000000;             // 12MHz
    baud_div = (clk_freq / baudrate) - 1;
    ctrl_val =
        (ctrl_val & ~UART_CTRL_BAUD_MASK) | (baud_div & UART_CTRL_BAUD_MASK);
    break;

  case UART_CLK_XTAL:
    reg5_val |= UART_REG5_XTAL_SEL; // | UART_REG5_USE_XTAL;
    clk_freq = 24000000;            // 24MHz
    baud_div = (clk_freq / baudrate) - 1;
    ctrl_val =
        (ctrl_val & ~UART_CTRL_BAUD_MASK) | (baud_div & UART_CTRL_BAUD_MASK);
    break;

  case UART_CLK_SYS:
    clk_freq = 167000000; // 167MHz system clock divided by 4
    baud_div = (clk_freq / 4 / baudrate) - 1;
    ctrl_val =
        (ctrl_val & ~UART_CTRL_BAUD_MASK) | (baud_div & UART_CTRL_BAUD_MASK);
    break;

  case UART_NOT_USE_XTAL:
  default:
    clk_freq = 41625000;
    baud_div = (clk_freq / 4 / baudrate) - 1;
    ctrl_val =
        (ctrl_val & ~UART_CTRL_BAUD_MASK) | (baud_div & UART_CTRL_BAUD_MASK);
    break;
  }

  *regs.REG5 = reg5_val;
  *regs.CONTROL = ctrl_val;
  return 0;
}

// Initialize UART with real hardware registers (order matters!)
int32_t uart_init(UART_TypeDef *uart, uint32_t baudrate) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);

  // Step 1: Configure clock FIRST (before pinmux to prevent glitches)
  uart_config_clock(regs, UART_NOT_USE_XTAL, baudrate);

  // Step 2: Set pinmux after clock is stable
  if (uart == UART0) {
    set_uart_a_pinmux();
  } else if (uart == UART1) {
    set_uart_b_pinmux();
  }

  // Step 3: Configure basic UART settings
  uint32_t ctrl_val = *regs.CONTROL;
  ctrl_val |= UART_CTRL_PARITY_EN; // Enable parity as per test code

  // Step 4: Set FIFO interrupt thresholds before enabling interrupts
  uint32_t misc_val = *regs.MISC;
  misc_val = (misc_val & ~UART_MISC_THRESH_MASK) |
             (63 << UART_STAT_TX_CNT_POS) |
             (44 << UART_STAT_RX_CNT_POS); // TX threshold: 10, RX threshold: 44
  *regs.MISC = misc_val;

  // Step 5: Configure two-wire mode (as per test code)
  ctrl_val |= UART_CTRL_TWO_WIRE; // Two-wire mode (TX/RX only, no CTS/RTS)

  // Step 6: Enable UART TX/RX BEFORE reset (critical order as per test code!)
  ctrl_val |= UART_CTRL_TX_EN; // Enable transmitter
  ctrl_val |= UART_CTRL_RX_EN; // Enable receiver
  *regs.CONTROL = ctrl_val;

  // Step 7: Reset UART (after enabling - as per test code sequence)
  ctrl_val |= UART_CTRL_RST_TX | UART_CTRL_RST_RX | UART_CTRL_CLR_ERR;
  *regs.CONTROL = ctrl_val;

  // Step 8: Clear reset bits
  ctrl_val &= ~(UART_CTRL_RST_TX | UART_CTRL_RST_RX | UART_CTRL_CLR_ERR);
  *regs.CONTROL = ctrl_val;

  // enable interrupts after enabling tx/rx
  uart_config_interrupts(uart, 1, 1);

  return 0;
}

// Configure stop bit setting
int32_t uart_config_stopbit(UART_TypeDef *uart, UART_StopBit_TypeDef stopbit) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  uint32_t ctrl_val = *regs.CONTROL;

  // Clear stop bit field and set new value
  ctrl_val &= ~UART_CTRL_STOP_MASK;
  ctrl_val |= ((uint32_t)stopbit << UART_CTRL_STOP_POS) & UART_CTRL_STOP_MASK;

  *regs.CONTROL = ctrl_val;
  return 0;
}

// Write data to UART transmit FIFO
int32_t uart_write(UART_TypeDef *uart, uint8_t val) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);

  // Wait until transmit FIFO is not full
  while (*regs.STATUS & UART_STAT_TX_FULL)
    ;

  // Write data to transmit FIFO
  *regs.WFIFO = val;
  return 0;
}

// Read data from UART receive FIFO
uint8_t uart_read(UART_TypeDef *uart) {
  if (__RARELY(uart == NULL)) {
    return 0xFF;
  }

  UART_TypeDef regs = uart_get_regs(uart);

  // Wait until receive FIFO is not empty
  while (*regs.STATUS & UART_STAT_RX_EMPTY)
    ;

  // Read data from receive FIFO
  return (uint8_t)(*regs.RFIFO & 0xFF);
}

// Configure UART interrupts
int32_t uart_config_interrupts(UART_TypeDef *uart, int enable_rx,
                               int enable_tx) {
  // receive interrupt (uart HW pipe is nearly full)
  // transmit interrupt (uart HW pipe doesn't have enough data)
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  uint32_t ctrl_val = *regs.CONTROL;

  if (enable_rx) {
    ctrl_val |= UART_CTRL_RX_INT_EN;
  } else {
    ctrl_val &= ~UART_CTRL_RX_INT_EN;
  }

  if (enable_tx) {
    ctrl_val |= UART_CTRL_TX_INT_EN;
  } else {
    ctrl_val &= ~UART_CTRL_TX_INT_EN;
  }

  *regs.CONTROL = ctrl_val;
  return 0;
}

// Set FIFO interrupt watermarks
int32_t uart_set_watermarks(UART_TypeDef *uart, uint32_t rx_watermark,
                            uint32_t tx_watermark) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  uint32_t misc_val = *regs.MISC;

  // Set TX threshold (bits 15-8) and RX threshold (bits 7-0)
  misc_val &= ~UART_MISC_THRESH_MASK;
  misc_val |= ((tx_watermark & 0xFF) << UART_STAT_TX_CNT_POS) |
              ((rx_watermark & 0xFF) << UART_STAT_RX_CNT_POS);

  *regs.MISC = misc_val;
  return 0;
}

// Get UART status register
int32_t uart_get_status(UART_TypeDef *uart) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  return *regs.STATUS;
}

// Clear UART error status
int32_t uart_clear_status(UART_TypeDef *uart, uint32_t mask) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);

  // Clear errors by pulsing the clear error bit
  *regs.CONTROL |= UART_CTRL_CLR_ERR;
  *regs.CONTROL &= ~UART_CTRL_CLR_ERR;

  return 0;
}

// Additional real UART functions based on hardware capabilities

// Configure UART wire mode (2-wire or 3-wire)
int32_t uart_config_wire_mode(UART_TypeDef *uart,
                              UART_WireMode_TypeDef wire_mode) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  uint32_t ctrl_val = *regs.CONTROL;

  if (wire_mode == UART_WIRE_2) {
    ctrl_val |= UART_CTRL_TWO_WIRE; // 2-wire mode (TX/RX only)
  } else {
    ctrl_val &= ~UART_CTRL_TWO_WIRE; // 3-wire mode (with CTS/RTS)
  }

  *regs.CONTROL = ctrl_val;
  return 0;
}

// Configure character length
int32_t uart_config_char_length(UART_TypeDef *uart,
                                UART_CharLen_TypeDef char_len) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  uint32_t ctrl_val = *regs.CONTROL;

  ctrl_val &= ~UART_CTRL_CHAR_LEN_MASK;
  ctrl_val |=
      ((uint32_t)char_len << UART_CTRL_CHAR_LEN_POS) & UART_CTRL_CHAR_LEN_MASK;

  *regs.CONTROL = ctrl_val;
  return 0;
}

// Configure parity settings
int32_t uart_config_parity(UART_TypeDef *uart, int enable,
                           UART_Parity_TypeDef parity_type) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  uint32_t ctrl_val = *regs.CONTROL;

  if (enable) {
    ctrl_val |= UART_CTRL_PARITY_EN;
    if (parity_type == UART_PARITY_ODD) {
      ctrl_val |= UART_CTRL_PARITY_TYPE;
    } else {
      ctrl_val &= ~UART_CTRL_PARITY_TYPE;
    }
  } else {
    ctrl_val &= ~UART_CTRL_PARITY_EN;
  }

  *regs.CONTROL = ctrl_val;
  return 0;
}

// Configure clock source and baud rate (public function)
int32_t uart_config_clock_source(UART_TypeDef *uart,
                                 UART_ClkSrc_TypeDef clk_src,
                                 uint32_t baudrate) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);
  return uart_config_clock(regs, clk_src, baudrate);
}

// Non-blocking write function
int32_t uart_write_nb(UART_TypeDef *uart, uint8_t val) {
  if (__RARELY(uart == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);

  // Check if transmit FIFO is full
  if (*regs.STATUS & UART_STAT_TX_FULL) {
    return -2; // FIFO full, cannot write
  }

  // Write data to transmit FIFO
  *regs.WFIFO = val;
  return 0;
}

// Non-blocking read function
int32_t uart_read_nb(UART_TypeDef *uart, uint8_t *val) {
  if (__RARELY(uart == NULL) || __RARELY(val == NULL)) {
    return -1;
  }

  UART_TypeDef regs = uart_get_regs(uart);

  // Check if receive FIFO is empty
  if (*regs.STATUS & UART_STAT_RX_EMPTY) {
    return -2; // FIFO empty, no data available
  }

  // Read data from receive FIFO
  *val = (uint8_t)(*regs.RFIFO & 0xFF);
  return 0;
}