
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

extern void idle_task(void);
static void vExampleTimerCallback(TimerHandle_t xTimer);

static TaskHandle_t StartTask1_Handler;
static TaskHandle_t StartTask2_Handler;

void task1(void *pvParameters);
void task2(void *pvParameters);

void print_freertos_banner();
void print_mcfg_info();
void print_soc_status();
void clocks_setup();

extern uintptr_t __StackTop, __StackLimit, __heap_start, __heap_limit;
extern volatile uint32_t SystemCoreClock; // will be updated during boot
extern volatile uint32_t BAUD_RATE;       // will be updated during boot
extern void _start(void);                 // CPU reset entry point from linker script

int main(void) {

  // TimerHandle_t xSoftwareTimer = xTimerCreate(
  //     (const char *)"ExTimer",
  //     mainSOFTWARE_TIMER_PERIOD_MS,
  //     pdTRUE, (void *)0,
  //     vExampleTimerCallback);
  //
  // QueueHandle_t xQueue = xQueueCreate(
  //     /* The number of items the queue can hold. */
  //     mainQUEUE_LENGTH,
  //     /* The size of each item the queue holds. */
  //     sizeof(uint32_t));
  //
  // if (xQueue == NULL) {
  //   printf("Unable to create xQueue due to low memory.\n");
  //   while (1)
  //     ;
  // }
  //
  // xTaskCreate((TaskFunction_t)task1, (const char *)"task1",
  //             (uint16_t)256, (void *)NULL, (UBaseType_t)2,
  //             (TaskHandle_t *)&StartTask1_Handler);
  //
  // xTaskCreate((TaskFunction_t)task2, (const char *)"task2",
  //             (uint16_t)256, (void *)NULL, (UBaseType_t)1,
  //             (TaskHandle_t *)&StartTask2_Handler);

  // xTimerStart(xSoftwareTimer, 0);

  print_freertos_banner();
  print_mcfg_info();
  print_soc_status();
  clocks_setup();

  printf("Start Scheduler...\r\n");

  Wr(EE_SYSCTRL_VGD_CTRL, 0x1234);
  int rdata;
  rdata = Rd(EE_SYSCTRL_VGD_CTRL);
  printf("EE_SYSCTRL_VGD_CTRL: 0x%x\r\n", rdata);

  Wr(RDMA_AHB_END_ADDR_3, 0x12345678);
  rdata = Rd(RDMA_AHB_END_ADDR_3);
  printf("VPU RDMA_AHB_END_ADDR_3: 0x%x\r\n", rdata);

  Wr(VOUT_PRE_CMPR_CTRL1, 0x4728abcd);
  rdata = Rd(VOUT_PRE_CMPR_CTRL1);
  printf("VOUT VOUT_PRE_CMPR_CTRL1: 0x%x\r\n", rdata);




  // vTaskStartScheduler();
  while (1)
    ;

  printf("OS should never run to here\r\n");

  while (1)
    printf("⣿⡟⠙⠛⠋⠩⠭⣉⡛⢛⠫⠭⠄⠒⠄⠄⠄⠈⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿\r\n"
           "⣿⡇⠄⠄⠄⠄⣠⠖⠋⣀⡤⠄⠒⠄⠄⠄⠄⠄⠄⠄⠄⠄⣈⡭⠭⠄⠄⠄⠉⠙\r\n"
           "⣿⡇⠄⠄⢀⣞⣡⠴⠚⠁⠄⠄⢀⠠⠄⠄⠄⠄⠄⠄⠄⠉⠄⠄⠄⠄⠄⠄⠄⠄\r\n"
           "⣿⡇⠄⡴⠁⡜⣵⢗⢀⠄⢠⡔⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"
           "⣿⡇⡜⠄⡜⠄⠄⠄⠉⣠⠋⠠⠄⢀⡄⠄⠄⣠⣆⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⢸\r\n"
           "⣿⠸⠄⡼⠄⠄⠄⠄⢰⠁⠄⠄⠄⠈⣀⣠⣬⣭⣛⠄⠁⠄⡄⠄⠄⠄⠄⠄⢀⣿\r\n"
           "⣏⠄⢀⠁⠄⠄⠄⠄⠇⢀⣠⣴⣶⣿⣿⣿⣿⣿⣿⡇⠄⠄⡇⠄⠄⠄⠄⢀⣾⣿\r\n"
           "⣿⣸⠈⠄⠄⠰⠾⠴⢾⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⢁⣾⢀⠁⠄⠄⠄⢠⢸⣿⣿\r\n"
           "⣿⣿⣆⠄⠆⠄⣦⣶⣦⣌⣿⣿⣿⣿⣷⣋⣀⣈⠙⠛⡛⠌⠄⠄⠄⠄⢸⢸⣿⣿\r\n"
           "⣿⣿⣿⠄⠄⠄⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠈⠄⠄⠄⠄⠄⠈⢸⣿⣿\r\n"
           "⣿⣿⣿⠄⠄⠄⠘⣿⣿⣿⡆⢀⣈⣉⢉⣿⣿⣯⣄⡄⠄⠄⠄⠄⠄⠄⠄⠈⣿⣿\r\n"
           "⣿⣿⡟⡜⠄⠄⠄⠄⠙⠿⣿⣧⣽⣍⣾⣿⠿⠛⠁⠄⠄⠄⠄⠄⠄⠄⠄⠃⢿⣿\r\n"
           "⣿⡿⠰⠄⠄⠄⠄⠄⠄⠄⠄⠈⠉⠩⠔⠒⠉⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠐⠘⣿\r\n"
           "⣿⠃⠃⠄⠄⠄⠄⠄⠄⣀⢀⠄⠄⡀⡀⢀⣤⣴⣤⣤⣀⣀⠄⠄⠄⠄⠄⠄⠁⢹\r\n");
  ;
}

void task1(void *pvParameters) {
  int cnt = 0;
  printf("Enter to task_1\r\n");
  while (1) {
    printf("task1 is running %d.....\r\n", cnt++);
    vTaskDelay(TASKDLYMS);
  }
}

void task2(void *pvParameters) {
  int cnt = 0;
  printf("Enter to task_2\r\n");
  while (1) {
    printf("task2 is running %d.....\r\n", cnt++);
    vTaskDelay(TASKDLYMS);
  }
}

static void vExampleTimerCallback(TimerHandle_t xTimer) {
  /* The timer has expired.  Count the number of times this happens.  The
  timer that calls this function is an auto re-load timer, so it will
  execute periodically. */
  static int cnt = 0;
  printf("timers Callback %d\r\n", cnt++);
}

void vApplicationTickHook(void) {
  // BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  /* The RTOS tick hook function is enabled by setting configUSE_TICK_HOOK to
  1 in FreeRTOSConfig.h.

  "Give" the semaphore on every 500th tick interrupt. */

  /* If xHigherPriorityTaskWoken is pdTRUE then a context switch should
  normally be performed before leaving the interrupt (because during the
  execution of the interrupt a task of equal or higher priority than the
  running task was unblocked).  The syntax required to context switch from
  an interrupt is port dependent, so check the documentation of the port you
  are using.

  In this case, the function is running in the context of the tick interrupt,
  which will automatically check for the higher priority task to run anyway,
  so no further action is required. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void) {
  /* The malloc failed hook is enabled by setting
  configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

  Called if a call to pvPortMalloc() fails because there is insufficient
  free memory available in the FreeRTOS heap.  pvPortMalloc() is called
  internally by FreeRTOS API functions that create tasks, queues, software
  timers, and semaphores.  The size of the FreeRTOS heap is set by the
  configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
  printf("malloc failed\n");
  while (1)
    ;
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  /* Run time stack overflow checking is performed if
  configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected.  pxCurrentTCB can be
  inspected in the debugger if the task name passed into this function is
  corrupt. */
  printf("Stack Overflow\n");
  while (1)
    ;
}
/*-----------------------------------------------------------*/

extern UBaseType_t uxCriticalNesting;
void vApplicationIdleHook(void) {
  // volatile size_t xFreeStackSpace;
  /* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
  FreeRTOSConfig.h.

  This function is called on each cycle of the idle task.  In this case it
  does nothing useful, other than report the amount of FreeRTOS heap that
  remains unallocated. */
  /* By now, the kernel has allocated everything it is going to, so
  if there is a lot of heap remaining unallocated then
  the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
  reduced accordingly. */
}

void print_freertos_banner() {
  // FreeRTOS welcome banner
  printf(
      "\r\n"
      "========================================================================\r\n"
      "  ______             _____ _______ ____   _____ \r\n"
      " |  ____|           |  __ \\__   __/ __ \\ / ____|\r\n"
      " | |__ _ __ ___  ___| |__) | | | | |  | | (___  \r\n"
      " |  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\ \r\n"
      " | |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\r\n"
      " |_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/ \r\n"
      "\r\n"
      "  MS1 AOCPU - FreeRTOS Demo Application\r\n"
      "========================================================================\r\n"
      "\r\n");
}

void print_mcfg_info() {
  unsigned long reg = __RV_CSR_READ(CSR_MCFG_INFO);
  printf("\nCPU Hardware mcfg INFO:\n"
         "SAFETY_MECHA:   %s\n"
         "ETRACE:         %s\n"
         "SEC_MODE:       %s\n"
         "IREGION_EXIST:  %s\n"
         "ZC_XLCZ_EXT:    %s\n"
         "DSP_N3:         %s\n"
         "DSP_N2:         %s\n"
         "DSP_N1:         %s\n"
         "SMP:            %s\n"
         "DCACHE:         %s\n"
         "ICACHE:         %s\n"
         "DLM:            %s\n"
         "ILM:            %s\n"
         "NICE:           %s\n"
         "PPI:            %s\n"
         "FIO:            %s\n"
         "PLIC:           %s\n"
         "CLIC:           %s\n"
         "ECC:            %s\n"
         "TEE:            %s\n",
         (reg & (3 << 21)) ? "Y" : "N",
         (reg & (1 << 20)) ? "Y" : "N",
         (reg & (1 << 19)) ? "Y" : "N",
         (reg & (1 << 16)) ? "Y" : "N",
         (reg & (1 << 15)) ? "Y" : "N",
         (reg & (1 << 14)) ? "Y" : "N",
         (reg & (1 << 13)) ? "Y" : "N",
         (reg & (1 << 12)) ? "Y" : "N",
         (reg & (1 << 11)) ? "Y" : "N",
         (reg & (1 << 10)) ? "Y" : "N",
         (reg & (1 << 9)) ? "Y" : "N",
         (reg & (1 << 8)) ? "Y" : "N",
         (reg & (1 << 7)) ? "Y" : "N",
         (reg & (1 << 6)) ? "Y" : "N",
         (reg & (1 << 5)) ? "Y" : "N",
         (reg & (1 << 4)) ? "Y" : "N",
         (reg & (1 << 3)) ? "Y" : "N",
         (reg & (1 << 2)) ? "Y" : "N",
         (reg & (1 << 1)) ? "Y" : "N",
         (reg & (1 << 0)) ? "Y" : "N");
}

void print_soc_status() {
  printf("\nStatus: \r\n");
  printf("    HartID    : %lu\r\n", __get_hart_id());
  printf("    BAUD_RATE : %u\n", BAUD_RATE);
  // printf("    Frequency : %dMHz\r\n", SystemCoreClock / 1000 / 1000);
  printf("    System_Image_Build_Time: %s, %s\r\n", __DATE__, __TIME__);

  printf("\nBoot: \r\n");
  printf("    Reset Addr: 0x%p (CPU entry point)\r\n", (void *)_start);
  printf("    Library   : CRT(C runtime libraries) and LIBC(Standard C Library(Newlib_Nano))...\r\n");
  printf("    Exp/Int   : Exception:non-vec, Interrupt:vec/non-vec...\r\n"); // CLINT(PLIC): Linux/SMP; CLIC(ECLIC): Real-Time/Embed
  printf("Stack: Top: 0x%p; Bottom: 0x%p; Size:0x%x\r\n"
         "Heap : End: 0x%p; Start:  0x%p; Size:0x%x\r\n",
         &__StackTop, &__StackLimit, (uintptr_t)&__StackTop - (uintptr_t)&__StackLimit,
         &__heap_limit, &__heap_start, (uintptr_t)&__heap_limit - (uintptr_t)&__heap_start);
}

void clocks_setup() {
  printf("Starting MS1 Clock Tree Configuration...\n");

  // 配置系统时钟分支A: target=sys_clk_a, source=fclk_div2, freq=166MHz
  if (clock_set_frequency(CLK_AO_SYS_CLK_A, CLK_SRC_XTAL, 24) == 0) {
    printf("✓ sys_clk_a: %u MHz (source: CLK_SRC_XTAL)\n", clock_get_frequency(CLK_AO_SYS_CLK_A));
  } else {
    printf("✗ sys_clk_a configuration failed\n");
  }

  // 配置系统时钟选择分支A: target=sys_clk, source=sys_clk_a, freq=166MHz
  if (clock_set_frequency(CLK_AO_SYS_CLK, CLK_AO_SYS_CLK_A, 24) == 0) {
    printf("✓ sys_clk: %u MHz (source: CLK_AO_SYS_CLK_A)\n", clock_get_frequency(CLK_AO_SYS_CLK));
  } else {
    printf("✗ sys_clk selection failed\n");
  }
}