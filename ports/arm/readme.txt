ARM-Port
--------

This is a release of the ARM port. Maybe not all bugs are found yet. 
The code has been only tested on SAMSUNG S3C2510A CPU(ARM940T core),
and compiled with ARM's Software Development Toolkit (SDT) version
2.51.

Restrictions of this port:
   This port does not support extended nano-layer features like standard-
   console-I/O yet. To enable standard-I/O, you need to add a UART driver.
   Please see the pico]OS documentation for how to connect the UART driver
   to nano-layer standard-I/O (see description of functions  c_nos_keyinput
   and p_putchar). The file noscfg.h is not shipped with this port, thus you
   need to create your own one (see noscfg.h in examples directory).
 
   The file port.mak is not complete and thus non-working yet.
   Please use the batch file build.bat instead to build the demo program.


About task context:
   We save ARM CPU register : r0-r12,r14(return PC),r15(PC),CPSR(status register), 
     where r13 is stack pointer, this saved in current TCB(task control block).
     and current stack top(4bytes) saved also.
     with task stack init, CPSR mode is SUPER MODE(interrupt not masked).
   when context switch, ARM CPU interrupt masked.
     we save registers to current task's stack, and restore new task's stack pointer to r13,
     r14(return PC), then force ARM in IRQ mode, copy new task's registers to ARM IRQ mode's 
     stack(this stack is not same as super mode stack),
     then
        ldmfd sp!, {r0-r12,pc}^
     restore r0-r12,pc,CPSR(CPSR include ARM mode, force it return SUPER MODE).


File List:
    types.h
       some type define.
       
    interrupt.h
       interrupt.c function declare.

    mem.h
       mem.c function declare.  

    port.h
    poscfg.h
       picoos port.

    Boot.s
        ARM CPU boot code.
        when boot from ROM, need modify it.

    arch_arm_asm.s    
        ARM CPU specific code.
        include interrupt init, stack init, 
        interrupt service, context save/restore function
   
   arch_arm_c.c
        ARM CPU specific code.
        include stack init.

   init.c
        ARM C-Entry point and basic initialization

   interrupt.c
        ARM CPU interrupt service C file.
        
   mem.c
        utils for mem manage.
        
   demo.c
        example.
        Two task, one sleep, other sleep also.


Memory use:
   In this port, define
     POSCFG_MAX_TASKS      =64
     POSCFG_MAX_EVENTS     =64
     POSCFG_MAX_MESSAGES   =1024
     POSCFG_MAX_TIMER      =32 
     POSCFG_SOFTINTERRUPTS =40
     POSCFG_SOFTINTQUEUELEN=40
     (all above feature is static memory declare)
     
   and include all
     POSCFG_FEATURE_xxx
   
   and ARM interrupt service use 14Kbytes memory.
   
   This use memory 50K(task's stack not include).


TODO
   for boot from ROM, modify boot.s.
   for other ARM CPU:
       modify the source code between
   /************ BEGIN TARGET SPECIFIC SECTION ************/ 
    ..... 
   /************ END TARGET SPECIFIC SECTION **************/ 

