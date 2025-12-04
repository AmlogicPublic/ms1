# MS1 Stack Design - Simplified Architecture

## Overview

This document describes the **simplified stack architecture** for MS1 SoC, designed for clarity, safety, and ease of debugging.

## Design Philosophy

**Extreme Simplification**: 
- No complex stack switching
- No separate interrupt stack
- Direct register save on current stack
- Same behavior in Boot and FreeRTOS phases

## Memory Layout

```
RAM Memory Map:
┌─────────────────────────────────┐
│ .text, .rodata (Flash/ILM)      │
├─────────────────────────────────┤
│ .data (initialized globals)     │
├─────────────────────────────────┤
│ .bss (uninitialized globals)    │
├─────────────────────────────────┤
│ heap (malloc)                   │  ← __heap_start to __heap_limit
├─────────────────────────────────┤
│ [gap for heap growth]           │
├─────────────────────────────────┤
│ Stack (4KB default)             │  ← __StackLimit to __StackTop (_sp)
│ ↓ grows downward                │     Shared by main & interrupts
├─────────────────────────────────┤
│ FreeRTOS heap (if enabled)      │  ← Task stacks allocated here
└─────────────────────────────────┘
```

## Stack Usage by Phase

### Boot Phase (Before FreeRTOS)

**Single Shared Stack** (`_sp` = 4KB):

```
High Address: _sp (Stack Top)
       ↓
    [main() stack frame ~500 bytes]
       ↓ interrupt occurs
    [saved registers 128 bytes]      ← SAVE_CONTEXT
       ↓
    [irq_handler() stack frame]      ← C function calls
       ↓
Low Address: _sp - 4KB
```

**Behavior**:
- `main()` uses stack normally
- Interrupt: save registers directly on current `sp`
- No stack switching (`csrrw sp, mscratch, sp` NOT used)
- Stack grows continuously downward

**Pros**:
- ✅ Extremely simple
- ✅ Easy to debug (GDB shows full call chain)
- ✅ No mscratch management needed

**Cons**:
- ⚠️ Must ensure: main_depth + interrupt_depth < 4KB
- ⚠️ Stack overflow immediately visible (good for debugging!)

### FreeRTOS Phase (After Scheduler Starts)

**Multiple Stacks**:

1. **Task Stacks** (in FreeRTOS heap):
   - Each task: dedicated stack (configurable size)
   - Allocated by `xTaskCreate()`
   
2. **Boot Stack** (4KB) - **becomes interrupt stack**:
   - No longer used by tasks
   - Used only by interrupts

```
Task Running:
High Address: task_stack_top
       ↓
    [task stack frame]
       ↓ interrupt occurs
    [saved registers 128 bytes]      ← SAVE_CONTEXT on task stack!
       ↓
    [irq_handler() stack frame]      ← continues on task stack
       ↓
Low Address

Note: Interrupts save on CURRENT stack (task's stack, not boot stack)
```

**Transition**:
- `vTaskStartScheduler()` called
- `prvPortStartFirstTask()` loads first task
- Boot stack (`_sp`) no longer used by tasks
- Interrupts still save on current stack (now task's stack)

## Code Implementation

### Linker Script (All 3 variants identical)

```ld
.stack : ALIGN(16) {
    PROVIDE( __StackLimit = . );
    PROVIDE( __StackBottom = . );
    
    . += __TOT_STACK_SIZE;    /* 4KB */
    . = ALIGN(16);
    
    PROVIDE( __StackTop = . );
    PROVIDE( _sp = . );
    
    /* Simplified: No _sp_main, _sp_irq - all use _sp */
} >RAM
```

### Startup Code

```assembly
/* startup_from_sram.S / startup_from_flash.S */
_start:
    /* Initialize stack pointer */
    la sp, _sp                    /* Use full stack */
    
    /* NO mscratch setup needed */
    /* Interrupts save directly on sp */
    
    call SystemInit
    call main
```

### Interrupt Handler (portasm.S)

```assembly
.macro SAVE_CONTEXT
    /* Simplified: Direct save on current stack */
    addi sp, sp, -32*REGBYTES    /* Allocate 128 bytes */
    STORE x1, 0(sp)              /* Save all registers */
    ...
.endm

.macro RESTORE_CONTEXT
    LOAD x1, 0(sp)               /* Restore all registers */
    ...
    addi sp, sp, 32*REGBYTES     /* Deallocate 128 bytes */
.endm

irq_entry:
    SAVE_CONTEXT                 /* No csrrw! */
    call clear_eclic_interrupt
    call core_intexc_handler
    RESTORE_CONTEXT              /* No csrrw! */
    mret
```

### FreeRTOS Integration

```assembly
/* portasm.S - prvPortStartFirstTask */
prvPortStartFirstTask:
    /* Simplified: No mscratch setup */
    
    LOAD t0, pxCurrentTCB        /* Get first task TCB */
    LOAD sp, 0(t0)               /* Load task stack pointer */
    
    /* Restore task context and start */
    RESTORE_CONTEXT
    mret
```

## Stack Size Guidelines

### Boot Phase (main + interrupts share 4KB)

**Calculation**:
```
Total = main_depth + interrupt_overhead + safety_margin

Where:
- main_depth: deepest call chain in main()
  Example: main → init → setup → config → write_reg
  ~10 levels × 32 bytes = 320 bytes

- interrupt_overhead: 
  * Saved registers: 128 bytes
  * IRQ handler stack: ~200 bytes
  * Nested interrupt: +328 bytes per level
  Total: ~328 bytes (single level)

- safety_margin: 50% recommended
  Example: (320 + 328) × 1.5 = 972 bytes

Recommended: 4KB provides comfortable headroom
```

### FreeRTOS Phase

**Task Stacks** (per task):
```c
xTaskCreate(
    taskFunction,
    "TaskName",
    512,           // Stack size in words (2KB for 32-bit)
    ...
);

Guidelines:
- Idle task: 128 words (512 bytes)
- Simple task: 256 words (1KB)
- Complex task: 512 words (2KB)
- Plus interrupt overhead on task stack!
```

**Interrupt on Task Stack**:
```
Task stack must accommodate:
- Task's own usage
- Interrupt overhead (128 + handler)
- Nested interrupts if enabled

Example:
  Task needs: 1KB
  Interrupt: 328 bytes
  → Allocate: 2KB (512 words)
```

## Debugging Stack Issues

### Detecting Stack Overflow

**At Runtime**:
```c
#include <stdio.h>

extern char __StackLimit, __StackTop;

void check_stack_usage(void) {
    volatile char stack_probe;
    size_t used = (size_t)(&__StackTop - (char*)&stack_probe);
    size_t total = (size_t)(&__StackTop - &__StackLimit);
    
    printf("Stack: %u / %u bytes (%.1f%%)\n", 
           used, total, 100.0 * used / total);
    
    if (used > total * 0.9) {
        printf("WARNING: Stack usage >90%%!\n");
    }
}
```

**In GDB**:
```gdb
# Check current stack usage
p/x $sp
p/x &__StackTop
p/x &__StackLimit

# Verify stack is in valid range
p/d (&__StackTop - $sp)
```

**Linker Check** (optional in .ld):
```ld
ASSERT(__StackTop - __StackLimit >= 0x1000, "ERROR: Stack too small")
```

### Common Issues

**Symptom**: Illegal instruction, random crashes after interrupt
**Cause**: Stack overflow, corrupted return address
**Fix**: 
1. Call `check_stack_usage()` regularly
2. Reduce main() call depth
3. Increase `__STACK_SIZE` in linker script

**Symptom**: Hard fault in interrupt
**Cause**: Interrupt handler stack usage too large
**Fix**:
1. Simplify interrupt handler
2. Move complex logic to task (use queue)

## Comparison: Old vs New Design

| Aspect | Old Design (Complex) | New Design (Simplified) |
|--------|---------------------|------------------------|
| **Stack Count** | 2+ (main, irq, tasks) | 1 shared + task stacks |
| **Stack Switching** | `csrrw sp, mscratch, sp` | None |
| **Boot Stack Usage** | Split: 2KB main, 2KB irq | Shared: 4KB |
| **mscratch Setup** | Required, phase-dependent | Not used |
| **Code Complexity** | High (swap logic) | Minimal |
| **Debugging** | Difficult (discontinuous stack) | Easy (continuous stack) |
| **Performance** | 2 extra instructions | Direct |
| **Safety** | Complex to verify | Stack overflow obvious |

## Configuration

### Adjusting Stack Size

**In linker script** (ms1_sram_boot.ld, ms1_flash_boot.ld, ms1_ilm_boot.ld):

```ld
/* Default: 4KB total stack */
__STACK_SIZE = 0x1000;         /* 4KB per core */

/* For complex applications: */
__STACK_SIZE = 0x2000;         /* 8KB per core */

/* For minimal applications: */
__STACK_SIZE = 0x800;          /* 2KB per core */
```

### Multi-Core Considerations

For SMP systems (`SMP_CPU_CNT > 1`):

```assembly
/* Each core gets its own stack */
__TOT_STACK_SIZE = __STACK_SIZE * SMP_CPU_CNT;

/* Core 0: _sp
   Core 1: _sp - __STACK_SIZE
   Core 2: _sp - 2*__STACK_SIZE
   ... */
```

## Summary

**Key Points**:
1. ✅ **One shared stack** for boot phase (main + interrupts)
2. ✅ **No stack switching** - interrupts save on current stack
3. ✅ **FreeRTOS tasks** get dedicated stacks from FreeRTOS heap
4. ✅ **Simple code** - easy to understand and debug
5. ⚠️ **Monitor stack usage** to prevent overflow

**When to Choose This Design**:
- Boot phase has simple main() function
- Easy debugging more important than micro-optimization
- Want clean, maintainable code
- Stack overflow should be immediately visible

**This is the recommended design for MS1 SoC applications.**
