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
; * CVS-ID $Id: arch_c.c,v 1.1.1.1 2004/02/16 20:11:33 smocz Exp $
; */

;================== CONSTANTS ==================
INTR_MASK       EQU     0xC0


;================= CODE =========================

    AREA    Boot, CODE, READONLY

; --- Define entry point
        EXPORT  __main  
__main                  
    ENTRY

    ; disable CPU interrupt
    MRS     r0,CPSR                     
    ORR r0, r0, #INTR_MASK          
    MSR cpsr_cxsf, r0

    IMPORT  ARM_CPU_Setup
    IMPORT  SystemStackInit

    BL  SystemStackInit
    
    B   ARM_CPU_Setup

    ALIGN

    END