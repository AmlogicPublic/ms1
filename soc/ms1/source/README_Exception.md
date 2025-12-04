# N级处理器异常处理机制

## 1. 异常响应过程

### 1.1 硬件自动行为
异常发生时,处理器硬件自动完成以下操作（单周期内同时完成）：

1. **跳转执行**：从 `mtvec` CSR 定义的 PC 地址开始执行
2. **更新 CSR 寄存器**：
   - `mcause` - 异常类型编号
   - `mdcause` - 详细异常子类型
   - `mepc` - 异常返回地址（当前异常指令 PC）
   - `mtval` - 存储器访问地址或指令编码
   - `mstatus` - 处理器状态
3. **更新 Privilege Mode**：切换到 Machine Mode
4. **更新 Machine Sub-Mode**：切换到异常处理模式

### 1.2 CSR 寄存器更新详情

#### mstatus 更新
- `mstatus.MPIE` ← 异常前的 `mstatus.MIE` 值（用于恢复）
- `mstatus.MIE` ← 0（关闭全局中断）
- `mstatus.MPP` ← 异常前的 Privilege Mode（用于恢复）

#### msubm 更新
- `msubm.PTYP` ← 异常前的 Machine Sub-Mode（用于恢复）
- `msubm.TYP` ← 异常处理模式

#### mtval 更新规则
- 存储器访问异常 → 存储访问地址
- 非法指令异常 → 指令编码
- 其他异常 → 相关值

## 2. 异常类型表

| Code | 异常类型 | 同步/异步 | 描述 | mdcause 详细类型 |
|------|---------|-----------|------|-----------------|
| 0 | 指令地址非对齐 | 同步 | 指令 PC 地址非对齐（"C"扩展中不可能发生） | - |
| 1 | 指令访问错误 | 同步 | 取指令访存错误 | 1=PMP错误, 2=总线错误 |
| 2 | 非法指令 | 同步 | 非法指令 | - |
| 3 | 断点 | 同步 | EBREAK 指令,用于调试器断点 | - |
| 4 | 读地址非对齐 | 同步 | Load 指令访存地址非对齐 | - |
| 5 | 读访问错误 | 非精确异步 | Load 指令访存错误 | 1=PMP错误, 2=总线错误, 3=NICE错误 |
| 6 | 写/AMO地址非对齐 | 同步 | Store/AMO 指令访存地址非对齐（AMO不支持非对齐） | - |
| 7 | 写/AMO访问错误 | 非精确异步 | Store/AMO 指令访存错误 | 1=PMP错误, 2=总线错误 |
| 8 | U-mode环境调用 | 同步 | User Mode 下执行 ecall 指令 | - |
| 11 | M-mode环境调用 | 同步 | Machine Mode 下执行 ecall 指令 | - |

**注意**：
- N 级处理器支持可配置的地址非对齐访问,若未开启则产生异常
- AMO 指令不支持非对齐访问

## 3. 退出异常过程

### 3.1 使用 mret 指令
软件必须使用 `mret` 指令退出异常（Machine Mode 下）。

### 3.2 硬件自动行为
执行 `mret` 后,硬件自动完成（单周期内）：

1. **跳转执行**：从 `mepc` 定义的 PC 地址开始执行
2. **更新 mstatus**：
   - `mstatus.MIE` ← `mstatus.MPIE`（恢复中断使能）
   - `mstatus.MPIE` ← 1
   - `mstatus.MPP` ← 0x0（有U-mode）或 0x3（无U-mode）
3. **恢复 Privilege Mode**：从 `mstatus.MPP` 恢复
4. **恢复 Machine Sub-Mode**：`msubm.TYP` ← `msubm.PTYP`

### 3.3 mepc 调整注意事项
对于 `ecall`/`ebreak` 指令：
- `mepc` 指向触发异常的指令本身
- 需在异常处理中手动调整：`mepc = mepc + 4`（避免死循环）

## 4. 异常服务程序

### 4.1 执行流程
1. 从 `mtvec` 地址开始执行
2. 查询 `mcause` 获取异常编号
3. 查询 `mdcause` 获取详细信息
4. 跳转到对应异常服务子程序

### 4.2 上下文保存
**关键**：硬件不自动保存/恢复上下文,需软件明确使用汇编指令完成。

## 5. 异常嵌套

N 级处理器支持**两级 NMI/异常状态堆栈**（Two Levels of NMI/Exception State Save Stacks）。

---

## 附：关键 CSR 寄存器

| CSR | 全称 | 用途 |
|-----|------|------|
| mtvec | Machine Trap Vector | 异常入口地址 |
| mcause | Machine Cause | 异常类型编号（低5位） |
| mdcause | Machine Detailed Cause | 详细异常子类型（低2位） |
| mepc | Machine Exception PC | 异常返回地址 |
| mtval | Machine Trap Value | 异常相关值（地址/指令码） |
| mstatus | Machine Status | 处理器状态 |
| msubm | Machine Sub-Mode | Machine 子模式 |

Exception Call Paths
Path 1: Synchronous Exceptions (Non-CLIC Mode) ============================================================
Exception Occurs (e.g., illegal instruction, page fault, etc.)
    ↓
CPU automatically jumps to mtvec (points to exc_entry)
    ↓
exc_entry (FreeRTOS portasm.S:172)
    ↓
SAVE_CONTEXT (saves x1,x4-x7,x10-x15,x16-x17,x28-x31 to stack)
    ↓
SAVE_CSR_CONTEXT (saves MEPC, MCAUSE to stack)
    ↓
Load mcause into a0, stack pointer into a1
    ↓
call core_intexc_handler(mcause, sp)
    ↓
core_intexc_handler() detects it's an exception (not interrupt)
    ↓
Extract exception code: exccode = (mcause & MCAUSE_CAUSE)
    ↓
Look up handler in SystemExceptionHandlers[exccode]
    ↓
If handler found: call registered exception handler
    ↓
If no handler: call system_default_exception_handler()
    ↓
system_default_exception_handler() prints debug info and halts
    ↓
RESTORE_CSR_CONTEXT (restore MEPC, MCAUSE from stack)
    ↓
RESTORE_CONTEXT (restore saved registers from stack)
    ↓
mret (return from exception)

Path 2: Synchronous Exceptions (CLIC Mode) ============================================================
Exception Occurs
    ↓
CPU jumps to exc_entry (same as above)
    ↓
Uses hardware-assisted CSR saving (CSR_PUSHMCAUSE, CSR_PUSHMEPC, CSR_PUSHMSUBM)
    ↓
call core_intexc_handler(mcause, sp)
    ↓
Same processing as Path 1
