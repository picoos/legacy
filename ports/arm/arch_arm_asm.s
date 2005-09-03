;/*
; *  Copyright (c) 2004, Jun Li, lj_sourceforge@users.sourceforge.net.
; *  Copyright (c) 2004, Dennis Kuschel.
; *  All rights reserved. 
; *
; *  Redistribution and use in source and binary forms, with or without
; *  modification, are permitted provided that the following conditions
; *  are met:
; *
; *   1. Redistributions of source code must retain the above copyright
; *      notice, this list of conditions and the following disclaimer.
; *   2. Redistributions in binary form must reproduce the above copyright
; *      notice, this list of conditions and the following disclaimer in the
; *      documentation and/or other materials provided with the distribution.
; *   3. The name of the author may not be used to endorse or promote
; *      products derived from this software without specific prior written
; *      permission. 
; *
; *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
; *  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
; *  ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
; *  INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
; *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
; *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
; *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
; *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
; *  OF THE POSSIBILITY OF SUCH DAMAGE.
; *
; */
;/*
; * This file is originally from the pico]OS realtime operating system
; * (http://picoos.sourceforge.net).
; *
; * CVS-ID $Id: arch_arm_asm.s,v 1.1 2004/06/07 18:47:22 dkuschel Exp $
; */


;================== CONSTANTS ==================

; arm CPSR
INTR_MASK       EQU     0xC0       ; disable IRQ & FIQ

UDF_MODE        EQU     0x1B       ; Undefine Mode(UDF)
ABT_MODE        EQU     0x17       ; Abort Mode(ABT)
SUP_MODE        EQU     0x13       ; Supervisor Mode (SVC)
IRQ_MODE        EQU     0x12       ; Interrupt Mode (IRQ)
FIQ_MODE        EQU     0x11       ; Fast Interrupt Mode (FIQ)

; ************ BEGIN TARGET SPECIFIC SECTION **************

; S3C2510 registers
ASIC_BASE   EQU 0xf0000000
CLKCON      EQU     ASIC_BASE+0x0008
PCLKDIS     EQU ASIC_BASE+0x000c    

TIC     EQU ASIC_BASE+0x040004

INTMOD      EQU ASIC_BASE+0x140000
EXTMOD      EQU ASIC_BASE+0x140004
INTMASK     EQU ASIC_BASE+0x140008
EXTMASK     EQU ASIC_BASE+0x14000C
INTOFFSET_IRQ   EQU ASIC_BASE+0x14001C

; ************ END TARGET SPECIFIC SECTION **************

; stack size
UDF_STACK_SIZE  EQU 1024
ABT_STACK_SIZE  EQU 1024
IRQ_STACK_SIZE  EQU 4096
FIQ_STACK_SIZE  EQU 1024
SUP_STACK_SIZE  EQU 8192
RES_STACK_SIZE  EQU 16


;================= CODE ========================

    AREA |C$$code|, CODE, READONLY

    EXPORT  ARM_CPU_Setup

    IMPORT  C_Entry         

VECTOR_TABLE
        LDR     PC, RST_PC
        LDR     PC, UDF_PC
        LDR     PC, SWI_PC
        LDR     PC, PRE_PC
        LDR     PC, ABT_PC
        NOP
        LDR     PC, IRQ_PC
        LDR     PC, FIQ_PC

RST_PC      DCD     ARM_CPU_Setup
UDF_PC      DCD     SystemUndefinedHandler
SWI_PC          DCD     SystemSwiHandler
PRE_PC      DCD     SystemPrefetchHandler
ABT_PC          DCD     SystemAbortHandler
                DCD     0       
IRQ_PC          DCD     SystemIrqHandler
FIQ_PC          DCD     SystemFiqHandler

    
ARM_CPU_Setup   

; System Configuration
SYSTEM_CONFIGURATION
    ; disable CPU interrupt
    MRS     r0,CPSR                     
    ORR r0, r0, #INTR_MASK          
    MSR cpsr_cxsf, r0

; ************ BEGIN TARGET SPECIFIC SECTION **************
    LDR r1, =CLKCON 
    LDR r0, =0x0    
    STR r0, [r1]

    LDR r1, =PCLKDIS
    LDR r0, =0x0
    STR r0, [r1]

    ; interrupt mode
    LDR r0, =INTMOD         
    MOV     r3, #0x0                
    STR     r3, [r0]        

    LDR r0, =EXTMOD         
    MOV     r3, #0x0                
    STR     r3, [r0]        

    ; disable external interrupt source
    LDR     r0, =0x8000003f         
    LDR r1, =EXTMASK            
    STR     r0, [r1]                    

    ;  disable internal interrupt source
    LDR     r0, =0xffffffff         
    LDR r1, =INTMASK            
    STR     r0, [r1]    
; ************ END TARGET SPECIFIC SECTION **************

INITIALIZE_STACK
    LDR r0,=(INTR_MASK:OR:FIQ_MODE)
    MSR cpsr_c, r0
    LDR sp, =FIQ_STACK

    LDR r0, =(INTR_MASK:OR:IRQ_MODE)
    MSR cpsr_c, r0
    LDR sp, =IRQ_STACK

    LDR r0, =(INTR_MASK:OR:ABT_MODE)
    MSR cpsr_c, r0
    LDR sp, =ABT_STACK

    LDR r0, =(INTR_MASK:OR:UDF_MODE)
    MSR cpsr_c, r0      
    LDR sp, =UDF_STACK

    LDR r0, =(INTR_MASK:OR:SUP_MODE)
    MSR cpsr_c, r0
    LDR sp, =SUP_STACK

SETUP_VECTOR
    ; setup arm vectors
        MOV     R8,  #0
        ADR     R9,  VECTOR_TABLE
        LDMIA   R9!, {R0-R7}
        STMIA   R8!, {R0-R7}
        LDMIA   R9!, {R0-R7}
        STMIA   R8!, {R0-R7}

BSS_CLEAR
    IMPORT  |Image$$ZI$$Base|
    IMPORT  |Image$$ZI$$Limit|

    LDR     r3, =|Image$$ZI$$Base|
    LDR     r0, =|Image$$ZI$$Limit|
    MOV     r2, #0                    

bss_clear_Loop
    CMP     r3, r0                
    STRCC   r2, [r3], #4           
    BCC     bss_clear_Loop        

    ; get free memory start address
    LDR r0, =|Image$$ZI$$Limit|
    B   C_Entry

;========================== ARM's Vectors =========================

    ; C interrupt handle function
    IMPORT  ISRUndefHandler
    IMPORT  ISRSwiHandler
    IMPORT  ISRPrefetchHandler
    IMPORT  ISRAbortHandler
    IMPORT  ISRIrqHandler
    IMPORT  ISRFiqHandler

SystemUndefinedHandler      
    STMFD   sp!, {r0-r12,lr}
    SUB r0, lr, #0x4
    BL  ISRUndefHandler
    LDMFD   sp!, {r0-r12, pc}^

SystemSwiHandler
    STMFD   sp!, {r0-r12,lr}
    BL  ISRSwiHandler
    LDMFD   sp!, {r0-r12, pc}^
        
SystemPrefetchHandler
    STMFD   sp!, {r0-r12, lr}
    SUB r0, lr, #0x8    
    BL  ISRPrefetchHandler
    LDMFD   sp!, {r0-r12, lr}
    SUBS    pc, lr, #4

SystemAbortHandler
    STMFD   sp!, {r0-r12, lr}
    SUB r0, lr, #0xc        
    BL  ISRAbortHandler
    LDMFD   sp!, {r0-r12, lr}
    SUBS    pc, lr, #8

SystemReserv
    SUBS    pc, lr, #4

SystemFiqHandler
    STMFD   sp!, {r0-r7, lr}
    BL  ISRFiqHandler
    LDMFD   sp!, {r0-r7, lr}
    SUBS    pc, lr, #4

;====================== ARM IRQ & task context ==========================
    IMPORT  c_pos_timerInterrupt
    IMPORT  c_pos_intEnter
    IMPORT  c_pos_intExit
    IMPORT  posCurrentTask_g
    IMPORT  posNextTask_g
    IMPORT  posInInterrupt_g

    EXPORT  _osEnterCritical
    EXPORT  _osExitCritical
    EXPORT  _osThreadSwitch
    EXPORT  _osThreadSwitchTo
    EXPORT  p_pos_startFirstContext
    EXPORT  p_pos_intContextSwitch
    EXPORT  p_pos_softContextSwitch

    EXPORT  SystemStackInit


p_pos_startFirstContext
    LDR r0,=posCurrentTask_g
    LDR r0,[r0]
    B   _osThreadSwitchTo
    
p_pos_intContextSwitch
    LDR r0,=posNextTask_g
    LDR r0,[r0]
    B   _osThreadSwitchTo

p_pos_softContextSwitch
    LDR r0,=posCurrentTask_g
    LDR r0,[r0]
    LDR r1,=posNextTask_g
    LDR r1,[r1]
    B   _osThreadSwitch

SystemStackInit
    LDR sp, =SUP_STACK
    MOV pc, lr


SystemIrqHandler
    ; save r0-r12 & lr, lr is return pc
    SUBS    lr, lr, #4
    STMFD   sp!, {r0-r12, lr}

; ************ BEGIN TARGET SPECIFIC SECTION **************
    LDR     r1, =INTOFFSET_IRQ          ; Get location of interrupt contrl
    LDR r0, [r1]                    ; Get current IRQ status
    AND r0, r0, #0xff

    CMP     r0, #0x23
    BLE is_valid_intno

    ; invalid then return
    LDMFD   sp!, {r0-r12, pc}^

is_valid_intno
    CMP r0, #0x1d                   ; Check for timer interrupt
    BNE     skip_timer

    ; clear timer
    LDR     r2, =TIC                ; Get timer base
    LDR     r1, [r2]         
    ORR     r1, r1, #0x02       ; clear timer0
    STR     r1, [r2]            ; Clear timer interrupt

skip_timer  

; ************ END TARGET SPECIFIC SECTION **************

    ; save SPSR_irq, it is CPSR_svc
    mrs     r1, SPSR
    STMFD   sp!, {r1}

    ; save SP_irq to r1
    MOV     r1, sp

    ; restore IRQ's sp
    ADD sp, sp, #60         ;r0-r12, pc, CPSR
    
    ; goto SVC mode
    LDR r2, =(INTR_MASK:OR:SUP_MODE)
    MSR cpsr_c, r2

    ; now in  SVC mode
    ; r0 = interrupt no, r1=SP_irq
    
    sub     sp,sp,#64       ; a free stack space, r0-r12,pc,cpsr
    mov r2,sp
    
    STMFD   sp!, {lr}       ; save lr_svc
    sub     sp,sp,#4        ; a free stack space

        LDMIA   r1!, {R3-R12}       ; copy r0-r9
        STMIA   r2!, {R3-R12}
        
        LDMIA   r1!, {R3-R7}        ; copy r10-r12, pc, cpsr
        STMIA   r2!, {R3-R7}

    ; save current thread's SP
    ldr r1, =posCurrentTask_g
    ldr r2, [r1]
    str sp, [r2]

    ; save interrupt no 
    MOV r5, r0

    BL  c_pos_intEnter

    MOV r0, r5
    BL  ISRIrqHandler

    CMP r5, #0x1d
    BNE skip_postimer

    BL  c_pos_timerInterrupt

skip_postimer
    BL  c_pos_intExit

    LDR r1,=posCurrentTask_g
    LDR r0,[r1]
    B   _osThreadSwitchTo


; u_int _NutEnterCritical(int reserve1, int reserve2)
_osEnterCritical    
        mrs     r0, CPSR
        orr     r1, r0, #INTR_MASK
        msr     CPSR_c, r1
        mov     pc, lr

; void _NutExitCritical(u_int old_int)
_osExitCritical
        msr     CPSR_c, r0
        mov     pc, lr

; void _osThreadSwitch(void *prevthread, void *activethread)
_osThreadSwitch
    ; save prev TCB
    sub     sp, sp, #4       ; add a stack free
    ldr r2, =task_reactive   ; set pc to 
        stmfd   sp!,{r2}             ; push pc (lr should be pushed in place of PC)
        stmfd   sp!,{r0-r12}         ; push lr & register file
    mrs     r2, cpsr             ; Pickup current CPSR
        stmfd   sp!,{r2}             ; push current cpsr
        stmfd   sp!,{lr}             ; push lr & register file
    sub sp, sp, #4
    str sp, [r0]

    ; restore new active TCB
    mov r0, r1
    B   _osThreadSwitchTo

task_reactive
    mov pc, lr

; _osThreadSwitchTo(void *activethread)
_osThreadSwitchTo
    ;save   posNextTask_g to posCurrentTask_g
    ldr r1,=posCurrentTask_g
    str r0, [r1]

    ; get thread's sp
    ldr r1, [r0]
    mov sp, r1
    ; restore it value
    add sp, sp, #72     ;stack,r0-r12, lr, pc, cpsr, stack

    ; get stack & lr, IRQ's lr is not the SVC's lr
    ldmia   r1!,{r2,lr}
        
    ; switch to IRQ mode
    LDR r2, =(INTR_MASK:OR:IRQ_MODE)
    MSR cpsr_c, r2

    ; now in IRQ mode

    ; we use IRQ mode stack, copy thread's stack data to it
    ; for restore thread's context
    ldmia   r1!,{r3}            ; pop new task's cpsr to spsr
        msr     SPSR_cxsf,r3

        sub sp,sp,#56       ; save r0-r12,pc
    mov r0,sp

    LDR lr, [r1]
        
        LDMIA   r1!, {R2-R12}
        STMIA   r0!, {R2-R12}
        
        LDMIA   r1!, {R2-R4}
        STMIA   r0!, {R2-R4}

        ldmfd   sp!, {r0-r12,pc}^       ; pop new task's r0-r12 & pc & spsr->cpsr


;================= SYSTEM STACK MEMORY =================

    AREA    SYS_STACK, NOINIT
    ALIGN   


STACK_START 
                %       UDF_STACK_SIZE
UDF_STACK
                %       ABT_STACK_SIZE
ABT_STACK
                %       IRQ_STACK_SIZE
IRQ_STACK
                %       FIQ_STACK_SIZE
FIQ_STACK
                %       SUP_STACK_SIZE
SUP_STACK
        %   RES_STACK_SIZE
RES_STACK

    END

