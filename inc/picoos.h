/*
 * License:  Modified BSD license, see documentation below.
 *
 * This file is originally from the pico]OS realtime operating system
 * (http://picoos.sourceforge.net).
 *
 * CVS-ID $Id: picoos.h,v 1.3 2004/02/22 20:11:46 dkuschel Exp $
 *
 */

/**
 * @file    picoos.h
 * @brief   pico]OS main include file
 * @author  Dennis Kuschel
 */

/**
 * @mainpage pico]OS - Real Time Operating System
 *
 * @section intro Introduction
 * pico]OS is a highly configurable and very fast real time operating
 * system. It targets a wide range of architectures, from the small
 * 8 bit processors with very low memory till huge architectures
 * like 32 bit processors with lots of memory.<br><br><br>
 *
 * @subsection features Features
 *
 * <b>Scheduler:</b>
 *  - two modes: standard priority based or round robin
 *  - preemptive multitasking
 *  - max. 64 tasks on 8 bit processors,
 *    max. 1024 tasks on 32 bit processors
 *  - round robin: max. 8 priority level on 8 bit processors,
 *    max. 32 priority level on 32 bit processors
 *
 * <b>Events:</b>
 *  - the number of events is unlimited
 *  - semaphores
 *  - mutexes
 *  - flag events
 *
 * <b>Message Boxes:</b>
 *  - one built in message box per task
 *  - unlimited box size, messages are collected
 *  - messages can be pointers or huge buffers
 *
 * <b>Timer:</b>
 *  - autonomous running timer, the system timer interrupt is used as timebase
 *  - timer can work in one shot mode or in auto reload mode
 *  - when a timer expires, it triggers a semaphore event
 *
 * <b>Software Interrupts:</b>
 *  - up to 256 simulated software interrupts on 8 bit processors
 *  - can be used to interface fast hardware interrupts outside the scope of
 *    pico]OS to the operating system
 *
 * <br><br>
 * @subsection ports Available Ports
 *
 * Currently, ports to the following platforms are available:
 *
 *  - Intel 80x86 in real mode, the executable is loadable from DOS
 *  - 6502 / 65c02 / 6510 series, port for Commodore 64 available.
 *    Minimum configuration (4 tasks): 4.6kb code, 4.7kb data
 *  - PowerPC: IBM PPC440  (GNU C and MetaWare compiler supported)
 *  - AVR (GNU C compiler supported)
 *
 *
 * <br><br>
 * @subsection files Files
 *
 * The pico]OS Real Time Operating System consists of only
 * a couple of files:
 *
 *  - picoos.h 
 *       - This header is the interface to the operating system.
 *         It is included by the pico]OS core and must also be
 *         included by user applications. 
 *  - poscfg.h
 *       - pico]OS configuration file. Must be supplied by the user.
 *  - port.h
 *       - Configuration file for the port.
 *         Must be supplied by the user / port developer.
 *  - picoos.c
 *       - The pico]OS core source file. This is the operating system.
 *  - fbit_gen.c 
 *       - This file implements a set of generic "findbit" functions.
 *         The "findbit" function is needed by the operating system.
 *         The user may support its own "findbit" function to
 *         increase performance on a particular architecture.
 *  - arch_c.c
 *       - User supplied file. This is the architecture specific C-code
 *         of the platform port.
 *  - arch_a.asm
 *       - User supplied file (optional). This is the architecture specific
 *         assembler code of the platform port.
 * 
 * <br><br>
 * @section license License
 *
 *  Copyright (c) 2004, Dennis Kuschel. <br>
 *  All rights reserved. <br>
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met: <br>
 *
 *   -# Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   -# Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   -# The name of the author may not be used to endorse or promote
 *      products derived from this software without specific prior written
 *      permission. 
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 *  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 *  INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE. <br>
 *
 *
 * <br><br>
 * @section cont Contact Information
 * Dennis Kuschel <br>
 * Emanuel-Backhaus-Strasse 20 <br>
 * 28277 Bremen <br>
 * GERMANY <br>
 *
 * mail: dennis_k@freenet.de <br>
 * http: http://mycpu.mikrocontroller.net <br>
 *
 * (C) 2004 Dennis Kuschel
 */

#ifndef _PICOOS_H
#define _PICOOS_H


#define POS_VER_N           0x0051
#define POS_VER_S           "0.5.1"
#define POS_COPYRIGHT       "(c) 2004, Dennis Kuschel"
#define POS_STARTUPSTRING   "pico]OS " POS_VER_S "  " POS_COPYRIGHT

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef DOX
#define DOX  0
#endif
#if DOX==0

#include <port.h>
#include <poscfg.h>



/*---------------------------------------------------------------------------
 *  CONFIGURATION
 *-------------------------------------------------------------------------*/

/* test if all needed parameters are set */
#ifndef MVAR_t
#error  MVAR_t not defined
#endif
#ifndef MVAR_BITS
#error  MVAR_BITS not defined
#endif
#ifndef POSCFG_ALIGNMENT
#error  POSCFG_ALIGNMENT not defined
#endif
#ifndef POSCFG_MAX_PRIO_LEVEL
#error  POSCFG_MAX_PRIO_LEVEL not defined
#endif
#ifndef POSCFG_MAX_TASKS
#error  POSCFG_MAX_TASKS not defined
#endif
#ifndef POSCFG_MAX_EVENTS
#error  POSCFG_MAX_EVENTS not defined
#endif
#ifndef POSCFG_MAX_MESSAGES
#error  POSCFG_MAX_MESSAGES not defined
#endif
#ifndef POSCFG_MAX_TIMER
#error  POSCFG_MAX_TIMER not defined
#endif
#ifndef POSCFG_SOFT_MTASK
#error  POSCFG_SOFT_MTASK not defined
#endif
#ifndef POSCFG_CTXSW_COMBINE
#error  POSCFG_CTXSW_COMBINE not defined
#endif
#ifndef POSCFG_REALTIME_PRIO
#error POSCFG_REALTIME_PRIO not defined
#endif
#ifndef POSCFG_ISR_INTERRUPTABLE
#error  POSCFG_ISR_INTERRUPTABLE not defined
#endif
#ifndef POSCFG_ROUNDROBIN
#error  POSCFG_ROUNDROBIN not defined
#endif
#ifndef POSCFG_TASKS_PER_PRIO
#error  POSCFG_TASKS_PER_PRIO not defined
#endif
#ifndef POSCFG_FASTCODE
#error  POSCFG_FASTCODE not defined
#endif
#ifndef POSCFG_SMALLCODE
#error  POSCFG_SMALLCODE not defined
#endif
#ifndef POSCFG_ARGCHECK
#error  POSCFG_ARGCHECK not defined
#endif
#ifndef POSCFG_MSG_MEMORY
#error  POSCFG_MSG_MEMORY not defined
#endif
#ifndef POSCFG_MSG_BUFSIZE
#error  POSCFG_MSG_BUFSIZE not defined
#endif
#ifndef POSCFG_DYNAMIC_MEMORY
#error  POSCFG_DYNAMIC_MEMORY not defined
#endif
#if POSCFG_DYNAMIC_MEMORY != 0
#ifndef POS_MEM_ALLOC
#error  POS_MEM_ALLOC not defined
#endif
#ifndef POS_MEM_ALLOC
#error  POS_MEM_ALLOC not defined
#endif
#endif
#ifndef POSCFG_CALLINITARCH
#error POSCFG_CALLINITARCH not defined
#endif
#ifndef POSCFG_LOCK_USEFLAGS
#error  POSCFG_LOCK_USEFLAGS not defined
#endif
#if POSCFG_LOCK_USEFLAGS != 0
#ifndef POSCFG_LOCK_FLAGSTYPE
#error  POSCFG_LOCK_FLAGSTYPE not defined
#endif
#endif
#ifndef POS_SCHED_LOCK
#error  POS_SCHED_LOCK not defined
#endif
#ifndef POS_SCHED_UNLOCK
#error  POS_SCHED_UNLOCK not defined
#endif
#ifndef HZ
#error  HZ not defined
#endif
#ifndef POSCFG_TASKSTACKTYPE
#error  POSCFG_TASKSTACKTYPE not defined
#endif
#ifndef POS_USERTASKDATA
#error  POS_USERTASKDATA not defined
#endif
#ifndef POSCFG_FEATURE_YIELD
#error  POSCFG_FEATURE_YIELD not defined
#endif
#ifndef POSCFG_FEATURE_SLEEP
#error  POSCFG_FEATURE_SLEEP not defined
#endif
#ifndef POSCFG_FEATURE_EXIT
#error  POSCFG_FEATURE_EXIT not defined
#endif
#ifndef POSCFG_FEATURE_GETPRIORITY 
#error  POSCFG_FEATURE_GETPRIORITY  not defined
#endif
#ifndef POSCFG_FEATURE_SETPRIORITY
#error  POSCFG_FEATURE_SETPRIORITY not defined
#endif
#ifndef POSCFG_FEATURE_SEMAPHORES
#error  POSCFG_FEATURE_SEMAPHORES not defined
#endif
#ifndef POSCFG_FEATURE_SEMADESTROY
#error  POSCFG_FEATURE_SEMADESTROY not defined
#endif
#ifndef POSCFG_FEATURE_SEMAWAIT
#error  POSCFG_FEATURE_SEMAWAIT not defined
#endif
#ifndef POSCFG_FEATURE_MUTEXES
#error  POSCFG_FEATURE_MUTEXES not defined
#endif
#ifndef POSCFG_FEATURE_MUTEXDESTROY
#error  POSCFG_FEATURE_MUTEXDESTROY not defined
#endif
#ifndef POSCFG_FEATURE_MUTEXTRYLOCK
#error  POSCFG_FEATURE_MUTEXTRYLOCK not defined
#endif
#ifndef POSCFG_FEATURE_GETTASK
#error  POSCFG_FEATURE_GETTASK not defined
#endif
#ifndef POSCFG_FEATURE_TASKUNUSED
#error  POSCFG_FEATURE_TASKUNUSED not defined
#endif
#ifndef POSCFG_FEATURE_MSGBOXES
#error  POSCFG_FEATURE_MSGBOXES not defined
#endif
#ifndef POSCFG_FEATURE_MSGWAIT
#error  POSCFG_FEATURE_MSGWAIT not defined
#endif
#ifndef POSCFG_FEATURE_INHIBITSCHED
#error  POSCFG_FEATURE_INHIBITSCHED not defined
#endif
#ifndef POSCFG_FEATURE_JIFFIES
#error  POSCFG_FEATURE_JIFFIES not defined
#endif
#ifndef POSCFG_FEATURE_TIMER
#error  POSCFG_FEATURE_TIMER not defined
#endif
#ifndef POSCFG_FEATURE_TIMERDESTROY
#error  POSCFG_FEATURE_TIMERDESTROY not defined
#endif
#ifndef POSCFG_FEATURE_TIMERFIRED
#error  POSCFG_FEATURE_TIMERFIRED not defined
#endif
#ifndef POSCFG_FEATURE_FLAGS
#error  POSCFG_FEATURE_FLAGS not defined
#endif
#ifndef POSCFG_FEATURE_FLAGDESTROY
#error  POSCFG_FEATURE_FLAGDESTROY not defined
#endif
#ifndef POSCFG_FEATURE_FLAGWAIT
#error  POSCFG_FEATURE_FLAGWAIT not defined
#endif
#ifndef POSCFG_FEATURE_SOFTINTS
#error POSCFG_FEATURE_SOFTINTS not defined
#else
#ifndef POSCFG_FEATURE_SOFTINTDEL
#error POSCFG_FEATURE_SOFTINTDEL not defined
#endif
#ifndef POSCFG_SOFTINTERRUPTS
#error POSCFG_SOFTINTERRUPTS not defined
#endif
#ifndef POSCFG_SOFTINTQUEUELEN
#error POSCFG_SOFTINTQUEUELEN not defined
#endif
#endif
#ifndef POSCFG_FEATURE_IDLETASKHOOK
#error POSCFG_FEATURE_IDLETASKHOOK not defined
#endif


/* parameter range checking */
#if (MVAR_BITS != 8) && (MVAR_BITS != 16) && (MVAR_BITS != 32)
#error MVAR_BITS must be 8, 16 or 32
#endif
#if POSCFG_MAX_PRIO_LEVEL == 0
#error POSCFG_MAX_PRIO_LEVEL must not be zero
#endif
#if (POSCFG_ROUNDROBIN != 0) && (POSCFG_MAX_PRIO_LEVEL > MVAR_BITS)
#error POSCFG_MAX_PRIO_LEVEL must not exceed MVAR_BITS
#endif 
#if (POSCFG_ROUNDROBIN == 0) && (POSCFG_MAX_PRIO_LEVEL > (MVAR_BITS*MVAR_BITS))
#error POSCFG_MAX_PRIO_LEVEL must not exceed (MVAR_BITS * MVAR_BITS)
#endif 
#if (POSCFG_MAX_TASKS < 3) || (POSCFG_MAX_TASKS > (POSCFG_TASKS_PER_PRIO * POSCFG_MAX_PRIO_LEVEL))
#error POSCFG_MAX_TASKS is less than 3 or much to big
#endif
#if POSCFG_MAX_EVENTS < 1
#error POSCFG_MAX_EVENTS must be at least 1
#endif
#if POSCFG_TASKS_PER_PRIO > MVAR_BITS
#error POSCFG_TASKS_PER_PRIO can not exceed MVAR_BITS
#endif
#if (POSCFG_TASKS_PER_PRIO < 2) && (POSCFG_ROUNDROBIN != 0)
#error POSCFG_TASKS_PER_PRIO must be at least 2
#endif
#if (POSCFG_TASKS_PER_PRIO != 1) && (POSCFG_ROUNDROBIN == 0)
#error POSCFG_TASKS_PER_PRIO must be 1 when round robin scheduling is disabled
#endif
#if ((POSCFG_TASKS_PER_PRIO % 3) == 0) || ((POSCFG_TASKS_PER_PRIO % 5) == 0)
#error POSCFG_TASKS_PER_PRIO must be 1/2/4/8/16 or 32
#endif
#if (POSCFG_REALTIME_PRIO >= POSCFG_MAX_PRIO_LEVEL) && (POSCFG_ROUNDROBIN != 0)
#error POSCFG_REALTIME_PRIO must be less than POSCFG_MAX_PRIO_LEVEL
#endif
#if POSCFG_FEATURE_MSGBOXES != 0
#if POSCFG_MAX_MESSAGES < 2
#error POSCFG_MAX_MESSAGES must be at least 2
#endif
#if POSCFG_MSG_BUFSIZE < 1
#error POSCFG_MSG_BUFSIZE must be at least 1
#endif
#endif
#if (POSCFG_FEATURE_TIMER != 0) && (POSCFG_MAX_TIMER == 0)
#error POSCFG_MAX_TIMER must be at least 1
#endif
#if (POSCFG_TASKSTACKTYPE < 0) || (POSCFG_TASKSTACKTYPE > 2)
#error POSCFG_TASKSTACKTYPE must be 0, 1 or 2
#endif
#if POSCFG_ALIGNMENT == 0
#undef POSCFG_ALIGNMENT
#define POSCFG_ALIGNMENT  1
#else
#if ((POSCFG_ALIGNMENT % 3) == 0) || ((POSCFG_ALIGNMENT % 5) == 0)
#error POSCFG_ALIGNMENT must be 1/2/4/8/16/32/64/128/256 etc.
#endif
#endif
#if POSCFG_FEATURE_SOFTINTS != 0
#if POSCFG_SOFTINTERRUPTS == 0
#error POSCFG_SOFTINTERRUPTS must be greater than 0
#endif
#if POSCFG_SOFTINTQUEUELEN < 2
#error POSCFG_SOFTINTQUEUELEN must be at least 2
#endif
#endif


/* parameter reconfiguration */
#if POSCFG_FEATURE_SEMAPHORES == 0
#undef POSCFG_FEATURE_SEMADESTROY
#define POSCFG_FEATURE_SEMADESTROY  0
#endif
#if POSCFG_FEATURE_FLAGS == 0
#undef POSCFG_FEATURE_FLAGDESTROY
#define POSCFG_FEATURE_FLAGDESTROY  0
#endif
#if POSCFG_FEATURE_MUTEXES == 0
#undef POSCFG_FEATURE_MUTEXDESTROY
#define POSCFG_FEATURE_MUTEXDESTROY  0
#else
#if (POSCFG_FEATURE_MUTEXDESTROY != 0) && (POSCFG_FEATURE_SEMADESTROY == 0)
#undef POSCFG_FEATURE_SEMADESTROY
#define POSCFG_FEATURE_SEMADESTROY 1
#endif
#endif
#if POSCFG_FEATURE_MSGBOXES != 0
#define MSGBOXEVENTS  2
#else
#define MSGBOXEVENTS  0
#endif
#if (POSCFG_FEATURE_MSGBOXES != 0) && (POSCFG_FEATURE_GETTASK == 0)
#undef POSCFG_FEATURE_GETTASK
#define POSCFG_FEATURE_GETTASK 1
#endif
#if (POSCFG_FEATURE_SEMAWAIT != 0) || (POSCFG_FEATURE_MSGWAIT != 0)
#define SYS_TASKDOUBLELINK  1
#else
#define SYS_TASKDOUBLELINK  0
#endif
#define SYS_EVENTS_USED  \
      (POSCFG_FEATURE_MUTEXES | POSCFG_FEATURE_MSGBOXES | POSCFG_FEATURE_FLAGS)
#define SYS_FEATURE_EVENTS  (POSCFG_FEATURE_SEMAPHORES | SYS_EVENTS_USED)
#define SYS_FEATURE_EVENTFREE  (POSCFG_FEATURE_SEMADESTROY | \
          POSCFG_FEATURE_MUTEXDESTROY | POSCFG_FEATURE_FLAGDESTROY)
#if (POSCFG_FEATURE_MSGBOXES != 0) && (POSCFG_FEATURE_EXIT != 0)
#undef  SYS_FEATURE_EVENTFREE
#define SYS_FEATURE_EVENTFREE  1
#endif
#ifndef POSCFG_FEATURE_LARGEJIFFIES
#if MVAR_BITS < 16
#define POSCFG_FEATURE_LARGEJIFFIES  1
#else
#define POSCFG_FEATURE_LARGEJIFFIES  0
#endif
#endif
#endif /* DOX!=0 */



/*---------------------------------------------------------------------------
 *  SOME DEFINITIONS AND MACROS
 *-------------------------------------------------------------------------*/

#ifndef _POSCORE_C
#define POSEXTERN extern
#else
#define POSEXTERN
#endif

#if POSCFG_ROUNDROBIN == 0
#define SYS_TASKTABSIZE_X  MVAR_BITS
#define SYS_TASKTABSIZE_Y  ((POSCFG_MAX_PRIO_LEVEL+MVAR_BITS-1)/MVAR_BITS)
#else
#define SYS_TASKTABSIZE_X  POSCFG_TASKS_PER_PRIO
#define SYS_TASKTABSIZE_Y  POSCFG_MAX_PRIO_LEVEL
#endif

#define SYS_TASKSTATE (POSCFG_FEATURE_TASKUNUSED | POSCFG_FEATURE_MSGBOXES)

#if POSCFG_LOCK_USEFLAGS != 0
#define POS_LOCKFLAGS   POSCFG_LOCK_FLAGSTYPE flags
#else
#define POS_LOCKFLAGS   do { } while(0)
#endif

#define POSTASKSTATE_UNUSED      0
#define POSTASKSTATE_ZOMBIE      1
#define POSTASKSTATE_ACTIVE      2

#define INFINITE   ((UINT_t)~0)



/*---------------------------------------------------------------------------
 *  ERROR CODES
 *-------------------------------------------------------------------------*/

/** @defgroup errcodes Error Codes
 * Most pico]OS functions return a signed integer value describing the
 * execution status of the function. Generally, a negative value denotes
 * an error, zero means success (no error), and a positive value may
 * be returned as result of a successful operation.
 * To check if an operation was successful, you need only to test if the
 * returned value is positiv ( status >= 0 ).  For example, if you would
 * like to test if a function failed because the system ran out of memory,
 * you may test the status against the (negativ) value -ENOMEM.
 * @{
 */

/** Error Code: No Error (the operation was successful) */
#define EOK         0

/** Error Code: The operation failed. */
#define EFAIL       1

/** Error Code: The System ran out of memory. */
#define ENOMEM      2

/** Error Code: The given argument is wrong or inacceptable. */
#define EARG        3

/** Error Code: The operation is forbidden at the current operation stage. */
#define EFORB       4

/** @} */



/*---------------------------------------------------------------------------
 *  MACROS
 *-------------------------------------------------------------------------*/

/** Convert milliseconds into HZ timer ticks.
 * This macro is used to convert milliseconds into the timer tick
 * rate (see ::HZ define).
 * Example:  Use  ::posTaskSleep(MS(1000))  to sleep 1000 ms.
 */
#if (DOX==0) && (HZ <= 1000)
#define MS(msec)  (((UINT_t)(msec)<(1000/HZ)) ? \
                    ((UINT_t)1) : ((UINT_t)((1L*HZ*(UINT_t)(msec))/1000)))
#else
#define MS(msec)  ((UINT_t)((1L*HZ*(UINT_t)(msec))/1000))
#endif



/*---------------------------------------------------------------------------
 *  DATA TYPES
 *-------------------------------------------------------------------------*/

/** Signed machine variable type.
 * This variable type is the fastest for
 * the target architecture.
 * @sa UVAR_t
 */
typedef signed MVAR_t     VAR_t;

/** Unsigned machine variable type.
 * This variable type is the fastest for
 * the target architecture.
 * @sa VAR_t
 */
typedef unsigned MVAR_t   UVAR_t;

#ifndef MINT_t
#define MINT_t int
#endif
/** Signed integer.
 * The bit size can be changed by the user
 * by defining <b>MINT_t</b> to something other than <i>int</i>
 * in the pico]OS configuration file.
 * This integer type is used by the operating system e.g.
 * for semaphore counters and timer.
 */
typedef signed MINT_t     INT_t;

/** Unsigned integer.
 * The bit size can be changed by the user
 * by defining <b>MINT_t</b> to something other than <i>int</i>
 * in the pico]OS configuration file.
 * This integer type is used by the operating system e.g.
 * for semaphore counters and timer.
 */
typedef unsigned MINT_t   UINT_t;

#ifndef MPTR_t
#define MPTR_t long
#endif
/** Memory pointer type.
 * This variable type is an integer with the width
 * of the address lines of memory architecure. The bit width
 * is equal to the width of a void-pointer.
 * This variable type is needed by the operating system for
 * lossless typecasting of void pointers to integers.
 * <b>MPTR_t</b> is a define that is set in the pico]OS configuration
 * file. When <b>MPTR_t</b> is not set, it defaults to <i>long</i>.
 */
typedef unsigned MPTR_t   MEMPTR_t;

#if (DOX!=0) || (POSCFG_FEATURE_LARGEJIFFIES == 0)
/** Signed type of JIF_t. */
typedef VAR_t             SJIF_t;
/** Timer counter type. Can be <b>UVAR_t</b> or <b>UINT_t</b>. */
typedef UVAR_t            JIF_t;
#else
typedef INT_t             SJIF_t;
typedef UINT_t            JIF_t;
#endif

/** Generic function pointer. */
typedef void (*POSTASKFUNC_t)(void* arg);

/** Software interrupt callback function pointer. */
typedef void (*POSINTFUNC_t)(UVAR_t arg);

#if (DOX!=0) ||(POSCFG_FEATURE_IDLETASKHOOK != 0)
/** Idle task function pointer */
typedef void (*POSIDLEFUNC_t)(void);
#endif

/** Handle to a semaphore object. */
typedef void*  POSSEMA_t;

/** Handle to a mutex object. */
typedef void*  POSMUTEX_t;

/** Handle to a flag object. */
typedef void*  POSFLAG_t;

/** Handle to a timer object. */
typedef void*  POSTIMER_t;

/** 
 * Task environment structure.
 * Most members of this structure are private, and are hidden from the user.
 * The user can add its own members to the structure. For this purpose the
 * user must define the macro <b>POS_USERTASKDATA</b> in the pico]OS
 * configuration file. Here is an example of this macro:<br>
 * 
 * #define POS_USERTASKDATA \ <br>
 * &nbsp; &nbsp; void  *stackptr; \ <br>
 * &nbsp; &nbsp; unsigned short  stack[FIXED_STACK_SIZE]; \ <br>
 * &nbsp; &nbsp; int   errno;<br>
 *
 * Note that the stackptr variable is required by most of the architecture
 * ports. The stack array is an example of how to include the stack frame
 * into the task environment structure (e.g. when ::POSCFG_TASKSTACKTYPE
 * is defined to 2).
 */
typedef struct POSTASK_s *POSTASK_t; /* forward declaration */



/*---------------------------------------------------------------------------
 *  DEFINITIONS FOR GENERIC "findbit" FUNCTION  (file fbit_gen.c)
 *-------------------------------------------------------------------------*/

#ifndef POSCFG_FBIT_USE_LUTABLE
#define POSCFG_FBIT_USE_LUTABLE  0
#endif
#if (POSCFG_FBIT_USE_LUTABLE > 1) && (POSCFG_ROUNDROBIN == 0)
#undef POSCFG_FBIT_USE_LUTABLE
#define POSCFG_FBIT_USE_LUTABLE 1
#endif
#ifndef FINDBIT
#if POSCFG_FBIT_USE_LUTABLE == 1
#if POSCFG_ROUNDROBIN == 0
#ifndef _FBIT_GEN_C
extern VAR_t const p_pos_fbittbl[256];
#endif
#define FINDBIT(x)  p_pos_fbittbl[x] 
#else
UVAR_t p_pos_findbit(const UVAR_t bitfield, UVAR_t rrOffset);
#define FINDBIT(x, o)  p_pos_findbit(x, o)
#endif
#elif POSCFG_FBIT_USE_LUTABLE == 2
#ifndef _FBIT_GEN_C
extern VAR_t const p_pos_fbittbl_rr[8][256];
#endif
#define FINDBIT(x, o)  p_pos_fbittbl_rr[o][x]
#endif
#endif /* !FINDBIT */



/*---------------------------------------------------------------------------
 *  GLOBAL VARIABLES
 *-------------------------------------------------------------------------*/

/**
 * Global task variable.
 * This variable points to the environment structure of the currently
 * active task.
 * @note  Only the context switch functions ::p_pos_softContextSwitch,
 *        ::p_pos_intContextSwitch and ::p_pos_startFirstContext
 *        need to access this variable.
 * @sa posNextTask_g
 */
POSEXTERN  POSTASK_t posCurrentTask_g;

/**
 * Global task variable.
 * This variable points to the environment structure of the next task
 * that shall be scheduled.
 * @note  The context switch functions ::p_pos_softContextSwitch and
 *        ::p_pos_intContextSwitch must copy the content of this variable
 *        into the ::posCurrentTask_g variable.
 * @sa posCurrentTask_g
 */
POSEXTERN  POSTASK_t posNextTask_g;

/**
 * Global flag variable.
 * This variable is nonzero when the CPU is currently executing an
 * interrupt service routine.
 * @note  Only the architecture specific ISR functions need to access
 *        this variable.
 */
#ifndef _POSCORE_C
POSEXTERN  UVAR_t    posInInterrupt_g;
#else
POSEXTERN  UVAR_t    posInInterrupt_g = 1;
#endif

/**
 * Global flag variable.
 * This variable is nonzero when the operating system is initialized
 * and running.
 * @note  Only the architecture specific ISR functions need to access
 *        this variable.
 */
#ifndef _POSCORE_C
POSEXTERN  UVAR_t    posRunning_g;
#else
POSEXTERN  UVAR_t    posRunning_g = 0;
#endif



/*---------------------------------------------------------------------------
 *  PROTOTYPES OF INTERNAL FUNCTIONS
 *-------------------------------------------------------------------------*/

/** @defgroup port pico]OS Porting Information
 * <h3>General Information</h3>
 * <h4>Choose the best type of stack management</h4>
 * The operating system can be easily ported to other architectures,
 * it can be ported to very small 8 bit architectures with low memory
 * and to 32 bit architectures with lots of memory. To keep the
 * porting as simple as possible, there are only a couple of functions
 * that must be adapted to the architecute.
 * Befor you start porting the operating system to your architecture,
 * you must choose a stack management type. You have the choice
 * between:<br>
 * 
 * ::POSCFG_TASKSTACKTYPE <b>= 0</b><br>
 * The stack memory is provided by the user. This is the best choice for
 * very small architectures with low memory.<br>
 *
 * ::POSCFG_TASKSTACKTYPE <b>= 1</b><br>
 * The stack memory is dynamically allocated by the architecture dependent
 * code of the operating system. The size of the stack frame is variable
 * and can be choosen by the user who creates the task. This is the best
 * choice for big architectures with lots of memory.<br>
 * 
 * ::POSCFG_TASKSTACKTYPE <b>= 2</b><br>
 * The stack memory is dynamically allocated by the architecture dependent
 * code of the operating system. The size of the stack frame is fixed and
 * can not be changed by the user. This may be an alternative to type 0,
 * it is a little bit more user friendly.<br>
 * 
 * Here is a list of the functions that are architecture specific and 
 * must be ported:<br>
 * ::p_pos_initTask, ::p_pos_startFirstContext, ::p_pos_softContextSwitch,
 * ::p_pos_intContextSwitch.<br>
 *
 * If you choose ::POSCFG_TASKSTACKTYPE <b>= 2</b> or <b>3</b>, you must
 * also provide the function ::p_pos_freeStack.<br><br><br>
 *
 * <h4>Get more speed with optimized "findbit" function</h4>
 * If your application is critical in performance, you may also provide
 * an assembler version of the function "findbit".
 * There are two different function prototypes possible. The simple
 * prototype for the standard scheduling scheme
 * (::POSCFG_ROUNDROBIN <b>= 0</b>) is<br>
 *
 * <b>UVAR_t ::p_pos_findbit(const UVAR_t bitfield);</b>
 *
 * The prototype for a findbit function that supports round robin
 * scheduling (::POSCFG_ROUNDROBIN <b>= 1</b>) is<br>
 *
 * <b>UVAR_t ::p_pos_findbit(const UVAR_t bitfield, UVAR_t rrOffset);</b><br>
 *
 * The function gets a bitfield as input, and returns the number of the
 * right most set bit (that is the number of the first lsb that is set).
 * If round robin is enabled, the function takes an offset as second
 * parameter. The offset is the position where the function starts to
 * search the first set bit. The function scans the bitfield always from
 * right to left, starting with the bit denoted by the offset. The bitfield
 * is seen as circle, when the rightmost bit is not set the function
 * must continue scanning the leftmost bit (wrap around), so all bits
 * of the field are scanned.<br>
 * It is possible to implement the findbit mechanism as look up table.
 * For this purpose you can define the macro <b>FINDBIT</b>. Please see the
 * header file picoos.h (search for the word ::POSCFG_FBIT_USE_LUTABLE)
 * and the source file fbit_gen.c for details.<br>
 *
 * <br><h3>Assembler Functions</h3>
 * Unfortunately, not the whole operating system can be written in C.
 * The platform port must be written in assembly language. I tried to
 * keep the assembly part of the RTOS as small as possible. But there
 * are three assembly functions left, that are needed for doing
 * the context switching:<br>
 *
 *   - ::p_pos_startFirstContext
 *   - ::p_pos_softContextSwitch
 *   - ::p_pos_intContextSwitch
 *
 * The operating system requires also a timer interrupt that is used
 * to cut the task execution time into slices. Hardware interrupts
 * must comply with some conventions to be compatible to pico]OS.
 * So the fourth thing you need to write in assember is a framework
 * for hardware interrupts.<br>
 *
 * The diagram shows the assembler functions in logical structure.
 * At the left side I have drawn a normal interrupt service routine
 * for reference.<br><br><br>
 *
 * <p><img src="../pic/portfc1.png" align="middle" border=0></p><br>
 *
 * The context switching (multitasking) is done by simply swaping the
 * stack frame when an interrupt service routine (eg. the timer interrupt)
 * is left. But it must also be possible for a task to give of processing
 * time without the need of an interrupt.
 * This is done by the function ::p_pos_softContextSwitch
 * at the right side in the diagram. Since this function is not called
 * by a processor interrupt, it must build up an ISR compatible
 * stack frame by itself. Note that the second part of this function is
 * equal to the function ::p_pos_intContextSwitch, so the function must be
 * terminated by an return-from-interrupt instruction, even if the
 * function was called from a C-routine.<br>
 *
 * For completeness, the next diagram shows at its left side how
 * the function ::p_pos_startFirstContext works. Again, this function
 * looks like the lower part of the funtion ::p_pos_intContextSwitch
 * in the diagram above. In the middle you can see how the timer
 * interrupt routine must look like.<br><br><br>
 *
 * <p><img src="../pic/portfc2.png" align="middle" border=0></p><br>
 *
 * There is a special interrupt handling needed when  interrupts are
 * interruptable on your system. To prevent a deadlock situation (that
 * is, an ISR would be called again and again until the stack flows over),
 * a counting flag variable is exported by pico]OS: ::posInInterrupt_g.
 * This variable contains the value zero if no interrupt is running yet.
 * And only if no other interrupt is running, the ISR must save the
 * stack pointer to the task environment structure where ::posCurrentTask_g
 * points to. This behaviour is shown at the right side in the
 * diagram above.<br>
 *
 * Note that interrupt service routines need some stack space to be
 * able to do their work - in the discussed configuration every ISR
 * would take some stack memory from the stack frame of the currently
 * active task. But this may be a problem at platforms that are low
 * on memory - it would be to expensive to increase every tasks stack
 * frame by the count of bytes an ISR would need. In this case, you can
 * set up a special stackframe that is only used by interrupt service
 * routines. The diagram below shows the small changes to the ISRs
 * discussed above. But attention - this method is only applicable on
 * platforms where interrupts can not interrupt each other.<br><br><br>
 *
 * <p><img src="../pic/portfc3.png" align="middle" border=0></p><br>
 *
 * @{
 */

/* findbit  function or macro definition */
#if (DOX!=0) || (POSCFG_ROUNDROBIN == 0)
#ifdef FINDBIT
#define POS_FINDBIT(bf)          FINDBIT(bf, 0)
#define POS_FINDBIT_EX(bf, ofs)  FINDBIT(bf, 0)
#else  /* FINDBIT */
UVAR_t p_pos_findbit(const UVAR_t bitfield);  /* arch_c.c */
#define POS_FINDBIT(bf)          p_pos_findbit(bf)
#define POS_FINDBIT_EX(bf, ofs)  p_pos_findbit(bf)
#endif /* FINDBIT */
#else  /* POSCFG_ROUNDROBIN */
#ifdef FINDBIT
#define POS_FINDBIT(bf)          FINDBIT(bf, 0)
#define POS_FINDBIT_EX(bf, ofs)  FINDBIT(bf, ofs)
#else  /* FINDBIT */
UVAR_t p_pos_findbit(const UVAR_t bitfield, UVAR_t rrOffset);  /* arch_c.c */
#define POS_FINDBIT(bf)          p_pos_findbit(bf, 0)
#define POS_FINDBIT_EX(bf, ofs)  p_pos_findbit(bf, ofs)
#endif /* FINDBIT */
#endif /* POSCFG_ROUNDROBIN */

#if DOX!=0
/**
 * Bit finding function.
 * This function is called by the operating system to find the
 * first set bit in a bitfield. See the file fbit_gen.c for an example.
 * @param   bitfield  This is the bitfield that shall be scanned.
 * @return  the number of the first set bit (scanning begins with the lsb).
 * @note    ::POSCFG_ROUNDROBIN <b>must be defined to 0</b>
 *          to have this format of the function compiled in.
 */
UVAR_t      p_pos_findbit(const UVAR_t bitfield);  

/**
 * Bit finding function.
 * This function is called by the operating system to find the
 * first set bit in a bitfield. See the file fbit_gen.c for an example.
 * @param   bitfield  This is the bitfield that shall be scanned.
 * @param   rrOffset  Offset into the bitfield. Scanning begins here.
 * @return  the number of the first set bit (scanning begins with the lsb).
 * @note    ::POSCFG_ROUNDROBIN <b>must be defined to 1</b>
 *          to have this format of the function compiled in.
 */
UVAR_t      p_pos_findbit(const UVAR_t bitfield, UVAR_t rrOffset); 
#endif

#if (DOX!=0) || (POSCFG_CALLINITARCH != 0)
/**
 * Architecture port initialization.
 * This function is called from the ::posInit function to initialize
 * the architecture specific part of the operating system.
 * @note    ::POSCFG_CALLINITARCH must be defined to 1
 *          when ::posInit shall call this function.<br>
 *          This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          A timer interrupt should be initialized in the funcion
 *          ::p_pos_startFirstContext.
 */
void        p_pos_initArch(void);
#endif

#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 0)
/**
 * Task initialization function.
 * This function is called by the operating system to
 * initialize the stack frame of a new task.
 * See the available port source files for an
 * example on how to write this function.
 * @param   task        pointer to the task environment structure.
 * @param   stackstart  pointer to the start of the stack memory.
 * @param   funcptr     pointer to the first function that shall
 *                      be executed by the new task.
 * @param   funcarg     argument that should be passed to the
 *                      first function.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 0</b>
 *          to have this format of the function compiled in.<br>
 *          This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          The processor interrupts are disabled when this function
 *          is called.
 */
void        p_pos_initTask(POSTASK_t task, void *stackstart,
                           POSTASKFUNC_t funcptr,
                           void *funcarg);  /* arch_c.c */
#endif
#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 1)
/**
 * Task initialization function.
 * This function is called by the operating system to
 * initialize the stack frame of a new task.
 * See the available port source files for an
 * example on how to write this function.
 * @param   task        pointer to the task environment structure.
 * @param   stacksize   size of the stack memory for the new task.
 *                      The stack memory may be allocated
 *                      dynamically from within this function.
 * @param   funcptr     pointer to the first function that shall
 *                      be executed by the new task.
 * @param   funcarg     argument that should be passed to the
 *                      first function.
 * @return  zero on success. A negative value should be returned
 *          to denote an error (e.g. out of stack memory).
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 1</b>
 *          to have this format of the function compiled in.<br>
 *          This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          The processor interrupts are disabled when this function
 *          is called.
 * @sa      p_pos_freeStack
 */
VAR_t       p_pos_initTask(POSTASK_t task, UINT_t stacksize,
                           POSTASKFUNC_t funcptr,
                           void *funcarg);  /* arch_c.c */
/**
 * Stack free function.
 * This function is called by the operating system to
 * free a stack frame that was set up by the function
 * ::p_pos_initTask.
 * See the available port source files for an
 * example on how to write this function.
 * @param   task  pointer to the task environment structure.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 1 or 2</b>
 *          to have this format of the function compiled in.<br>
 *          This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          The processor interrupts are disabled when this function
 *          is called; but the processor may still write some bytes to
 *          the stack frame after this function was called and before
 *          the interrupts are enabled again.
 * @sa      p_pos_initTask
 */
void        p_pos_freeStack(POSTASK_t task);/* arch_c.c */
#endif

#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 2)
/**
 * Task initialization function.
 * This function is called by the operating system to
 * initialize the stack frame of a new task.
 * This function is responsible to allocate the stack memory and
 * to store the pointer of the stack frame into the task environment.
 * See the available port source files for an
 * example on how to write this function.
 * @param   task        pointer to the task environment structure.
 * @param   funcptr     pointer to the first function that shall
 *                      be executed by the new task.
 * @param   funcarg     argument that should be passed to the
 *                      first function.
 * @return  zero on success. A negative value should be returned
 *          to denote an error (e.g. out of stack memory).
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 2</b>
 *          to have this format of the function compiled in.<br>
 *          This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          The processor interrupts are disabled when this function
 *          is called.
 * @sa      p_pos_freeStack
 */
VAR_t       p_pos_initTask(POSTASK_t task, POSTASKFUNC_t funcptr,
                           void *funcarg);  /* arch_c.c */

/**
 * Stack free function.
 * This function is called by the operating system to
 * free a stack frame that was set up by the function
 * ::p_pos_initTask.
 * See the available port source files for an
 * example on how to write this function.
 * @param   task  pointer to the task environment structure.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 1 or 2</b>
 *          to have this format of the function compiled in.<br>
 *          This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          The processor interrupts are disabled when this function
 *          is called.
 * @sa      p_pos_initTask
 */
void        p_pos_freeStack(POSTASK_t task);/* arch_c.c */
#endif

/**
 * Context switch function.
 * This function is called by the operating system to
 * start the multitasking. The function has
 * to restore the first context from stack memory.
 * See the available port source files for an
 * example on how to write this function.
 * @note    This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          The processor interrupts are disabled when this function
 *          is called.
 * @sa      p_pos_softContextSwitch, p_pos_intContextSwitch
 */
void        p_pos_startFirstContext(void);   /* arch_c.c */

/**
 * Context switch function.
 * This function is called by the operating system to
 * initiate a software context switch. This function has then to
 * save all volatile processor registers to stack memory, switch
 * the context variable and restore the new context from
 * stack memory. See the available port source files for an
 * example on how to write this function.
 * @note    This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.<br>
 *          The processor interrupts are disabled when this function
 *          is called.
 * @sa      p_pos_intContextSwitch, p_pos_startFirstContext
 */
void        p_pos_softContextSwitch(void);   /* arch_c.c */

/**
 * Context switch function.
 * This function is called by the operating system to initiate a
 * context switch from interrupt level. This function has then
 * to switch the context variable and restore the new context
 * from stack memory. See the available port source files for an
 * example on how to write this function.
 * @note    This function is not part of the pico]OS. It must be
 *          provided by the user, since it is architecture specific.
 *          The processor interrupts are disabled when this function
 *          is called.
 * @sa      p_pos_softContextSwitch, p_pos_startFirstContext
 */
void        p_pos_intContextSwitch(void);    /* arch_c.c */


/**
 * Interrupt control function.
 * This function must be called from an interrupt service routine
 * to show the operating system that an ISR is currently running.
 * This function must be called first before other operating system
 * functions can be called from within the ISR.
 * @sa      c_pos_intExit, c_pos_timerInterrupt
 */
void        c_pos_intEnter(void);            /* picoos.c */

/**
 * Interrupt control function.
 * This function must be called from an interrupt service routine
 * to show the operating system that the ISR is going to complete its
 * work and no operating system functions will be called any more
 * from within the ISR.
 * @sa      c_pos_intEnter, c_pos_timerInterrupt
 */
void        c_pos_intExit(void);             /* picoos.c */

/**
 * Timer interrupt control function.
 * This function must be called periodically from within a timer
 * interrupt service routine. The whole system timing is derived
 * from this timer interrupt.<br>
 *
 * A timer ISR could look like this:<br>
 *
 * PUSH ALL; // push all registers to stack<br>
 * if (posInInterrupt_g == 0) saveStackptrToCurrentTaskEnv();<br>
 * c_pos_intEnter();<br>
 * c_pos_timerInterrupt();<br>
 * c_pos_intExit();<br>
 * PULL ALL; // pull all registers from stack<br>
 * RETI; // return from interrupt<br>
 *
 * @note    Any other ISR looks like this, only the function
 *          ::c_pos_timerInterrupt is replaced by an user function.<br>
 *          Dependent on the platform port, it can be necessary to
 *          evaluate the variable ::posRunning_g to ensure that the
 *          timer interrupt is not triggered when the OS is not yet
 *          running.<br>
 *          To avoid this race condintions, it is better to initialize
 *          the timer interrupt in the function ::p_pos_startFirstContext.
 * @sa      c_pos_intEnter, c_pos_intExit
 */
void        c_pos_timerInterrupt(void);      /* picoos.c */

/** @} */



/*---------------------------------------------------------------------------
 *  PROTOTYPES OF EXPORTED FUNCTIONS  (USER API)
 *-------------------------------------------------------------------------*/

/** @defgroup task User API: Task Control Functions
 * @{
 */

#if (DOX!=0) || (POSCFG_FEATURE_YIELD != 0)
/**
 * Task function.
 * This function can be called to give off processing time so other tasks
 * ready to run will be scheduled (= cooparative multitasking).
 * @note    ::POSCFG_FEATURE_YIELD must be defined to 1
 *          to have this function compiled in.
 * @sa      posTaskSleep
 */
void        posTaskYield(void);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_SLEEP != 0)
/**
 * Task function.
 * Delay task execution for a couple of timer ticks.
 * @param   ticks  delay time in timer ticks
 *          (see ::HZ define and ::MS macro)
 * @note    ::POSCFG_FEATURE_SLEEP must be defined to 1
 *          to have this function compiled in.<br>
 *          It is not guaranteed that the task will proceed
 *          execution exactly when the time has elapsed.
 *          A higher priorized task or a task having the same
 *          priority may steal the processing time.
 *          Sleeping a very short time is inaccurate.
 * @sa      posTaskYield, HZ, MS
 */
void        posTaskSleep(UINT_t ticks);
#endif

#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 0)
/**
 * Task function.
 * Creates a new task. The stack memory is managed by the user.
 * @param   funcptr     pointer to the function that shall be executed
 *                      by the new task.
 * @param   funcarg     optional argument passed to function.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   stackstart  pointer to the stack memory for the new task.
 * @return  handle to the task. NULL is returned when the
 *          task could not be created.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 0</b>
 *          to have this format of the function compiled in.
 * @sa      posTaskExit
 */
POSTASK_t   posTaskCreate(POSTASKFUNC_t funcptr, void *funcarg,
                          VAR_t priority, void *stackstart);

/**
 * Operating System Initialization.
 * This function initializes the operating system and starts the
 * first tasks: The idle task and the first user task.
 * @param   firstfunc   pointer to the first task function that
 *                      will run in the multitasking environment.
 * @param   funcarg     optional argument passed to the first task.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   stackFirstTask  pointer to the stack memory for the first task.
 * @param   stackIdleTask   pointer to the stack memory for the idle task.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 0</b>
 *          to have this format of the function compiled in.
 */
void        posInit(POSTASKFUNC_t firstfunc, void *funcarg, VAR_t priority,
                    void *stackFirstTask, void *stackIdleTask);
#endif
#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 1)
/**
 * Task function.
 * Creates a new task. The stack memory is managed by the achitecture
 * specific portion of the operating system, the size can be set by the user.
 * @param   funcptr     pointer to the function that shall be executed
 *                      by the new task.
 * @param   funcarg     optional argument passed to function.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   stacksize   size of the stack memory. This parameter is
 *                      passed to the architecture specific portion of
 *                      the operating system.
 * @return  handle to the task. NULL is returned when the
 *          task could not be created.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 1</b>
 *          to have this format of the function compiled in.
 * @sa      posTaskExit
 */
POSTASK_t   posTaskCreate(POSTASKFUNC_t funcptr, void *funcarg,
                          VAR_t priority, UINT_t stacksize);

/**
 * Operating System Initialization.
 * This function initializes the operating system and starts the
 * first tasks: The idle task and the first user task.
 * @param   firstfunc   pointer to the first task function that
 *                      will run in the multitasking environment.
 * @param   funcarg     optional argument passed to the first task.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   taskStackSize  size of the stack memory for the first task.
 * @param   idleStackSize  size of the stack memory for the idle task.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 1</b>
 *          to have this format of the function compiled in.
 */
void        posInit(POSTASKFUNC_t firstfunc, void *funcarg, VAR_t priority,
                    UINT_t taskStackSize, UINT_t idleStackSize);
#endif
#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 2)
/**
 * Task function.
 * Creates a new task. The stack memory is fixed, its size is set by
 * the architecture specific portion of the operating system.
 * @param   funcptr     pointer to the function that shall be executed
 *                      by the new task.
 * @param   funcarg     optional argument passed to function.
 * @param   priority    task priority. Must be in the range
 *                      0.. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @return  handle to the task. NULL is returned when the
 *          task could not be created.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 2</b>
 *          to have this format of the function compiled in.
 * @sa      posTaskExit
 */
POSTASK_t   posTaskCreate(POSTASKFUNC_t funcptr, void *funcarg,
                          VAR_t priority);

/**
 * Operating System Initialization.
 * This function initializes the operating system and starts the
 * first tasks: The idle task and the first user task.
 * @param   firstfunc   pointer to the first task function that
 *                      will run in the multitasking environment.
 * @param   funcarg     optional argument passed to the first task.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 2</b>
 *          to have this format of the function compiled in.
 */
void        posInit(POSTASKFUNC_t firstfunc, void *funcarg, VAR_t priority);

#endif


#if (DOX!=0) || (POSCFG_FEATURE_EXIT != 0)
/**
 * Task function.
 * Terminate execution of a task.
 * @note    ::POSCFG_FEATURE_EXIT must be defined to 1 
 *          to have this function compiled in.
 * @sa      posTaskCreate
 */
void        posTaskExit(void);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_GETTASK != 0)
/**
 * Task function.
 * Get the handle to the currently running task.
 * @return  the task handle.
 * @note    ::POSCFG_FEATURE_GETTASK must be defined to 1 
 *          to have this function compiled in.
 * @sa      posTaskCreate, posTaskSetPriority
 */
POSTASK_t   posTaskGetCurrent(void);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_TASKUNUSED != 0)
/**
 * Task function.
 * Tests if a task is yet in use by the operating system.
 * This function can be used to test if a task has been
 * fully terminated (and the stack memory is no more in use).
 * @param   taskhandle  handle to the task.
 * @return  1 (=true) when the task is unused. If the task
 *          is still in use, zero is returned.
 *          A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_TASKUNUSED must be defined to 1 
 *          to have this function compiled in.
 * @sa      posTaskCreate, posTaskExit
 */
VAR_t       posTaskUnused(POSTASK_t taskhandle);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_SETPRIORITY != 0)
/**
 * Task function.
 * Change the priority of a task. Note that in a non-roundrobin
 * scheduling environment every priority level can only exist once.
 * @param   taskhandle  handle to the task.
 * @param   priority    new priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_SETPRIORITY must be defined to 1 
 *          to have this function compiled in.
 * @sa      posTaskGetPriority, posTaskGetCurrent, posTaskCreate
 */
VAR_t       posTaskSetPriority(POSTASK_t taskhandle, VAR_t priority);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_GETPRIORITY != 0)
/**
 * Task function.
 * Get the priority of a task.
 * @param   taskhandle  handle to the task.
 * @return  the priority of the task. A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_GETPRIORITY must be defined to 1 
 *          to have this function compiled in.
 * @sa      posTaskSetPriority, posTaskGetCurrent, posTaskCreate
 */
VAR_t       posTaskGetPriority(POSTASK_t taskhandle);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_INHIBITSCHED != 0)
/**
 * Task function.
 * Locks the scheduler. When this function is called, no task switches
 * will be done any more, until the counterpart function ::posTaskSchedUnlock
 * is called. This function is usefull for short critical sections that
 * require exclusive access to variables. Note that interrupts still
 * remain enabled.
 * @note    ::POSCFG_FEATURE_INHIBITSCHED must be defined to 1 
 *          to have this function compiled in.
 * @sa      posTaskSchedUnlock
 */
void        posTaskSchedLock(void);

/**
 * Task function.
 * Unlocks the scheduler. This function is called to leave a critical section.
 * If a context switch request is pending, the context switch will happen
 * directly after calling this function.
 * @note    ::POSCFG_FEATURE_INHIBITSCHED must be defined to 1 
 *          to have this function compiled in.
 * @sa      posTaskSchedLock
 */
void        posTaskSchedUnlock(void);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_IDLETASKHOOK != 0)
/**
 * Task function.
 * Install or remove an optional idle task hook function.
 * The hook function is called every time the system is idle.
 * It is possible to use this hook to implement your own idle task;
 * in this case the function does not need to return to the system.
 * You may insert a call to ::posTaskYield into your idle task loop
 * to get a better task performance.
 * @param   idlefunc  function pointer to the new idle task handler.
 *                    If this parameter is set to NULL, the idle
 *                    task function hook is removed again.
 * @note    ::POSCFG_FEATURE_IDLETASKHOOK must be defined to 1 
 *          to have this function compiled in.
 */
void posInstallIdleTaskHook(POSIDLEFUNC_t idlefunc);
#endif

/** @} */

/*-------------------------------------------------------------------------*/

#if (DOX!=0) || (SYS_FEATURE_EVENTS != 0)
/** @defgroup sema User API: Semaphore Functions
 * Semaphores are basically used for task synchronization.
 * Task synchronization means that only a defined number of tasks can
 * execute a piece of code. Usually, a semaphore is initialized with
 * the value 1, so only one task can hold the semaphore at a time
 * (Please read the chapter about the mutex functions also if you
 * are interested in task synchronization).<br>
 * The second thing semaphores can be used for is sending signals
 * to waiting tasks. Imagine you have an interrupt service routine
 * that is triggered every time when a big chunk of data is available
 * on a device. The data is to big to process them directly in the ISR.
 * The ISR will only trigger a semaphore (it will signalize the semaphore),
 * and a high priorized task waiting for the semaphore will be set to
 * running state and will process the data from the device. In this
 * case, the semaphore would be initialized with zero when it is created.
 * The first task requesting the semaphore would block immediately, and
 * can only proceed its work when the semaphore is triggered from outside.<br>
 * 
 * Semaphores are implemented as counters. A task requesting a semaphore
 * (via ::posSemaGet or ::posSemaWait) will decrement the counter. If the
 * counter is zero, the task willing to decrement the counter is blocked.
 * When a semaphore is signaled (via ::posSemaSignal), the counter is
 * incremented. If the counter reaches a positive, nonzero value,
 * the highest priorized task pending on the semaphore is set to
 * running state and can decrement the counter by itself.
 * @{
 */

/**
 * Semaphore function.
 * Allocates a new semaphore object.
 * @param   initcount  Initial semaphore count
 *                     (see detailed semaphore description).
 * @return  the pointer to the new semaphore object. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.
 * @sa      posSemaDestroy, posSemaGet, posSemaWait, posSemaSignal
 */
POSSEMA_t   posSemaCreate(INT_t initcount);

#if (DOX!=0) || (SYS_FEATURE_EVENTFREE != 0)
/**
 * Semaphore function.
 * Frees a no more needed semaphore object.
 * @param   sema  handle to the semaphore object.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.<br>
 *          ::POSCFG_FEATURE_SEMADESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      posSemaCreate
 */
void        posSemaDestroy(POSSEMA_t sema);
#endif

/**
 * Semaphore function.
 * This function tries to get the semaphore object. If the semaphore
 * is in nonsignalized state (that means its counter is zero or less),
 * this function blocks the task execution until the semaphore
 * gets signaled.
 * @param   sema  handle to the semaphore object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.
 * @sa      posSemaWait, posSemaSignal, posSemaCreate
 */
#if (DOX!=0) || (POSCFG_SMALLCODE == 0) || (POSCFG_FEATURE_SEMAWAIT == 0)
VAR_t       posSemaGet(POSSEMA_t sema);
#else
/* this define is for small code and it saves stack memory */
#define     posSemaGet(sema)  posSemaWait(sema, INFINITE)
#endif

/**
 * Semaphore function.
 * This function signalizes a semaphore object, that means it increments
 * the semaphore counter and sets tasks pending on the semaphore to 
 * running state, when the counter reaches a positive, nonzero value.
 * @param   sema  handle to the semaphore object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.
 * @sa      posSemaGet, posSemaWait, posSemaCreate
 */
VAR_t       posSemaSignal(POSSEMA_t sema);

#if (DOX!=0) || (POSCFG_FEATURE_SEMAWAIT != 0)
/**
 * Semaphore function.
 * This function tries to get the semaphore object. If the semaphore
 * is in nonsignalized state (that means its counter is zero or less),
 * this function blocks the task execution until the semaphore
 * gets signaled or a timeout happens.
 * @param   sema  handle to the semaphore object.
 * @param   timeoutticks  timeout in timer ticks
 *          (see ::HZ define and ::MS macro).
 *          If this parameter is set to zero, the function immediately
 *          returns. If this parameter is set to INFINITE, the
 *          function will never time out.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.<br>
 *          ::POSCFG_FEATURE_SEMAWAIT must be defined to 1
 *          to have this function compiled in.
 * @sa      posSemaGet, posSemaSignal, posSemaCreate, HZ, MS
 */
VAR_t       posSemaWait(POSSEMA_t sema, UINT_t timeoutticks);
#endif

#endif /* SYS_FEATURE_EVENTS */
/** @} */

/*-------------------------------------------------------------------------*/

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXES != 0)
/** @defgroup mutex User API: Mutex Functions
 * Mutexes are used for task synchronization. A source code
 * area that is protected by a mutex can only be executed by
 * one task at the time. The mechanism is comparable with
 * a semaphore that is initialized with a counter of 1.
 * A mutex is a special semaphore, that allows a
 * task having the mutex locked can execute the mutex lock
 * functions again and again without being blocked
 * (this is called reentrancy).
 * @{
 */

/**
 * Mutex function.
 * Allocates a new mutex object.
 * @return  the pointer to the new mutex object. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      posMutexDestroy, posMutexLock, posMutexTryLock, posMutexUnlock
 */
POSMUTEX_t  posMutexCreate(void);

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXDESTROY != 0)
/**
 * Mutex function.
 * Frees a no more needed mutex object.
 * @param   mutex  handle to the mutex object.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.<br>
 *          ::POSCFG_FEATURE_MUTEXDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      posMutexCreate
 */
void        posMutexDestroy(POSMUTEX_t mutex);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXTRYLOCK != 0)
/**
 * Mutex function.
 * Tries to get the mutex lock. This function does not block when the
 * mutex is not available, instead it returns a value showing that
 * the mutex could not be locked.
 * @param   mutex  handle to the mutex object.
 * @return  zero when the mutex lock could be set. Otherwise, when
 *          the mutex lock is yet helt by an other task, the function
 *          returns 1. A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.<br>
 *          ::POSCFG_FEATURE_MUTEXTRYLOCK must be defined to 1
 *          to have this function compiled in.
 * @sa      posMutexLock, posMutexUnlock, posMutexCreate
 */
VAR_t       posMutexTryLock(POSMUTEX_t mutex);
#endif

/**
 * Mutex function.
 * This function locks a code section so that only one task can execute
 * the code at a time. If an other task already has the lock, the task
 * requesting the lock will be blocked until the mutex is unlocked again.
 * Note that a ::posMutexLock appears always in a pair with ::posMutexUnlock.
 * @param   mutex  handle to the mutex object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      posMutexTryLock, posMutexUnlock, posMutexCreate
 */
VAR_t       posMutexLock(POSMUTEX_t mutex);

/**
 * Mutex function.
 * This function unlocks a section of code so that other tasks
 * are able to execute it.
 * @param   mutex  handle to the mutex object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      posMutexLock, posMutexTryLock, posMutexCreate
 */
VAR_t       posMutexUnlock(POSMUTEX_t mutex);

#endif /* POSCFG_FEATURE_MUTEXES */
/** @} */

/*-------------------------------------------------------------------------*/

#if (DOX!=0) || (POSCFG_FEATURE_MSGBOXES != 0)
/** @defgroup msg User API: Message Box Functions
 * Message boxes are a mechanism that is used for inter-process or,
 * in the case of pico]OS, inter-task communication. All tasks
 * can post messages to each other, and the receiving task will
 * look into its message box and continues when it
 * got a new message. Note that a message box in the pico]OS
 * can hold chunk of messages, so that no message will be lost
 * while the receiving task is still busy processing the last
 * message. <br>
 * There are two possible types of message boxes: The simple type
 * can only hold a pointer to a user supplied buffer. The other
 * message box type can hold whole messages with different sizes.
 * A message buffer must be allocated with posMessageAlloc by the
 * sending task, and the receiving task must free this buffer
 * again with ::posMessageFree. <br>
 * To select the simple message box type, you have to set
 * the define ::POSCFG_MSG_MEMORY to 0. When you want to have the
 * full message buffer support, you must set ::POSCFG_MSG_MEMORY to 1.
 * @{
 */

#if (DOX!=0) || (POSCFG_MSG_MEMORY != 0)
/**
 * Message box function.
 * Allocates a new message buffer. The maximum buffer size is
 * set at compilation time by the define ::POSCFG_MSG_BUFSIZE.
 * Usually the sending task would allocate a new message buffer, fill
 * in its data and send it via ::posMessageSend to the receiving task.
 * @return  the pointer to the new buffer. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.<br>
 *          ::POSCFG_MSG_MEMORY must be defined to 1
 *          to have this function compiled in.
 * @sa      posMessageSend, posMessageGet, posMessageFree
 */
void*       posMessageAlloc(void);

/**
 * Message box function.
 * Frees a message buffer again.
 * Usually the receiving task would call this function after
 * it has processed a message to free the message buffer again.
 * @param   buf  pointer to the message buffer that is no more used.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.<br>
 *          ::POSCFG_MSG_MEMORY must be defined to 1
 *          to have this function compiled in.
 * @sa      posMessageGet, posMessageSend, posMessageAlloc
 */
void        posMessageFree(void *buf);
#endif

/**
 * Message box function.
 * Sends a message to a task.
 * @param   buf  pointer to the message to send.
 *               If ::POSCFG_MSG_MEMORY is defined to 1,
 *               this pointer must point to the buffer
 *               that was allocated with ::posMessageAlloc.
 *               Note that this parameter must not be NULL.
 *               Exception: ::POSCFG_MSG_MEMORY = 0 and
 *               ::POSCFG_FEATURE_MSGWAIT = 0.
 * @param   taskhandle  handle to the task to send the message to.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      posMessageAlloc, posMessageGet
 */
VAR_t       posMessageSend(void *buf, POSTASK_t taskhandle);

/**
 * Message box function. Gets a new message from the message box.
 * If no message is available, the task blocks until a new message
 * is received.
 * @return  pointer to the received message. Note that the
 *          message memory must be freed again with ::posMessageFree
 *          when ::POSCFG_MSG_MEMORY is defined to 1.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      posMessageFree, posMessageAvailable,
 *          posMessageWait, posMessageSend
 */
#if (DOX!=0) || (POSCFG_SMALLCODE == 0) || (POSCFG_FEATURE_MSGWAIT == 0)
void*       posMessageGet(void);
#else
/* this define is for small code and it saves stack memory */
#define     posMessageGet()  posMessageWait(INFINITE)
#endif

/**
 * Message box function.
 * Tests if a new message is available
 * in the message box. This function can be used to prevent
 * the task from blocking.
 * @return  1 (=true) when a new message is available.
 *          Otherwise zero is returned. A negative value
 *          is returned on error.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      posMessageGet, posMessageWait
 */
VAR_t       posMessageAvailable(void);

#if (DOX!=0) || (POSCFG_FEATURE_MSGWAIT != 0)
/**
 * Message box function.
 * Gets a new message from the message box.
 * If no message is available, the task blocks until a new message
 * is received or the timeout has been reached.
 * @param   timeoutticks  timeout in timer ticks
 *          (see ::HZ define and ::MS macro).
 *          If this parameter is set to zero, the function immediately
 *          returns. If this parameter is set to INFINITE, the
 *          function will never time out.
 * @return  pointer to the received message. Note that the
 *          message memory must be freed again with posMessageFree
 *          when ::POSCFG_MSG_MEMORY is defined to 1.
 *          NULL is returned when no message was received
 *          within the specified time (=timeout).
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.<br>
 *          ::POSCFG_FEATURE_MSGWAIT must be defined to 1
 *          to have this function compiled in.
 * @sa      posMessageFree, posMessageGet, posMessageAvailable,
 *          posMessageSend, HZ, MS
 */
void*       posMessageWait(UINT_t timeoutticks);
#endif

#endif  /* POSCFG_FEATURE_MSGBOXES */
/** @} */

/*-------------------------------------------------------------------------*/

#if (DOX!=0) || (POSCFG_FEATURE_FLAGS != 0)
/** @defgroup flag User API: Flag Functions
 * Flags are one-bit semaphores. They can be used to simulate events.
 * A thread can simultaneousely wait for multiple flags to be set,
 * so it is possible to post multiple events to this thread.
 * The count of events a flag object can handle is only limited by the
 * underlaying architecutre, the maximum count is defined as ::MVAR_BITS - 1.
 * @{
 */
/**
 * Flag function.
 * Allocates a flag object. A flag object behaves like an array of
 * one bit semaphores. The object can hold up to ::MVAR_BITS - 1 flags.
 * The flags can be used to simulate events, so a single thread can wait
 * for several events simultaneously.
 * @return  handle to the new flag object. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      posFlagGet, posFlagSet, posFlagDestroy
 */
POSFLAG_t   posFlagCreate(void);

#if (DOX!=0) || (POSCFG_FEATURE_FLAGDESTROY != 0)
/**
 * Flag function.
 * Frees an unused flag object again.
 * @param   flg  handle to the flag object.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.<br>
 *          ::POSCFG_FEATURE_FLAGDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      posFlagCreate
 */
void        posFlagDestroy(POSFLAG_t flg);
#endif

/**
 * Flag function.
 * Sets a flag bit in the flag object and sets the task that
 * pends on the flag object to running state.
 * @param   flg     handle to the flag object.
 * @param   flgnum  Number of the flag to set. The flag number
 *                  must be in the range of 0 .. ::MVAR_BITS - 2.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      posFlagCreate, posFlagGet, posFlagWait
 */
VAR_t       posFlagSet(POSFLAG_t flg, UVAR_t flgnum);

/**
 * Flag function.
 * Pends on a flag object and waits until one of the flags 
 * in the flag object is set.
 * @param   flg   handle to the flag object.
 * @param   mode  can be POSFLAG_MODE_GETSINGLE or POSFLAG_MODE_GETMASK.
 * @return  the number of the next flag that is set when mode is set
 *          to POSFLAG_MODE_GETSINGLE. When mode is set to 
 *          POSFLAG_MODE_GETMASK, a bit mask with all set flags is
 *          returned. A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      posFlagCreate, posFlagSet, posFlagWait
 */
VAR_t       posFlagGet(POSFLAG_t flg, UVAR_t mode);

#if (DOX!=0) || (POSCFG_FEATURE_FLAGWAIT != 0)
/**
 * Flag function.
 * Pends on a flag object and waits until one of the flags 
 * in the flag object is set or a timeout has happened.
 * @param   flg   handle to the flag object.
 * @param   timeoutticks  timeout in timer ticks
 *          (see ::HZ define and ::MS macro).
 *          If this parameter is set to zero, the function immediately
 *          returns. If this parameter is set to INFINITE, the
 *          function will never time out.
 * @return  a mask of all set flags (positive value).
 *          If zero is returned, the timeout was reached.
 *          A negative value denotes an error.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.<br>
 *          ::POSCFG_FEATURE_FLAGWAIT must be defined to 1
 *          to have this function compiled in.
 * @sa      posFlagCreate, posFlagSet, posFlagGet, HZ, MS
 */
VAR_t       posFlagWait(POSFLAG_t flg, UINT_t timeoutticks);
#endif

#define POSFLAG_MODE_GETSINGLE   0
#define POSFLAG_MODE_GETMASK     1

#endif  /* POSCFG_FEATURE_FLAGS */
/** @} */

/*-------------------------------------------------------------------------*/

/** @defgroup timer User API: Timer Functions
 * A timer object is a counting variable that is counted down by the
 * system timer interrupt tick rate. If the variable reaches zero,
 * a semaphore, that is bound to the timer, will be signaled.
 * If the timer is in auto reload mode, the timer is restarted and
 * will signal the semaphore again and again, depending on the
 * period rate the timer is set to.
 * @{
 */
/** @def HZ
 * Hertz, timer ticks per second.
 * HZ is a define that is set to the number of ticks
 * the timer interrupt does in a second.
 * For example, the jiffies variable is incremented
 * HZ times per second. Also, much OS functions take
 * timeout parameters measured in timer ticks; thus
 * the HZ define can be taken as time base: HZ = 1 second,
 * 10*HZ = 10s, HZ/10 = 100ms, etc.
 * @sa jiffies, MS
 */
#if DOX!=0
#define HZ (timerticks per second)
#endif

/** @var POSEXTERN JIF_t jiffies;
 * Global timer variable.
 * The jiffies counter variable is incremented ::HZ times per second.
 * The maximum count the jiffie counter can reach until it wraps around
 * is system dependent.
 * @note    ::POSCFG_FEATURE_JIFFIES must be defined to 1 
 *          to have jiffies support compiled in.
 * @sa HZ
 */
#if (DOX!=0) || (POSCFG_FEATURE_JIFFIES != 0)
#if (DOX!=0) || (POSCFG_FEATURE_LARGEJIFFIES == 0)
POSEXTERN  JIF_t  jiffies;
#else
JIF_t       posGetJiffies(void);
#define jiffies  posGetJiffies()
#endif

/**
 * This macro is used to test if a specified time has expired.
 * It handles timer variable wrap arounds correctly.
 * The macro is used in conjunction with the jiffies variable,
 * the current jiffies should be passed as first parameter
 * to the macro. Example:<br>
 * exptime = jiffies + HZ/2;<br>
 * if (POS_TIMEAFTER(jiffies, exptime)) printf("500ms expired!\n");
 * @sa jiffies, HZ
 */
#define POS_TIMEAFTER(x, y)    ((((SJIF_t)(x)) - ((SJIF_t)(y))) >= 0)

#endif  /* POSCFG_FEATURE_JIFFIES */

#if (DOX!=0) || (POSCFG_FEATURE_TIMER != 0)

/**
 * Timer function.
 * Allocates a timer object. After a timer is allocated with this function,
 * it must be set up with posTimerSet and than started with posTimerStart.
 * @return  handle to the new timer object. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      posTimerSet, posTimerStart, posTimerDestroy
 */
POSTIMER_t  posTimerCreate(void);

/**
 * Timer function.
 * Sets up a timer object.
 * @param   tmr  handle to the timer object.
 * @param   sema seaphore object that shall be signaled when timer fires.
 * @param   waitticks  number of initial wait ticks. The timer fires the
 *                     first time when this ticks has been expired.
 * @param   periodticks  After the timer has fired, it is reloaded with
 *                       this value, and will fire again when this count
 *                       of ticks has been expired (auto reload mode).
 *                       If this value is set to zero, the timer
 *                       won't be restarted (= one shot mode).
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      posTimerCreate, posTimerStart
 */
VAR_t       posTimerSet(POSTIMER_t tmr, POSSEMA_t sema,
                        UINT_t waitticks, UINT_t periodticks);
/**
 * Timer function.
 * Starts a timer. The timer will fire first time when the
 * waitticks counter has been reached zero. If the periodticks
 * were set, the timer will be reloaded with this value.
 * @param   tmr  handle to the timer object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      posTimerStop, posTimerFired
 */
VAR_t       posTimerStart(POSTIMER_t tmr);

/**
 * Timer function.
 * Stops a timer. The timer will no more fire. The timer
 * can be reenabled with posTimerStart.
 * @param   tmr  handle to the timer object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      posTimerStart, posTimerDestroy
 */
VAR_t       posTimerStop(POSTIMER_t tmr);

#if (DOX!=0) || (POSCFG_FEATURE_TIMERDESTROY != 0)
/**
 * Timer function.
 * Deletes a timer object and free its resources.
 * @param   tmr  handle to the timer object.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in. <br>
 *          ::POSCFG_FEATURE_TIMERDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      posTimerCreate
 */
void        posTimerDestroy(POSTIMER_t tmr);
#endif
#if (DOX!=0) || (POSCFG_FEATURE_TIMERFIRED != 0)
/**
 * Timer function.
 * The function is used to test if a timer has fired.
 * @param   tmr  handle to the timer object.
 * @return  1 when the timer has fired, otherwise 0.
 *          A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in. <br>
 *          ::POSCFG_FEATURE_TIMERFIRED must be defined to 1
 *          to have this function compiled in.
 * @sa      posTimerCreate, posTimerSet, posTimerStart
 */
VAR_t       posTimerFired(POSTIMER_t tmr);
#endif

#endif  /* POSCFG_FEATURE_TIMER */
/** @} */

/*-------------------------------------------------------------------------*/

#if (DOX!=0) || (POSCFG_FEATURE_SOFTINTS != 0)
/** @defgroup sint User API: Software Interrupt Functions
 * pico]OS has a built in mechanism to simulate software interrupts.
 * For example, software interrupts can be used to connect hardware
 * interrupts, that are outside the scope of pico]OS, to the realtime
 * operating system. A hardware interrupt will trigger a software
 * interrupt that can then signalize a semaphore object.<br>
 * <br>
 * Note that hardware interrupts, that do not call ::c_pos_intEnter and
 * ::c_pos_intExit, can't do calls to pico]OS functions, except to the
 * function ::posSoftInt.<br>
 * <br>
 * All software interrupts, that are triggered by a call to ::posSoftInt,
 * are chained into a global list. This list is then executed as soon
 * as possible, but at least when the pico]OS scheduler is called
 * (that is, for example, when a time slice has expired or a task
 * gives of processing time by itself).<br>
 * <br>
 * A software interrupt runs at interrupt level, that means with
 * interrupts disabled (pico]OS calls ::POS_SCHED_LOCK before executing
 * the software interrupt handler). The execution of software interrupt
 * handlers can not be inhibited by setting the ::posTaskSchedLock flag.
 * <br>
 * Note that software interrupts need additional space on the
 * processors call stack. Make sure to have space for at least
 * 5 additional subroutine calls, plus the calls you will make in the
 * ISR handler routine.
 * @{
 */
/**
 * Software Interrupt Function.
 * Rises a software interrupt. The software interrupt handler will
 * be executed as soon as possible. See above for a detailed description.
 * @param   intno number of the interrupt to rise. Must be in the
 *          range of 0 to ::POSCFG_SOFTINTERRUPTS - 1.
 * @param   param optional parameter that will be passed to the
 *          software interrupt handler funtion.
 * @note    ::POSCFG_FEATURE_SOFTINTS must be defined to 1 
 *          to have software interrupt support compiled in.
 * @note    If a software interrupt is triggered from inside the
 *          pico]OS scope, usually from application level, it is
 *          recommended to do a call to ::posTaskYield after this
 *          function is called. This will immediately start the
 *          interrupt handler.
 * @sa      posSoftIntSetHandler, posSoftIntDelHandler, POSCFG_SOFTINTQUEUELEN
 */
void        posSoftInt(UVAR_t intno, UVAR_t param);

/**
 * Software Interrupt Function.
 * Sets a software interrupt handler function. Befor a software interrupt
 * can be rised by a call to ::posSoftInt, this function must be called
 * to tell pico]OS the handler function for the interrupt.
 * @param   intno number of the interrupt to rise. Must be in the
 *          range of 0 to ::POSCFG_SOFTINTERRUPTS - 1.
 * @param   inthandler pointer to the interrupt handler function.
 * @note    ::POSCFG_FEATURE_SOFTINTS must be defined to 1 
 *          to have software interrupt support compiled in.
 * @return  zero on success.
 * @sa      posSoftIntDelHandler, posSoftInt
 */
VAR_t       posSoftIntSetHandler(UVAR_t intno, POSINTFUNC_t inthandler);

#if (DOX!=0) || (POSCFG_FEATURE_SOFTINTDEL != 0)
/**
 * Software Interrupt Function.
 * Deletes a software interrupt handler function, that was set
 * with ::posSoftIntSetHandler before.
 * @param   intno number of the interrupt to rise. Must be in the
 *          range of 0 to ::POSCFG_SOFTINTERRUPTS - 1.
 * @note    ::POSCFG_FEATURE_SOFTINTS must be defined to 1 
 *          to have software interrupt support compiled in.<br>
 *          ::POSCFG_FEATURE_SOFTINTDEL must be defined to 1
 *          to have this function compiled in.
 * @return  zero on success.
 * @sa      posSoftIntDelHandler, posSoftInt
 */
VAR_t       posSoftIntDelHandler(UVAR_t intno);
#endif

#endif  /* POSCFG_FEATURE_SOFTINTS */
/** @} */


/* ==== END OF USER API ==== */



/*---------------------------------------------------------------------------
 *  INTERNAL DEFINITIONS
 *-------------------------------------------------------------------------*/

#ifdef PICOS_PRIVINCL

#if MVAR_BITS == 8
#ifndef _POSCORE_C
POSEXTERN UVAR_t posShift1lTab_g[8];
#endif
#define pos_shift1l(bits)   posShift1lTab_g[(UVAR_t)(bits)]
#else
#define pos_shift1l(bits)   (((UVAR_t)1)<<(bits))
#endif

#endif /* PICOS_PRIV_H */

/*-------------------------------------------------------------------------*/

/* include the header of the nano layer */

#ifdef POSNANO
#ifdef POSCFG_ENABLE_NANO
#undef POSCFG_ENABLE_NANO
#endif
#define POSCFG_ENABLE_NANO  1
#else
#ifndef POSCFG_ENABLE_NANO
#define POSCFG_ENABLE_NANO  0
#endif
#endif
#if POSCFG_ENABLE_NANO != 0
#include <pos_nano.h>
#endif

/*-------------------------------------------------------------------------*/

/* defaults for hooks */

#ifndef POSCFG_STKFREE_HOOK
#define POSCFG_STKFREE_HOOK  0
#endif

/*-------------------------------------------------------------------------*/

/* fully define the task structure */

#ifndef NOS_TASKDATA
#define NOS_TASKDATA
#endif

#if POSCFG_STKFREE_HOOK != 0
typedef void (*POSSTKFREEFUNC_t)(POSTASK_t task);
#endif

struct POSTASK_s {
    POS_USERTASKDATA
    NOS_TASKDATA
#if POSCFG_STKFREE_HOOK != 0
    POSSTKFREEFUNC_t  stkfree;
#endif
#if DOX==0
#if POSCFG_ARGCHECK > 1
    UVAR_t         magic;
#endif
#if SYS_TASKDOUBLELINK != 0
    struct POSTASK_s *prev;
#endif
    struct POSTASK_s *next;
    UVAR_t         bit_x;
#if SYS_TASKTABSIZE_Y > 1
    UVAR_t         bit_y;
    UVAR_t         idx_y;
#endif
    UINT_t         ticks;
#if SYS_TASKSTATE != 0
    UVAR_t         state;
#endif
#if POSCFG_FEATURE_MSGBOXES != 0
    UVAR_t         msgwait;
    POSSEMA_t      msgsem;
    void           *firstmsg;
    void           *lastmsg;
#endif
#endif /* !DOX */
};


#endif /* _PICOOS_H */

