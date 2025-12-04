# N级别处理器内核支持的 CSR 寄存器列表

## RISC-V 标准 CSR (Machine Mode)

| CSR 地址 | 读写属性 | 名称 | 全称 |
|---------|---------|------|------|
| 0xF11 | MRO | mvendorid | 商业供应商编号寄存器（Machine Vendor ID Register） |
| 0xF12 | MRO | marchid | 架构编号寄存器（Machine Architecture ID Register） |
| 0xF13 | MRO | mimpid | 硬件实现编号寄存器（Machine Implementation ID Register） |
| 0xF14 | MRO | mhartid | Hart 编号寄存器（Hart ID Register） |
| 0x300 | MRW | mstatus | 异常处理状态寄存器 |
| 0x301 | MRW | misa | 指令集架构寄存器（Machine ISA Register） |
| 0x304 | MRW | mie | 局部中断屏蔽控制寄存器（Machine Interrupt Enable Register） |
| 0x305 | MRW | mtvec | 异常入口基地址寄存器 |
| 0x307 | MRW | mtvt | ECLIC 中断向量表的基地址 |
| 0x340 | MRW | mscratch | 暂存寄存器（Machine Scratch Register） |
| 0x341 | MRW | mepc | 异常 PC 寄存器（Machine Exception Program Counter） |
| 0x342 | MRW | mcause | 异常原因寄存器（Machine Cause Register） |
| 0x343 | MRW | mtval | 异常值寄存器（Machine Trap Value Register） |
| 0x344 | MRW | mip | 中断等待寄存器（Machine Interrupt Pending Register） |
| 0x345 | MRW | mnxti | 标准寄存器用于使能中断,处理下一个中断并返回下一个中断的 Handler 入口地址 |
| 0x346 | MRW | mintstatus | 标准寄存器用于保存当前中断 Level |
| 0x348 | MRW | mscratchcsw | 标准寄存器用于在特权模式变化时交换 mscratch 与目的寄存器的值 |
| 0x349 | MRW | mscratchcswl | 标准寄存器用于在中断 Level 变化时交换 mscratch 与目的寄存器的值 |
| 0xB00 | MRW | mcycle | 周期计数器的低 32 位（Lower 32 bits of Cycle counter） |
| 0xB80 | MRW | mcycleh | 周期计数器的高 32 位（Upper 32 bits of Cycle counter） |
| 0xB02 | MRW | minstret | 完成指令计数器的低 32 位（Lower 32 bits of Instructions-retired counter） |
| 0xB82 | MRW | minstreth | 完成指令计数器的高 32 位（Upper 32 bits of Instructions-retired counter） |
| N/A | MRW | mtime | 计时器寄存器（Machine-mode timer register） |
| N/A | MRW | mtimecmp | 计时器比较寄存器（Machine-mode timer compare register） |
| N/A | MRW | msip | 机器模式软件中断等待寄存器（Machine-mode Software Interrupt Pending Register） |
| N/A | MRW | mtimectl | 用于停止计时器,切换计时时钟,控制 TIMER 中断发生时,计数器是否自动清零 |
| 0x3A0+x | MRW | pmpcfg&lt;x&gt; | PMP 表项权限配置寄存器 |
| 0x3B0+x | MRW | pmpaddr&lt;x&gt; | PMP 表项 x 的地址配置寄存器 |

## RISC-V 标准 CSR (User Mode)

| CSR 地址 | 读写属性 | 名称 | 全称 |
|---------|---------|------|------|
| 0x001 | URW | fflags | 浮点累积异常（Floating-Point Accrued Exceptions）<br>注意：此寄存器只有在配置了浮点指令（"F"或者"D"指令子集）时才会存在 |
| 0x002 | URW | frm | 浮点动态舍入模式（Floating-Point Dynamic Rounding Mode）<br>注意：此寄存器只有在配置了浮点指令（"F"或者"D"指令子集）时才会存在 |
| 0x003 | URW | fcsr | 浮点控制和状态寄存器（Floating-Point Control and Status Register）<br>注意：此寄存器只有在配置了浮点指令（"F"或者"D"指令子集）时才会存在 |
| 0xC00 | URO | cycle | mcycle 寄存器的只读副本<br>注意：该寄存器在 User Mode 下是否可读由 CSR 寄存器 mcounteren 的 CY 比特域来控制 |
| 0xC01 | URO | time | mtime 寄存器的只读副本<br>注意：该寄存器在 User Mode 下是否可读由 CSR 寄存器 mcounteren 的 TM 比特域来控制 |
| 0xC02 | URO | instret | minstret 寄存器的只读副本<br>注意：该寄存器在 User Mode 下是否可读由 CSR 寄存器 mcounteren 的 IR 比特域来控制 |
| 0xC80 | URO | cycleh | mcycleh 寄存器的只读副本<br>注意：该寄存器在 User Mode 下是否可读由 CSR 寄存器 mcounteren 的 CY 比特域来控制 |
| 0xC81 | URO | timeh | mtimeh 寄存器的只读副本<br>注意：该寄存器在 User Mode 下是否可读由 CSR 寄存器 mcounteren 的 TM 比特域来控制 |
| 0xC82 | URO | instreth | minstreth 寄存器的只读副本<br>注意：该寄存器在 User Mode 下是否可读由 CSR 寄存器 mcounteren 的 IR 比特域来控制 |
| 0x801 | URW | ucode | DSP 运算溢出标志寄存器<br>注意：此寄存器只有在配置了"P"扩展指令集时才会存在 |

## N 级别处理器内核自定义 CSR

| CSR 地址 | 读写属性 | 名称 | 全称 |
|---------|---------|------|------|
| 0x320 | MRW | mcountinhibit | 自定义寄存器用于控制计数器的开启和关闭 |
| 0x7c0 | MRW | milm_ctl | ILM 地址空间的打开或关闭 |
| 0x7c1 | MRW | mdlm_ctl | DLM 地址空间的打开或关闭 |
| 0x7c2 | MRW | mecc_code | ECC 校验码注入寄存器,可用于模拟 ECC 错误 |
| 0x7c3 | MRW | mnvec | NMI 处理入口基地址寄存器 |
| 0x7c4 | MRW | msubm | 自定义寄存器用于保存 Core 当前的 Trap 类型,以及进入 Trap 前的 Trap 类型 |
| 0x7c9 | MRW | mdcause | 自定义寄存器,作为 mcause 的补充,用于保存详细的异常原因 |
| 0x7ca | MRW | mcache_ctl | 自定义寄存器用来控制 I-CACHE 相关功能<br>注意：此寄存器只有配置了 I-CACHE 才会存在 |
| 0x7d0 | MRW | mmisc_ctl | 自定义寄存器用于控制 NMI,Misaligned Access 和 BPU 的相关功能 |
| 0x7d6 | MRW | msavestatus | 自定义寄存器用于保存 mstatus 值<br>注意：此寄存器只有配置了两级异常嵌套恢复才会存在 |
| 0x7d7 | MRW | msaveepc1 | 自定义寄存器用于保存第一级嵌套 NMI 或异常的 mepc<br>注意：此寄存器只有配置了两级异常嵌套恢复才会存在 |
| 0x7d8 | MRW | msavecause1 | 自定义寄存器用于保存第一级嵌套 NMI 或异常的 mcause<br>注意：此寄存器只有配置了两级异常嵌套恢复才会存在 |
| 0x7d9 | MRW | msaveepc2 | 自定义寄存器用于保存第二级嵌套 NMI 或异常的 mepc<br>注意：此寄存器只有配置了两级异常嵌套恢复才会存在 |
| 0x7da | MRW | msavecause2 | 自定义寄存器用于保存第二级嵌套 NMI 或异常的 mcause<br>注意：此寄存器只有配置了两级异常嵌套恢复才会存在 |
| 0x7db | MRW | msavedcause1 | 自定义寄存器用于保存第一级嵌套异常的 mdcause<br>注意：此寄存器只有配置了两级异常嵌套恢复才会存在 |
| 0x7dc | MRW | msavedcause2 | 自定义寄存器用于保存第二级嵌套异常的 mdcause<br>注意：此寄存器只有配置了两级异常嵌套恢复才会存在 |
| 0x7de | MRW | mecc_lock | 用于锁定 ECC 的配置,所有相关的 ECC 寄存器均不能再修改,除非复位 |
| 0x7eb | MRW | pushmsubm | 自定义寄存器用于将 msubm 的值存入堆栈地址空间 |
| 0x7ec | MRW | mtvt2 | 自定义寄存器用于设定非向量中断处理模式的中断入口地址 |
| 0x7ed | MRW | jalmnxti | 自定义寄存器用于使能 ECLIC 中断,该寄存器的读操作能处理下一个中断同时返回下一个中断 Handler 的入口地址,并跳转至此地址 |
| 0x7ee | MRW | pushmcause | 自定义寄存器用于将 mcause 的值存入堆栈地址空间 |
| 0x7ef | MRW | pushmepc | 自定义寄存器用于将 mepc 的值存入堆栈地址空间 |
| 0x7f0 | MRO | mppicfg_info | PPI 配置信息 |
| 0x7f1 | MRO | mfiocfg_info | FIO 配置信息 |
| 0x811 | URW | sleepvalue | WFI 的休眠模式寄存器 |
| 0x812 | URW | txevt | 发送 Event 寄存器 |
| 0x810 | URW | wfe | Wait for Event 控制寄存器 |
| 0xfc0 | MRO | micfg_info | ILM and I-Cache 配置信息 |
| 0xfc1 | MRO | mdcfg_info | DLM and D-Cache 配置信息 |
| 0xfc2 | MRO | mcfg_info | 处理器配置信息 |

## 读写属性说明

- **MRW**: Machine Mode Readable/Writeable
- **MRO**: Machine Mode Read-Only
- **URW**: User Mode Readable/Writeable
- **URO**: User Mode Read-Only

