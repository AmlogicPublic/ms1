
/* Kernel includes. */
#include "FreeRTOS.h" // IWYU pragma: export
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */

#include <stdio.h>
#include <stdlib.h>

// #include "soc.h"
// #include "Drivers/uart/uart.h"
#include "Drivers/clock_tree/clock_tree.h"

#define mainSOFTWARE_TIMER_PERIOD_MS pdMS_TO_TICKS(500) // ms -> ticks
#define mainQUEUE_LENGTH (1)
#define TASKDLYMS pdMS_TO_TICKS(100)

// 简单统一的时钟配置 - 一个函数配置所有时钟
static void clocks_setup(void) {
  printf("Starting MS1 Clock Tree Configuration...\n");

  // 配置系统时钟分支A: target=sys_clk_a, source=fclk_div2, freq=166MHz
  if (clock_set_frequency(CLK_AO_SYS_CLK_A, CLK_SRC_XTAL, 24) == 0) {
    printf("✓ sys_clk_a: %u MHz (source: fclk_div2)\n", clock_get_frequency(CLK_AO_SYS_CLK_A));
  } else {
    printf("✗ sys_clk_a configuration failed\n");
  }

  // 配置系统时钟选择分支A: target=sys_clk, source=sys_clk_a, freq=166MHz
  if (clock_set_frequency(CLK_AO_SYS_CLK, CLK_AO_SYS_CLK_A, 24) == 0) {
    printf("✓ sys_clk: %u MHz (source: sys_clk_a)\n", clock_get_frequency(CLK_AO_SYS_CLK));
  } else {
    printf("✗ sys_clk selection failed\n");
  }
}

int main(void) {
  // 简单统一的时钟配置 - 一个函数配置所有时钟
  // clocks_setup();

  while (1)
    printf("Hello, World!\r\n");
    // printf("⣿⡟⠙⠛⠋⠩⠭⣉⡛⢛⠫⠭⠄⠒⠄⠄⠄⠈⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿\r\n"
    //        "⣿⡇⠄⠄⠄⠄⣠⠖⠋⣀⡤⠄⠒⠄⠄⠄⠄⠄⠄⠄⠄⠄⣈⡭⠭⠄⠄⠄⠉⠙\r\n"
    //        "⣿⡇⠄⠄⢀⣞⣡⠴⠚⠁⠄⠄⢀⠠⠄⠄⠄⠄⠄⠄⠄⠉⠄⠄⠄⠄⠄⠄⠄⠄\r\n"
    //        "⣿⡇⠄⡴⠁⡜⣵⢗⢀⠄⢠⡔⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"
    //        "⣿⡇⡜⠄⡜⠄⠄⠄⠉⣠⠋⠠⠄⢀⡄⠄⠄⣠⣆⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⢸\r\n"
    //        "⣿⠸⠄⡼⠄⠄⠄⠄⢰⠁⠄⠄⠄⠈⣀⣠⣬⣭⣛⠄⠁⠄⡄⠄⠄⠄⠄⠄⢀⣿\r\n"
    //        "⣏⠄⢀⠁⠄⠄⠄⠄⠇⢀⣠⣴⣶⣿⣿⣿⣿⣿⣿⡇⠄⠄⡇⠄⠄⠄⠄⢀⣾⣿\r\n"
    //        "⣿⣸⠈⠄⠄⠰⠾⠴⢾⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⢁⣾⢀⠁⠄⠄⠄⢠⢸⣿⣿\r\n"
    //        "⣿⣿⣆⠄⠆⠄⣦⣶⣦⣌⣿⣿⣿⣿⣷⣋⣀⣈⠙⠛⡛⠌⠄⠄⠄⠄⢸⢸⣿⣿\r\n"
    //        "⣿⣿⣿⠄⠄⠄⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠈⠄⠄⠄⠄⠄⠈⢸⣿⣿\r\n"
    //        "⣿⣿⣿⠄⠄⠄⠘⣿⣿⣿⡆⢀⣈⣉⢉⣿⣿⣯⣄⡄⠄⠄⠄⠄⠄⠄⠄⠈⣿⣿\r\n"
    //        "⣿⣿⡟⡜⠄⠄⠄⠄⠙⠿⣿⣧⣽⣍⣾⣿⠿⠛⠁⠄⠄⠄⠄⠄⠄⠄⠄⠃⢿⣿\r\n"
    //        "⣿⡿⠰⠄⠄⠄⠄⠄⠄⠄⠄⠈⠉⠩⠔⠒⠉⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠐⠘⣿\r\n"
    //        "⣿⠃⠃⠄⠄⠄⠄⠄⠄⣀⢀⠄⠄⡀⡀⢀⣤⣴⣤⣤⣀⣀⠄⠄⠄⠄⠄⠄⠁⢹\r\n");
}

void vApplicationTickHook(void) {};
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {};

