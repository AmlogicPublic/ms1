# 中断处理模式

## 1. 非向量处理模式 (Non-Vector Mode)

### 1.1 入口地址配置

**mtvt2最低位 = 0** (默认)
- 所有非向量中断共享入口地址由`mtvec`指定（忽略最低2位）
- 异常和非向量中断共享同一入口地址

**mtvt2最低位 = 1** (推荐)
- 所有非向量中断共享入口地址由`mtvt2`指定（忽略最低2位）
- 非向量中断与异常入口地址分离,中断响应更快

### 1.2 处理流程

```assembly
common_entry:
    # 1. 保存CSR寄存器
    <保存mepc入堆栈>
    <保存mcause入堆栈>
    <保存msubm入堆栈>
    
    # 2. 保存上下文
    <保存通用寄存器入堆栈>  # RV32E: 8个寄存器, RV32I: 16个寄存器
    
    # 3. 特殊指令 - 跳转到ISR并支持嵌套/咬尾
    csrrw ra, CSR_JALMNXTI, ra
    
    # 4. 恢复上下文
    <从堆栈恢复通用寄存器>
    <重新关闭中断全局使能>
    <从堆栈恢复msubm>
    <从堆栈恢复mcause>
    <从堆栈恢复mepc>
    
    # 5. 返回
    mret
```

**csrrw ra, CSR_JALMNXTI, ra 指令行为:**
- 无中断Pending：相当于NOP
- 有中断Pending：
  - 跳转到中断向量表对应的ISR入口地址
  - 硬件设置`mstatus.MIE = 1` (打开全局中断,支持嵌套)
  - 设置返回地址为该指令PC (JAL效果,支持咬尾)

### 1.3 响应延迟

总延迟 = 硬件响应(~4周期) + 保存CSR(若干周期) + 保存上下文(RV32E: 8个寄存器, RV32I: 16个寄存器) + 跳转ISR(~5周期)

### 1.4 中断嵌套

**总是支持** - 高优先级中断可打断低优先级中断

```
主程序
  └─> 中断30 (低优先级)
      <保存上下文>
      └─> 中断31 (中优先级)
          <保存上下文>
          └─> 中断32 (高优先级)
              <保存上下文>
              <执行ISR>
              <恢复上下文>
          └─> 返回中断31
              <执行ISR剩余部分>
              <恢复上下文>
      └─> 返回中断30
          <执行ISR剩余部分>
          <恢复上下文>
  └─> 返回主程序
```

### 1.5 中断咬尾 (Tail-chaining)

**节省背靠背的保存/恢复上下文开销**

当ISR返回时,`csrrw ra, CSR_JALMNXTI, ra`重新执行：
- 检测到Pending中断 → 直接跳转下一个ISR,**省略恢复和保存上下文**
- 无Pending中断 → 继续执行恢复上下文

```
主程序
  └─> 中断30 (高优先级)
      <保存上下文>
      <执行ISR>
      ├─> 中断29 (中优先级) Pending
      │   <直接跳转,无需恢复/保存上下文>
      │   <执行ISR>
      │   ├─> 中断28 (低优先级) Pending
      │   │   <直接跳转,无需恢复/保存上下文>
      │   │   <执行ISR>
      │   │   <恢复上下文>
      <恢复上下文>
  └─> 返回主程序
```

---

## 2. 向量处理模式 (Vector Mode)

### 2.1 特点

- **直接跳转ISR** - 硬件查向量表后直接跳入中断服务程序
- **极低延迟** - 理想情况约6个时钟周期（无上下文保存）
- **默认不支持嵌套** - 硬件自动设置`mstatus.MIE = 0`
- **无咬尾支持** - 由于无上下文保存,咬尾意义不大

### 2.2 ISR函数要求

```c
void __attribute__((interrupt)) Interrupt_30_handler(void) {
    // 执行中断服务程序
}
```

**编译器行为:**
- ISR是Leaf Function（不调用子函数）→ 编译器不插入额外代码
- ISR调用了子函数 → 编译器自动插入上下文保存代码
  - ⚠️ 保证功能正确但增加延迟和代码尺寸
  - ❌ 不推荐在向量模式ISR中调用子函数

### 2.3 支持中断嵌套（需手动处理）

```c
void __attribute__((interrupt)) Interrupt_30_handler(void) {
    // 1. 保存关键CSR
    <保存mepc入堆栈>
    <保存mcause入堆栈>
    <保存msubm入堆栈>
    
    // 2. 打开全局中断使能
    <设置mstatus.MIE = 1>
    
    // 3. 执行中断服务
    <执行中断服务程序内容>
    
    // 4. 关闭全局中断使能（保证原子性）
    <设置mstatus.MIE = 0>
    
    // 5. 恢复CSR
    <从堆栈恢复msubm>
    <从堆栈恢复mcause>
    <从堆栈恢复mepc>
    
    // 6. 返回
    mret
}
```

**嵌套示例:**
```
主程序
  └─> 中断30 (低优先级)
      <保存CSR, 打开MIE>
      └─> 中断31 (中优先级)
          <保存CSR, 打开MIE>
          └─> 中断32 (高优先级)
              <保存CSR, 打开MIE>
              <执行ISR>
              <关闭MIE, 恢复CSR>
          └─> 返回中断31
              <执行ISR剩余部分>
              <关闭MIE, 恢复CSR>
      └─> 返回中断30
          <执行ISR剩余部分>
          <关闭MIE, 恢复CSR>
  └─> 返回主程序
```

---

## 3. 两种模式对比

| 特性 | 非向量处理模式 | 向量处理模式 |
|------|---------------|-------------|
| **入口地址** | 所有非向量中断共享入口 | 每个中断独立入口 |
| **响应延迟** | 较长（保存上下文+跳转） | 极短（~6周期） |
| **中断嵌套** | 总是支持 | 默认不支持,需手动处理 |
| **中断咬尾** | 支持,节省上下文切换 | 不支持 |
| **ISR函数** | 普通函数 | 需`__attribute__((interrupt))` |
| **子函数调用** | 支持 | 不推荐（增加延迟） |
| **适用场景** | 需要嵌套和咬尾优化 | 追求极致响应速度 |

---

## 4. 关键CSR寄存器

| 寄存器 | 功能 |
|--------|------|
| `mtvec` | 异常入口地址；mtvt2[0]=0时也是非向量中断入口 |
| `mtvt2` | mtvt2[0]=1时指定非向量中断入口地址（推荐） |
| `mstatus.MIE` | 全局中断使能位 |
| `mepc` | 异常/中断返回PC |
| `mcause` | 异常/中断原因 |
| `msubm` | 子模式寄存器 |
| `CSR_JALMNXTI` | 特殊CSR,配合csrrw指令实现嵌套和咬尾 |

---

*文档基于芯来科技N200系列RISC-V处理器中断机制*

+----------------------+-----------------------------+------------------------+------------------------+------------------------+-----------------------------+
| Aspect               | CLINT/PLIC Mode             | ECLIC Vectored         | ECLIC Non-Vectored     | Exception Mode         | Supervisor Exception Mode   |
+----------------------+-----------------------------+------------------------+------------------------+------------------------+-----------------------------+
| Hardware Required    | CLINT or PLIC peripheral    | ECLIC peripheral       | ECLIC peripheral       | None (CPU built-in)    | TEE extension + ECLIC       |
| Trigger Type         | Asynchronous events only    | Asynchronous events    | Asynchronous events    | Synchronous events     | Synchronous events          |
| Entry Point          | exc_entry (shared)          | Direct to vector table | irq_entry              | exc_entry              | exc_entry_s                 |
| Context Saving       | Software (SAVE_CONTEXT)     | Direct handler call    | Software (SAVE_CONTEXT)| Software (SAVE_CONTEXT)| Software (SAVE_CONTEXT)     |
| CSR Saving           | MEPC, MCAUSE                | Auto via ECLIC         | MEPC, MCAUSE           | MEPC, MCAUSE           | SEPC, SCAUSE                |
| Handler Lookup       | Array lookup:               | Direct jump via        | CSR_JALMNXTI claim     | Array lookup:          | Array lookup:               |
|                      | SystemCoreInterruptHandlers | MTVEC table            |                        | SystemExceptionHandlers| SystemExceptionHandlers_S   |
| Performance          | Medium (array lookup)       | Highest (direct jump)  | High (hardware claim)  | Medium (array lookup)  | Medium (array lookup)       |
| Interrupt Latency    | 20–50 cycles                | 5–15 cycles            | 10–25 cycles           | N/A                    | N/A                         |
| Memory Usage         | Low (handler array)         | Medium (vector table)  | Low (handler array)    | Low (handler array)    | Low (handler array)         |
| Code Size Impact     | Small                       | Medium                 | Small                  | Small                  | Small (when TEE enabled)    |
| Priority Support     | Software managed            | Hardware + Software    | Hardware + Software    | N/A                    | N/A                         |
| FreeRTOS Integration | eclic_msip_handler          | Direct integration     | eclic_msip_handler     | N/A                    | N/A                         |
| CSR Configuration    | mtvec = exc_entry           | mtvec = vector_base    | mtvec = exc_entry | 0x3| mtvec = exc_entry      | stvec = exc_entry_s         |
| Typical Use Cases    | Basic interrupt handling    | High-performance       | Complex priority       | CPU faults/debugging   | Secure world exceptions     |
|                      |                             | interrupts             | handling               |                        |                             |
| Configuration        | Interrupt_Init() →          | ECLIC_Interrupt_Init() | ECLIC_Interrupt_Init() | Exception_Init()       | Auto when TEE present       |
|                      | CLINT/PLIC                  | + vector setup         |                        |                        |                             |
| Handler Registration | Interrupt_Register_CoreIRQ()| ECLIC_Register_IRQ()   | ECLIC_Register_IRQ()   | Exception_Register_EXC()| Exception_Register_EXC_S() |
| Default Handler      | system_default_interrupt_   | default_intexc_vector_handler | system_default_        | system_default_        | system_default_             |
|                      | handler                     |                        | interrupt_handler      | exception_handler      | exception_handler_s         |
| Recovery Mechanism   | mret                        | mret                   | mret                   | mret                   | sret                        |
| Debugging Support    | Basic CSR dumps             | Basic CSR dumps        | Basic CSR dumps        | Full CSR dumps + stack | Full CSR dumps + stack trace|
|                      |                             |                        |                        | trace                  |                             |
| Power Management     | Standard                    | Standard               | Standard               | N/A                    | Enhanced (secure world)     |
| Security Features    | None                        | None                   | None                   | None                   | Memory isolation, secure    |
|                      |                             |                        |                        |                        | interrupts                  |
+----------------------+-----------------------------+------------------------+------------------------+------------------------+-----------------------------+

