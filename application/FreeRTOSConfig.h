/*
    FreeRTOS Kernel V10.3.1
    Configuration file for FreeRTOS on a RISC-V system.
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* General FreeRTOS Configuration */
#define USER_MODE_TASKS                         0  // 0: Tasks run in privileged mode, 1: User mode.

#define configUSE_PREEMPTION                    1  // Enable preemptive scheduling.
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0  // Use optimized task selection if supported.
#define configUSE_TICKLESS_IDLE                 0  // Disable tickless idle power-saving mode.
#define configCPU_CLOCK_HZ                      SystemCoreClock // CPU clock frequency.
#define configRTC_CLOCK_HZ                      12000000  // Real-Time Clock frequency.
#define configTICK_RATE_HZ                      100  // Tick frequency (100Hz = 10ms per tick).
#define configMAX_PRIORITIES                    4  // Maximum number of task priorities.
#define configMINIMAL_STACK_SIZE                256  // Minimum stack size for a task.
#define configMAX_TASK_NAME_LEN                 16  // Maximum length of task names.
#define configUSE_16_BIT_TICKS                  0  // Use 32-bit tick counter.
#define configIDLE_SHOULD_YIELD                 0  // Idle task should not yield CPU.
#define configUSE_TASK_NOTIFICATIONS            1  // Enable task notifications.
#define configUSE_MUTEXES                       1  // Enable mutex support.
#define configUSE_RECURSIVE_MUTEXES             0  // Disable recursive mutexes.
#define configUSE_COUNTING_SEMAPHORES           1  // Enable counting semaphores.
#define configQUEUE_REGISTRY_SIZE               10  // Max number of queue registry entries.
#define configUSE_QUEUE_SETS                    0  // Disable queue sets.
#define configUSE_TIME_SLICING                  1  // Enable time-slicing for tasks of equal priority.
#define configUSE_NEWLIB_REENTRANT              0  // Disable Newlib reentrancy support.
#define configENABLE_BACKWARD_COMPATIBILITY     0  // Disable backward compatibility macros.
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5  // Number of TLS pointers per task.

/* Memory Allocation Configuration */
#define configSUPPORT_STATIC_ALLOCATION         0  // Disable static allocation.
#define configSUPPORT_DYNAMIC_ALLOCATION        1  // Enable dynamic allocation.
#define configTOTAL_HEAP_SIZE                   16*1024  // FreeRTOS heap (in BSS) for task stacks
#define configAPPLICATION_ALLOCATED_HEAP        0  // FreeRTOS manages heap allocation.

/* Hook Function Configuration */
#define configUSE_IDLE_HOOK                     1  // Enable idle task hook.
#define configUSE_TICK_HOOK                     0  // Disable tick hook.
#define configCHECK_FOR_STACK_OVERFLOW          1  // Enable stack overflow checking.
#define configUSE_MALLOC_FAILED_HOOK            1  // Enable malloc failure hook.
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0  // Disable daemon task startup hook.

/* Runtime and Task Statistics Configuration */
#define configGENERATE_RUN_TIME_STATS           0  // Disable runtime stats.
#define configUSE_TRACE_FACILITY                0  // Disable trace facility.
#define configUSE_STATS_FORMATTING_FUNCTIONS    0  // Disable stats formatting functions.

/* Co-routine Configuration */
#define configUSE_CO_ROUTINES                   0  // Disable co-routines.
#define configMAX_CO_ROUTINE_PRIORITIES         1  // Set max priority for co-routines.

/* Software Timer Configuration */
#define configUSE_TIMERS                        1  // Enable software timers.
#define configTIMER_TASK_PRIORITY               3  // Timer task priority.
#define configTIMER_QUEUE_LENGTH                5  // Timer queue length.
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE  // Timer task stack size.

/* Interrupt Configuration */
#define configKERNEL_INTERRUPT_PRIORITY         0  // Kernel interrupt priority.
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    7  // Max syscall interrupt priority.

/* Development Debugging */
#define configASSERT( x ) if( ( x ) == 0 ) {taskDISABLE_INTERRUPTS(); for( ;; );}  // Trap errors.

/* FreeRTOS MPU specific definitions. */
//#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/* Optional API Function Inclusion */
#define INCLUDE_vTaskPrioritySet                1  // set the priority of a task, used for task priority management.
#define INCLUDE_uxTaskPriorityGet               1  // get the current priority of a task, useful for task scheduling.
#define INCLUDE_vTaskDelete                     1  // delete a task, allowing for dynamic removal of tasks from the system.
#define INCLUDE_vTaskSuspend                    1  // suspend a task, temporarily halting its execution.
#define INCLUDE_xResumeFromISR                  1  // resume a suspended task from an Interrupt Service Routine (ISR).
#define INCLUDE_vTaskDelayUntil                 1  // delay a task until a specific time, useful for precise task timing.
#define INCLUDE_vTaskDelay                      1  // delay a task for a specific duration, typically used for time-based task management.
#define INCLUDE_xTaskGetSchedulerState          1  // get the current state of the scheduler, useful for diagnostics and system state tracking.
#define INCLUDE_xTaskGetCurrentTaskHandle       1  // get the handle of the currently running task, useful for task-specific management.
#define INCLUDE_uxTaskGetStackHighWaterMark     1  // get the high-water mark of a task's stack, helpful for stack usage monitoring.
#define INCLUDE_xTaskGetIdleTaskHandle          1  // get the handle of the idle task, useful for system management and diagnostics.
#define INCLUDE_eTaskGetState                   0  // get the state of a task, as it is not required for this configuration.
#define INCLUDE_xEventGroupSetBitFromISR        1  // set a bit in an event group from an ISR, useful for signaling event groups in an interrupt context.
#define INCLUDE_xTimerPendFunctionCall          1  // pend a function call for execution in the timer task, useful for deferred function calls.
#define INCLUDE_xTaskAbortDelay                 0  // abort a task's delay, as it is not needed for this configuration.
#define INCLUDE_xTaskGetHandle                  1  // get a handle to a specific task, used for task management and interaction.
#define INCLUDE_xTaskResumeFromISR              1  // resume a task from an ISR, useful for task resumption in interrupt contexts.


/* A header file that defines trace macro can be included here. */

#endif /* FREERTOS_CONFIG_H */
