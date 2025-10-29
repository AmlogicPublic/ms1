#include "soc.h"
#include "Drivers/uart/uart.h"

__STATIC_FORCEINLINE uint64_t get_timer_freq(void)
{
    return (uint64_t)SOC_TIMER_FREQ;
}

// optimize measure_cpu_freq function with Os/O0
// to get a correct cpu frequency, which
// is important for flashxip linker script
#if defined ( __GNUC__ )
#pragma GCC push_options
#pragma GCC optimize ("Os")
#elif defined ( __ICCRISCV__ )
#pragma optimize=medium
#endif
uint32_t measure_cpu_freq(uint32_t n)
{
#if defined(__SYSTIMER_PRESENT) && (__SYSTIMER_PRESENT == 1)
    uint64_t start_timer, end_timer;
    uint64_t start_cycle, end_cycle;
    uint64_t timer_freq = get_timer_freq();
    
    // Simple measurement: read timer and cycle counters
    __enable_mcycle_counter();
    start_timer = SysTimer_GetLoadValue();
    start_cycle = __get_rv_cycle();
    
    // Wait for timer to advance by n ticks
    do {
        end_timer = SysTimer_GetLoadValue();
    } while ((end_timer - start_timer) < n);
    
    end_cycle = __get_rv_cycle();
    
    // Calculate frequency: avoid overflow by rearranging calculation
    uint64_t delta_cycle = end_cycle - start_cycle;
    uint64_t delta_timer = end_timer - start_timer;
    
    if (delta_timer == 0) return 0; // Avoid division by zero

    uint64_t result = (delta_cycle * timer_freq) / delta_timer;
    
    return (uint32_t)result;
#else
    // When system timer not exist, just return 1000000Hz
    #warning "measure_cpu_freq function require system timer present, if you are using this, it will not work"
    return 1000000;
#endif
}
#if defined ( __GNUC__ )
#pragma GCC pop_options
#elif defined ( __ICCRISCV__ )
#endif

uint32_t get_cpu_freq(void)
{
    // Direct measurement - use longer time for better accuracy
#ifdef CFG_SIMULATION
    return measure_cpu_freq(5);
#else
    return measure_cpu_freq(20);
#endif
}

/**
 * \brief      delay a time in milliseconds
 * \details
 *             provide API for delay
 * \param[in]  count: count in milliseconds
 * \remarks
 */
void delay_1ms(uint32_t count)
{
    uint64_t start_mtime, delta_mtime;
    uint64_t delay_ticks = (SOC_TIMER_FREQ * (uint64_t)count) / 1000;

#if defined(__SYSTIMER_PRESENT) && (__SYSTIMER_PRESENT == 1)
    start_mtime = SysTimer_GetLoadValue();

    do {
        delta_mtime = SysTimer_GetLoadValue() - start_mtime;
    } while (delta_mtime < delay_ticks);
#else
    #warning "delay_1ms function require system timer present, if you are using this, it will not work"
#endif
}

void simulation_exit(int status)
{
    // Both xlspike and qemu will write RXFIFO to make it works for xlspike even SIMU=qemu
    // workaround for fix cycle model exit with some message not print
    for (int i = 0; i < 10; i ++) {
        // print '\0' instead of '\r' for qemu simulation in ide
        uart_write(UART0, '\0');
    }
    uart_write(UART0, '\n');
    // pass exit status via rxfifo register
    // SIMULATION_EXIT(status);
#if defined(SIMULATION_MODE)
#if SIMULATION_MODE == SIMULATION_MODE_QEMU
    #define QEMU_VIRT_TEST_BASE 0x100000
    #define QEMU_SIG_EXIT      0x3333
    REG32(QEMU_VIRT_TEST_BASE) = (status << 16) | QEMU_SIG_EXIT;
#endif
#endif
}
