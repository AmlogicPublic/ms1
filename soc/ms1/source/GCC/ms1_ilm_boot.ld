/*
+--------------+----------------------+----------------------------+------------------------------------------+--------------------------------------------------------------+
| Region       | Section              | Attributes                 | Address (Load / Execution)               | Size / Usage                                                 |
+--------------+----------------------+----------------------------+------------------------------------------+--------------------------------------------------------------+
| ilm          | .init                | r-x                        | ILM_MEMORY_BASE                          | Initialization code (startup_from_ilm.S)                     |
|              | .text                | r-x                        | ADDR(.text)                              | Main executable code (already in ILM)                        |
+--------------+----------------------+----------------------------+------------------------------------------+--------------------------------------------------------------+
| dlm          | .data                | rw-                        | ADDR(.data)                              | Initialized data (already in DLM)                            |
|              | .tdata               | rw-                        | ADDR(.tdata)                             | Thread-local initialized data (already in DLM)               |
|              | .tbss                | rw- (uninitialized)        | ADDR(.tbss)                              | Thread-local uninitialized data                              |
|              | .bss                 | rw- (uninitialized)        | ADDR(.bss)                               | Uninitialized data                                           |
|              | .heap                | rw- (uninitialized)        | ADDR(.heap)                              | Heap memory                                                  |
|              | .stack               | rw- (uninitialized)        | ORIGIN(dlm) + LENGTH(dlm) - __TOT_STACK_SIZE | Stack memory                                             |
+--------------+----------------------+----------------------------+------------------------------------------+--------------------------------------------------------------+
| flash        |                      |                            | FLASH_MEMORY_BASE                        | Not used for execution (optional storage)                    |
+--------------+----------------------+----------------------------+------------------------------------------+--------------------------------------------------------------+
| sram         |                      |                            | SRAM_MEMORY_BASE                         | Reserved for other masters (e.g., USB)                       |
+--------------+----------------------+----------------------------+------------------------------------------+--------------------------------------------------------------+
*/

OUTPUT_ARCH( "riscv" )

ENTRY( _start )

INCLUDE ms1.memory

/*
  ILM Boot Configuration:
  - Boot directly from ILM memory with startup_from_ilm.S
  - Code and data are already pre-loaded in ILM/DLM
  - No copying operations needed during startup
  - Faster boot time as no flash-to-RAM copying is required
*/

/* NOTE: 
  DLM is configured as readable-writable-executable (rwxa) to support stack and data operations.
  The original non-readable design caused Store/AMO access faults during interrupt handling.
*/

MEMORY
{
  /* Instruction Local Memory (ILM): read-execute (rxa), not writable (!w), primary execution memory */
  ilm    (rxa!w) : ORIGIN = ILM_MEMORY_BASE,       LENGTH = ILM_MEMORY_SIZE   /*I/D combined sram*/
  
  /* Data Local Memory (DLM): read-write-execute (rwxa), for data and stack/heap */
  dlm    (wxar) : ORIGIN = DLM_MEMORY_BASE,        LENGTH = DLM_MEMORY_SIZE   /*I/D combined sram*/
  
  /* Flash memory: read-execute (rxa), not writable (!w), available but not used for execution */
  flash  (rxa!w) : ORIGIN = FLASH_MEMORY_BASE,     LENGTH = FLASH_MEMORY_SIZE /*nor flash as rom*/
  
  /* System SRAM: write-execute-read (wxar), reserved mainly for other masters (e.g., USB) */
  sram   (wxar)  : ORIGIN = SRAM_MEMORY_BASE,      LENGTH = SRAM_MEMORY_SIZE  /* on-chip system sram, reserved mainly for other masters(e.g. USB) */
}

/* Creates an alias 'RAM' for the 'ilm' memory region, used in SECTIONS for readability */
REGION_ALIAS("RAM", ilm)

SECTIONS
{
  PROVIDE(__STACK_SIZE = 8192);     /* Boot stack: total size (half for main + half for interrupt) */
  PROVIDE(__HEAP_SIZE = 1024);      /* Boot heap: for general malloc */
  PROVIDE(__SMP_CPU_CNT = 1);
  
  /* __TOT_STACK_SIZE: Total stack size, calculated as stack size times CPU count */
  __TOT_STACK_SIZE = __STACK_SIZE * __SMP_CPU_CNT;

  /* .init section: Initial code executed at startup, placed in ILM both virtually and physically */
  /* Contains startup_from_ilm.S which assumes ILM/DLM are already initialized */
  .init           :
  {
    *(.text.init)                   /* Startup code specific to initialization */
    KEEP (*(SORT_NONE(.init)))      /* Keeps .init sections in their original order */
    . = ALIGN(4);                   /* Aligns the section to a 4-byte boundary */
  } >RAM AT>RAM

  /* .text section: Main executable code, placed in ILM at runtime and loaded from ILM */
  .text           :
  {
    *(.text.vtable)                 /* Virtual table sections for C++ */
    *(.text.vtable_s)               /* Additional vtable sections */
    *(.text.unlikely .text.unlikely.*) /* Code unlikely to execute (e.g., error handlers) */
    *(.text.startup .text.startup.*)   /* Startup-related code */
    . = ALIGN(8);                   /* Align to 8-byte boundary */
    
    /* __jvt_base$: Base address of jump vector table for RISC-V */
    PROVIDE( __jvt_base$ = . );
    
    *(.text.tbljal .text.tbljal.*)  /* Jump and link table sections */
    *(.text .text.*)                /* General executable code */
    *(.gnu.linkonce.t.*)            /* GNU-specific text sections */
    
    /* .fini: Finalization code (e.g., destructors) */
    . = ALIGN(8);
    KEEP (*(SORT_NONE(.fini)))      /* Keeps .fini sections in original order */
    
    /* Read-only data placed in RAM (ILM) for faster access */
    . = ALIGN(8);
    *(.srodata.cst16)               /* 16-byte constant read-only data */
    *(.srodata.cst8)                /* 8-byte constant read-only data */
    *(.srodata.cst4)                /* 4-byte constant read-only data */
    *(.srodata.cst2)                /* 2-byte constant read-only data */
    *(.srodata .srodata.*)          /* Small read-only data */
    *(.rdata)                       /* General read-only data */
    *(.rodata .rodata.*)            /* Read-only data */
    *(.gnu.linkonce.r.*)            /* GNU-specific read-only data */

    /* .preinit_array: Array of function pointers for pre-initialization */
    . = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .); /* Start of preinit array */
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);   /* End of preinit array */
    
    /* .init_array: Array of function pointers for constructors */
    . = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);    /* Start of init array */
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);      /* End of init array */
    
    /* .fini_array: Array of function pointers for destructors */
    . = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);    /* Start of fini array */
    PROVIDE_HIDDEN (__libc_fini = _fini);       /* Pointer to libc finalization function */
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);      /* End of fini array */
    
    /* .ctors: Constructor sections */
    . = ALIGN(8);
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    
    /* .dtors: Destructor sections */
    . = ALIGN(8);
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >RAM AT>RAM                     /* Located in ILM, loaded from ILM */

  /* Symbols for .text section boundaries */
  PROVIDE( _ilm_lma = LOADADDR(.text) );    /* Load address of ILM (same as VMA for ILM boot) */
  PROVIDE( _ilm = ADDR(.text) );            /* Virtual address of ILM */
  PROVIDE( _eilm = . );                     /* End address of ILM */
  PROVIDE( _text_lma = LOADADDR(.text) );   /* Load address of .text (same as VMA for ILM boot) */
  PROVIDE( _text = ADDR(.text) );           /* Virtual address of .text */
  PROVIDE( _etext = . );                    /* End of text section (alias 1) */
  PROVIDE( __etext = . );                   /* End of text section (alias 2) */
  PROVIDE( etext = . );                     /* End of text section (alias 3) */

  /* .data section: Initialized data, placed in DLM at runtime and loaded from DLM */
  .data            : ALIGN(8)
  {
    KEEP(*(.data.ctest*))           /* Custom test data sections */
    *(.data .data.*)                /* General initialized data */
    *(.gnu.linkonce.d.*)            /* GNU-specific data sections */
    . = ALIGN(8);
    
    /* __global_pointer$: Global pointer for RISC-V ABI, offset by 0x800 for efficient access */
    PROVIDE( __global_pointer$ = . + 0x800 );
    
    *(.sdata .sdata.* .sdata*)      /* Small initialized data */
    *(.gnu.linkonce.s.*)            /* GNU-specific small data sections */
    
    /* Exception handling frame symbols */
    PROVIDE_HIDDEN (__eh_frame_hdr_start = .); /* Start of exception frame header */
    *(.eh_frame_hdr)
    PROVIDE_HIDDEN (__eh_frame_hdr_end = .);   /* End of exception frame header */
    PROVIDE_HIDDEN (__eh_frame_start = .);     /* Start of exception frame */
    *(.eh_frame)
    PROVIDE_HIDDEN (__eh_frame_end = .);       /* End of exception frame */
    
    /* RT-Thread specific sections */
    . = ALIGN(8);
    __rt_init_start = .;            /* Start of RT-Thread initialization functions */
    KEEP(*(SORT(.rti_fn*)))
    __rt_init_end = .;              /* End of RT-Thread initialization functions */
    
    . = ALIGN(8);
    __fsymtab_start = .;            /* Start of RT-Thread function symbol table */
    KEEP(*(FSymTab))
    __fsymtab_end = .;              /* End of RT-Thread function symbol table */
    
    . = ALIGN(8);
    __vsymtab_start = .;            /* Start of RT-Thread variable symbol table */
    KEEP(*(VSymTab))
    __vsymtab_end = .;              /* End of RT-Thread variable symbol table */
    
    . = ALIGN(8);
  } >dlm AT>dlm                     /* Located in DLM, loaded from DLM */

  /* .tdata section: Thread-local initialized data, placed in DLM and loaded from DLM */
  .tdata           : ALIGN(8)
  {
    /* __tls_base: Base address of thread-local storage */
    PROVIDE( __tls_base = . );
    
    *(.tdata .tdata.* .gnu.linkonce.td.*) /* Thread-local initialized data */
  } >dlm AT>dlm

  /* Symbols for .data section boundaries */
  PROVIDE( _data_lma = LOADADDR(.data) );   /* Load address of .data (same as VMA for ILM boot) */
  PROVIDE( _data = ADDR(.data) );           /* Virtual address of .data */
  PROVIDE( _edata = . );                    /* End of initialized data (alias 1) */
  PROVIDE( edata = . );                     /* End of initialized data (alias 2) */

  /* Symbols marking the start of uninitialized data */
  PROVIDE( _fbss = . );                     /* Start of BSS (uninitialized data) */
  PROVIDE( __bss_start = . );               /* Alias for start of BSS */

  /* .tbss section: Thread-local uninitialized data, placed in DLM, not loaded */
  .tbss (NOLOAD)   : ALIGN(8)
  {
    *(.tbss .tbss.* .gnu.linkonce.tb.*)    /* Thread-local uninitialized data */
    *(.tcommon)                            /* Common thread-local data */
    
    /* __tls_end: End of thread-local storage */
    PROVIDE( __tls_end = . );
  } >dlm AT>dlm                            /* Located in DLM, not loaded */

  /* .tbss_space section: Reserves space equivalent to .tbss size in DLM, not loaded */
  .tbss_space (NOLOAD) : ALIGN(8)
  {
    . = . + SIZEOF(.tbss);                /* Reserves space for thread-local BSS */
  } >dlm AT>dlm

  /* .bss section: Uninitialized global data, placed in DLM, not loaded */
  .bss (NOLOAD)   : ALIGN(8)
  {
    *(.sbss*)                              /* Small uninitialized data */
    *(.gnu.linkonce.sb.*)                  /* GNU-specific small BSS */
    *(.bss .bss.*)                         /* General uninitialized data */
    *(.gnu.linkonce.b.*)                   /* GNU-specific BSS */
    *(COMMON)                              /* Common symbols (uninitialized) */
    . = ALIGN(4);                          /* Align to 4-byte boundary */
  } >dlm AT>dlm

  /* Symbols marking the end of data sections */
  PROVIDE( _end = . );                      /* End of all data sections (alias 1) */
  PROVIDE( end = . );                       /* End of all data sections (alias 2) */

  /* .heap section: Heap memory, reserved in DLM, not loaded */
  /* Nuclei C Runtime Library requirements:
   * 1. Heap must be aligned at 16 bytes
   * 2. __heap_start and __heap_end symbols must be defined
   * 3. Reserves at least __HEAP_SIZE space (2K) */
  .heap (NOLOAD)   : ALIGN(16)
  {
    . = ALIGN(16);
    /* __heap_start: Starting address of the heap */
    PROVIDE( __heap_start = . );
    
    . += __HEAP_SIZE;                      /* Reserves heap size (2K) */
    . = ALIGN(16);
    
    /* __heap_limit: End address of the heap (upper boundary) */
    PROVIDE( __heap_limit = . );
  } >dlm AT>dlm

  /* .stack section: Stack memory, placed at the end of DLM, not loaded */
  .stack ORIGIN(dlm) + LENGTH(dlm) - __TOT_STACK_SIZE (NOLOAD) :
  {
    . = ALIGN(16);
    /* _heap_end: Alias for end of heap (start of stack) */
    PROVIDE( _heap_end = . );
    
    /* __heap_end: Alias for end of heap (start of stack) */
    PROVIDE( __heap_end = . );
    
    /* __StackLimit: Lowest address of the stack (bottom boundary) */
    PROVIDE( __StackLimit = . );
    
    /* __StackBottom: Alias for stack bottom */
    PROVIDE( __StackBottom = . );
    
    . += __TOT_STACK_SIZE;                 /* Reserves total stack size */
    . = ALIGN(16);
    
    /* Stack top - used by both main and interrupts (shared, grows down) */
    PROVIDE( __StackTop = . );
    PROVIDE( _sp = . );
    
    /* Simplified design: No separate interrupt stack */
    /* - Boot phase: main and interrupts share the same stack */
    /* - FreeRTOS phase: entire stack becomes interrupt stack */
    /* - Interrupts save registers directly on current stack (no swap) */
  } >dlm AT>dlm
}

/*
ILM Boot Linker Script Configuration:
- Boots directly from ILM memory using startup_from_ilm.S
- Code and data are assumed to be already loaded in ILM/DLM
- No flash-to-RAM copying operations needed
- Faster boot time due to direct execution from ILM
- This configuration requires startup_from_ilm.S which skips copy operations
*/
