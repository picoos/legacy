/*
 *  Copyright (c) 2004-2005, Dennis Kuschel.
 *  All rights reserved. 
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. The name of the author may not be used to endorse or promote
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
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*
 * This file is originally from the pico]OS realtime operating system
 * (http://picoos.sourceforge.net).
 *
 * CVS-ID $Id: arch_c.c,v 1.3 2005/01/17 21:59:46 dkuschel Exp $
 */


/*  Note on compiler:
 *
 *  This port was successfully tested with MS Visual C++ 6.0 and MinGW
 */


#define NANOINTERNAL
#include <picoos.h>

#undef INFINITE
#undef E_FAIL
#undef errno

#ifdef memset
#undef memset
#endif
#ifdef memcpy
#undef memcpy
#endif

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#ifndef GCC
#define HAVEMMLIB
#endif
#ifdef HAVEMMLIB
#include <mmsystem.h>
#endif

#if HZ > 100
#error  HZ is not allowed to be greater than 100 !
#endif



/*---------------------------------------------------------------------------
 *  PARAMETERS
 *-------------------------------------------------------------------------*/

#define MIN_STACKSIZE   (64 * 1024)



/*---------------------------------------------------------------------------
 *  TYPEDEFS
 *-------------------------------------------------------------------------*/

typedef enum {
  task_mustcreate,
  task_exist,
  task_interrupted,
  task_mustquit,
  task_terminated
} TSTATE_t;

typedef struct TASKPRIV_s {
            HANDLE        suspendSema;
            HANDLE        ownTaskHandle;
            DWORD         ownTaskID;
  volatile  TSTATE_t      state;
            POSTASKFUNC_t firstfunc;
            void*         taskarg;
            UINT_t        stacksize;
            int           priority;
  volatile  int           blockIntFlag;
} *TASKPRIV_t;



/*---------------------------------------------------------------------------
 *  GLOBAL VARIABLES
 *-------------------------------------------------------------------------*/

static            HANDLE    timerEvent_g;
static            HANDLE    globalSyncSem_g;
static            HANDLE    interruptWaitSem_g;
static  volatile  DWORD     interruptTaskId_g;
static  volatile  int       taskLockCnt_g;
static  volatile  int       blockInterrupt_g;
static  volatile  int       interruptWaiting_g;
static  volatile  int       interruptActive_g;
static  volatile  int       interruptExecuting_g;
static  volatile  int       idleTaskCreated_g;
static  volatile  int       archInitialized_g = 0;



/*---------------------------------------------------------------------------
 * FUNCTION PROTOTYPES
 *-------------------------------------------------------------------------*/

/* exported functions */
void p_pos_initArch(void);
void p_pos_softContextSwitch(void);
void p_pos_intContextSwitch(void);
void p_pos_startFirstContext(void);
void p_pos_globalLock(int *flags);
void p_pos_globalUnlock(int flags);

/* local functions */
static DWORD WINAPI a_newThread(LPVOID param);
static void a_quitThisTask(TASKPRIV_t thistask);
static void a_createThisTask(TASKPRIV_t thistask);
static void a_timerTask(void);
static void a_initTimer(void);
static void a_initTask(POSTASK_t task, UINT_t stacksize,
                       POSTASKFUNC_t funcptr, void *funcarg);
static void do_assert(const char* file, int line);
#if POSCFG_ENABLE_NANO
static void a_keyboardInput(void);
#endif
#ifdef HAVEMMLIB
static void CALLBACK a_timerCallback(UINT uTimerID, UINT uMsg,
                                     DWORD dwUser, DWORD dw1, DWORD dw2);
#endif



/*---------------------------------------------------------------------------
 *  MACROS
 *-------------------------------------------------------------------------*/

#define GETTASKPRIV(taskhandle)  ((TASKPRIV_t)((taskhandle)->portmem))
#define assert(x)     if (!(x)) do_assert(__FILE__, __LINE__)
#define SemaWait(s)   assert(WaitForSingleObject(s,INFINITE)==WAIT_OBJECT_0)
#define SemaSignal(s) ReleaseSemaphore(s, 1, NULL)



/*---------------------------------------------------------------------------
 *  SOME HELPER FUNCTIONS
 *-------------------------------------------------------------------------*/


static void do_assert(const char* file, int line)
{
  TASKPRIV_t  thistask = GETTASKPRIV(posCurrentTask_g);
  TASKPRIV_t  nexttask = GETTASKPRIV(posNextTask_g);

  fprintf(stderr, "\n\nASSERTION FAILED:  %s, line %i\n\n", file, line);

  fprintf(stderr, "taskLockCnt_g        = %i\n", taskLockCnt_g);
  fprintf(stderr, "blockInterrupt_g     = %i\n", blockInterrupt_g);
  fprintf(stderr, "interruptWaiting_g   = %i\n", interruptWaiting_g);
  fprintf(stderr, "interruptActive_g    = %i\n", interruptActive_g);
  fprintf(stderr, "interruptExecuting_g = %i\n\n", interruptExecuting_g);

#ifdef POS_DEBUGHELP
  fprintf(stderr, "thistask: name       = %s\n", posCurrentTask_g->deb.name);
  fprintf(stderr, "thistask: deb_state  = %i\n", posCurrentTask_g->deb.state);
#else
  fprintf(stderr, "thistask: handle     = %08x\n", (unsigned int)posCurrentTask_g);
#endif
  fprintf(stderr, "thistask: pico_state = %i\n", posCurrentTask_g->state);
  fprintf(stderr, "thistask: arch_state = %i\n", thistask->state);
  fprintf(stderr, "thistask: blockIntFl = %i\n", thistask->blockIntFlag);

  if (thistask != nexttask)
  {
#ifdef POS_DEBUGHELP
    fprintf(stderr, "\nnexttask: name       = %s\n", posNextTask_g->deb.name);
    fprintf(stderr, "nexttask: deb_state  = %i\n", posNextTask_g->deb.state);
#else
    fprintf(stderr, "\nnexttask: handle     = %08x\n", (unsigned int)posNextTask_g);
#endif
    fprintf(stderr, "nexttask: pico_state = %i\n", posNextTask_g->state);
    fprintf(stderr, "nexttask: arch_state = %i\n", nexttask->state);
    fprintf(stderr, "nexttask: blockIntFl = %i\n", nexttask->blockIntFlag);
  }

  for(;;) { SemaWait(globalSyncSem_g); }
}


#ifdef HAVEMMLIB
static void CALLBACK a_timerCallback(UINT uTimerID, UINT uMsg,
                                     DWORD dwUser, DWORD dw1, DWORD dw2)
{
  SemaSignal(timerEvent_g);
}
#endif


static void a_initTimer(void)
{
#ifdef HAVEMMLIB
  TIMECAPS timecaps;
  BOOL tok;

  timerEvent_g = CreateSemaphore(NULL, 0, 2*HZ, NULL);
  tok = (timerEvent_g != NULL);

  if (tok)
    tok = timeGetDevCaps(&timecaps, sizeof(timecaps)) == TIMERR_NOERROR;

  if (tok)
    tok = timeBeginPeriod(timecaps.wPeriodMin) == TIMERR_NOERROR;

  if (tok)
  {

    if (timeSetEvent(1000 / HZ, 1000 / HZ,
                     (LPTIMECALLBACK) a_timerCallback,
                     0, TIME_PERIODIC) == 0)
    {
      timeEndPeriod(timecaps.wPeriodMin);
      tok = FALSE;
    }
  }

  if (!tok && (timerEvent_g != NULL))
  {
    CloseHandle(timerEvent_g);
    timerEvent_g = NULL;
  }
#endif
}


static DWORD WINAPI a_newThread(LPVOID param)
{
  TASKPRIV_t thistask = (TASKPRIV_t) param;
  
  thistask->ownTaskID = GetCurrentThreadId();

  SetThreadPriority(GetCurrentThread(), thistask->priority);

  (thistask->firstfunc)(thistask->taskarg);

#if (POSCFG_FEATURE_EXIT != 0)
  posTaskExit();
#endif

  assert(0);
  return 0;
}


static void a_createThisTask(TASKPRIV_t thistask)
{
  thistask->suspendSema = CreateSemaphore(NULL, 0, 1, NULL);
  assert(thistask->suspendSema != NULL);

  thistask->state = task_exist;

  thistask->ownTaskHandle = CreateThread(
                              NULL, thistask->stacksize, a_newThread,
                              thistask, 0, &thistask->ownTaskID);

  assert(thistask->ownTaskHandle != NULL);
}


static void a_quitThisTask(TASKPRIV_t thistask)
{
  if (thistask->suspendSema != NULL)
  {
    CloseHandle(thistask->suspendSema);
    thistask->suspendSema = NULL;
  }
  thistask->state = task_terminated;
  ExitThread(0);
}


static void a_initTask(POSTASK_t task, UINT_t stacksize,
                       POSTASKFUNC_t funcptr, void *funcarg)
{
  TASKPRIV_t newtask = GETTASKPRIV(task);

  newtask->state        = task_mustcreate;
  newtask->ownTaskID    = 0;
  newtask->ownTaskHandle= NULL;
  newtask->suspendSema = NULL;

  newtask->stacksize    = (stacksize>MIN_STACKSIZE)? stacksize:MIN_STACKSIZE;
  newtask->firstfunc    = funcptr;
  newtask->taskarg      = funcarg;
  
  newtask->priority     = THREAD_PRIORITY_BELOW_NORMAL;
  if (!idleTaskCreated_g)
  {
    newtask->priority   = THREAD_PRIORITY_IDLE;
    idleTaskCreated_g = 1;
  }

  newtask->blockIntFlag = 0;
}



/*---------------------------------------------------------------------------
 *  TIMER INTERRUPT
 *-------------------------------------------------------------------------*/


static void a_timerTask(void)
{
  for(;;)
  {
    /* wait for the timer event */
    if (timerEvent_g != NULL)
    {
      Sleep(500/HZ);
      SemaWait(timerEvent_g);
    }
    else
    {
      Sleep(1000/HZ);
    }

    /* wait here until it is allowed to run the pico]OS functions */
    interruptActive_g = 1;
    while ((blockInterrupt_g != 0) || (taskLockCnt_g != 0))
    {
      interruptWaiting_g = 1;

      if (taskLockCnt_g == 0)
      {
        if (!interruptWaiting_g)
          SemaWait(interruptWaitSem_g);
        interruptWaiting_g = 0;
        break;
      }

      interruptActive_g = 0;
      SemaWait(interruptWaitSem_g);
      interruptActive_g = 1;
    }
    assert(taskLockCnt_g == 0);
    SemaWait(globalSyncSem_g);
    assert(taskLockCnt_g == 0);
    interruptExecuting_g = 1;

#if POSCFG_ENABLE_NANO
    /* do the keyboard interrupt */
    a_keyboardInput();
#endif

    /* do the timer interrupt */
    c_pos_intEnter();
    c_pos_timerInterrupt();
    c_pos_intExit();

    /* end the timer interrupt loop */
    interruptExecuting_g = 0;
    interruptActive_g = 0;
    SemaSignal(globalSyncSem_g);
  }
}




/*---------------------------------------------------------------------------
 *  EXPORTED FUNCTIONS
 *-------------------------------------------------------------------------*/



/*--------  PORT INITIALIZATION  --------*/


void p_pos_initArch(void)
{
  if (!archInitialized_g)
  {
    assert(TASK_RESERVED_PORT_MEM >= sizeof(struct TASKPRIV_s));

    interruptWaitSem_g = CreateSemaphore(NULL, 0, 1, NULL);
    assert(interruptWaitSem_g != NULL);

    globalSyncSem_g = CreateSemaphore(NULL, 1, 1, NULL);
    assert(globalSyncSem_g != NULL);

    timerEvent_g         = NULL;
    taskLockCnt_g        = 0;
    blockInterrupt_g     = 0;
    interruptTaskId_g    = 0;
    idleTaskCreated_g    = 0;
    interruptWaiting_g   = 0;
    interruptActive_g    = 0;
    interruptExecuting_g = 0;
    archInitialized_g    = 1;
  }
}



/*--------  TASK STRUCTURE SETUP  --------*/

#if (POSCFG_TASKSTACKTYPE == 0)

void p_pos_initTask(POSTASK_t task, void *user,
                    POSTASKFUNC_t funcptr, void *funcarg)
{
  (void) user;
  a_initTask(task, MIN_STACKSIZE, funcptr, funcarg);
}

#elif (POSCFG_TASKSTACKTYPE == 1)

VAR_t p_pos_initTask(POSTASK_t task, UINT_t stacksize,
                     POSTASKFUNC_t funcptr, void *funcarg)
{
  a_initTask(task, stacksize, funcptr, funcarg);
  return 0;
}
  
#elif (POSCFG_TASKSTACKTYPE == 2)

VAR_t p_pos_initTask(POSTASK_t task,
                     POSTASKFUNC_t funcptr, void *funcarg)
{
  a_initTask(task, MIN_STACKSIZE, funcptr, funcarg);
  return 0;
}

#else
#error POSCFG_TASKSTACKTYPE
#endif


void p_pos_freeStack(POSTASK_t task)
{
  TASKPRIV_t tp = GETTASKPRIV(task);
  tp->state = task_mustquit;
}



/*--------  CONTEXT SWITCHING  --------*/


void p_pos_softContextSwitch(void)
{
  TASKPRIV_t  thistask = GETTASKPRIV(posCurrentTask_g);
  TASKPRIV_t  nexttask = GETTASKPRIV(posNextTask_g);
  TSTATE_t    state = thistask->state;

  assert(interruptExecuting_g == 0);
  assert(GetCurrentThreadId() == thistask->ownTaskID);
  assert(thistask != nexttask);
  assert(taskLockCnt_g != 0);

  /* swap context variable */
  posCurrentTask_g = posNextTask_g;

  /* the idle task gives off processing time here */
  if (thistask->priority == THREAD_PRIORITY_IDLE)
  {
    Sleep(0);
  }

  /* start next task */
  if (nexttask->state == task_exist)
  {
    SemaSignal(nexttask->suspendSema);
  }
  else
  if (nexttask->state == task_interrupted)
  {
    Sleep(0);
    SemaWait(globalSyncSem_g);
    assert(taskLockCnt_g == 1);
    blockInterrupt_g = nexttask->blockIntFlag;
    nexttask->state = task_exist;
    p_pos_globalUnlock(-1); 
    assert( ResumeThread(nexttask->ownTaskHandle) == 1 );
    SemaSignal(globalSyncSem_g);
  }
  else
  if (nexttask->state == task_mustcreate)
  {
    SemaWait(globalSyncSem_g);
    assert(taskLockCnt_g == 1);
    p_pos_globalUnlock(-1);
    a_createThisTask(nexttask);
    SemaSignal(globalSyncSem_g);
  }
  else assert(0);

  /* suspend current task */
  if (state == task_exist)
  {
    SemaWait(thistask->suspendSema);
  }
  else
  if (state == task_mustquit)
  {
    a_quitThisTask(thistask);
  }
  else assert(0);

  assert(taskLockCnt_g != 0);
}


void p_pos_intContextSwitch(void)
{
  TASKPRIV_t  thistask = GETTASKPRIV(posCurrentTask_g);
  TASKPRIV_t  nexttask = GETTASKPRIV(posNextTask_g);
  int i;

  assert(thistask->state == task_exist);

  /* avoid race condition: wait until just started task is up */
  i = 0;
  while (thistask->ownTaskHandle == NULL)
  {
    Sleep(i);
    if (++i > 100) i = 100;
  }

  /* suspend the currently active task */
  assert( SuspendThread(thistask->ownTaskHandle) == 0 );
  thistask->state = task_interrupted;

  thistask->blockIntFlag = blockInterrupt_g;
  blockInterrupt_g = 0;

  /* start next task */
  posCurrentTask_g = posNextTask_g;
  if (nexttask->state == task_exist)
  {
    taskLockCnt_g = 1;
    interruptActive_g = 0;
    interruptExecuting_g = 0;
    SemaSignal(nexttask->suspendSema);
  }
  else
  if (nexttask->state == task_interrupted)
  {
    assert(taskLockCnt_g == 0);
    interruptActive_g = 0;
    interruptExecuting_g = 0;
    blockInterrupt_g = nexttask->blockIntFlag;
    nexttask->state = task_exist;
    assert( ResumeThread(nexttask->ownTaskHandle) == 1 );
  }
  else
  if (nexttask->state == task_mustcreate)
  {
    assert(taskLockCnt_g == 0);
    interruptActive_g = 0;
    interruptExecuting_g = 0;
    a_createThisTask(nexttask);
  }
  else assert(0);
}


void p_pos_startFirstContext(void)
{
  TASKPRIV_t firsttask;

  /* Set this thread to high priority. This thread will do the timer IRQ. */
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

  /* Start the first pico]OS task (=first context). */
  SemaWait(globalSyncSem_g);
  p_pos_globalUnlock(-1);
  firsttask = GETTASKPRIV(posCurrentTask_g);
  a_createThisTask(firsttask);
  SemaSignal(globalSyncSem_g);

  /* OK. We continue with doing the timer interrupt. */
  interruptTaskId_g = GetCurrentThreadId();
  a_initTimer();
  a_timerTask();
}



/*--------  GLOBAL INTERRUPT LOCKING  --------*/


void p_pos_globalLock(int *flags)
{
  TASKPRIV_t  thistask  = GETTASKPRIV(posCurrentTask_g);
  DWORD       curthread = GetCurrentThreadId();
  int i;

  if (!archInitialized_g) p_pos_initArch();

  if (interruptTaskId_g != curthread)
  {
    if (posRunning_g)
      assert(thistask->ownTaskID == curthread);

    i = 0;
    blockInterrupt_g = 1;
    while ((interruptActive_g || interruptExecuting_g))
    {
      blockInterrupt_g = 1;
      Sleep(i);
      if (++i > 100) i = 100;
    }

    assert(interruptExecuting_g == 0);

    *flags = taskLockCnt_g;
    taskLockCnt_g++;
    blockInterrupt_g = 0;
  }
  else
  {
    *flags = taskLockCnt_g;
  }
}


void p_pos_globalUnlock(int flags)
{
  TASKPRIV_t  thistask  = GETTASKPRIV(posCurrentTask_g);
  DWORD       curthread = GetCurrentThreadId();

  if (interruptTaskId_g != GetCurrentThreadId())
  {
    assert(taskLockCnt_g > 0);
    assert(taskLockCnt_g != flags);
    assert(interruptExecuting_g == 0);

    if (posRunning_g && (flags >= 0))
      assert(thistask->ownTaskID == curthread);

    if (taskLockCnt_g > 1)
    {
      assert(flags != 0);
      taskLockCnt_g--;
    }
    else
    {
      assert(flags <= 0);
      taskLockCnt_g = 0;

      if (interruptWaiting_g)
      {
        interruptWaiting_g = 0;
        SemaSignal(interruptWaitSem_g);
        Sleep(0);
      }
    }
  }
}




/*---------------------------------------------------------------------------
 *  NANO LAYER INTERFACE FUNCTIONS
 *-------------------------------------------------------------------------*/

#if POSCFG_ENABLE_NANO

UVAR_t p_putchar(char c)
{
  putchar(c);
  return 1;
}


static void a_keyboardInput(void)
{
#if NOSCFG_FEATURE_CONIN
  static int ctr = 0;
  UVAR_t c;

  if (++ctr >= (HZ/20))
  {
    ctr = 0;
    while (_kbhit())
    {
      c = (UVAR_t) _getch();
      if (c)
      {
        c_nos_keyinput(c);
      }
    }
  }
#endif
}

#endif /* POSCFG_ENABLE_NANO */
