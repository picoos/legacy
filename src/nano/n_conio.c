/*
 *  Copyright (c) 2004, Dennis Kuschel.
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


/**
 * @file   n_conio.c
 * @brief  nano layer, console I/O
 * @author Dennis Kuschel
 *
 * This file is originally from the pico]OS realtime operating system
 * (http://picoos.sourceforge.net).
 *
 * CVS-ID $Id:$
 */

#include "../src/nano/privnano.h"



/*---------------------------------------------------------------------------
 * CONFIGURATION
 *-------------------------------------------------------------------------*/

#define FEAT_XPRINTF    (NOSCFG_FEATURE_PRINTF + NOSCFG_FEATURE_SPRINTF)
#define FEAT_PRINTOUT   (NOSCFG_FEATURE_CONOUT + FEAT_XPRINTF)



/*---------------------------------------------------------------------------
 * IMPORTS
 *-------------------------------------------------------------------------*/

#if NOSCFG_FEATURE_CONOUT != 0
extern void p_putchar(char c);
#endif



/*---------------------------------------------------------------------------
 * PROTOTYPES OF PRIVATE FUNCTIONS
 *-------------------------------------------------------------------------*/

#if FEAT_XPRINTF != 0
static void n_printf(const char *fmt, NOSARG_t *args);
#endif
#if NOSCFG_FEATURE_SPRINTF != 0
static void n_updstr(char c);
#endif



/*---------------------------------------------------------------------------
 * GLOBAL VARIABLES
 *-------------------------------------------------------------------------*/

#if FEAT_PRINTOUT != 0
static POSSEMA_t    print_sema_g;
#endif

#if FEAT_XPRINTF != 0
static char         nbrbuf_g[(sizeof(INT_t)*5+1)/2];
#endif

#if NOSCFG_FEATURE_CONIN != 0
static UVAR_t       cin_inptr_g;
static UVAR_t       cin_outptr_g;
static UVAR_t       cin_waiting_g;
static char         cin_buf[NOSCFG_CONIO_KEYBUFSIZE + 1];
static POSSEMA_t    cin_sema_g;
#endif



/*---------------------------------------------------------------------------
 * MACROS
 *-------------------------------------------------------------------------*/

#if (NOSCFG_FEATURE_PRINTF != 0) && (NOSCFG_FEATURE_SPRINTF != 0)
typedef void (*NPRINTFUNC_t)(char c);
static NPRINTFUNC_t       prf_g;
#define SET_PRFUNC(func)  prf_g = &(func)
#define CALL_PRFUNC(c)    (prf_g)(c)
#else
#define SET_PRFUNC(func)  do { } while(0)
#if NOSCFG_FEATURE_PRINTF != 0
#define CALL_PRFUNC(c)    p_putchar(c)
#else
#define CALL_PRFUNC(c)    n_updstr(c)
#endif
#endif



/*---------------------------------------------------------------------------
 * CONSOLE OUTPUT FUNCTIONS
 *-------------------------------------------------------------------------*/

#if NOSCFG_FEATURE_CONOUT != 0

void nosPrintChar(char c)
{
  posSemaGet(print_sema_g);
  p_putchar(c);
  posSemaSignal(print_sema_g);
}

/*-------------------------------------------------------------------------*/

void nosPrint(const char *str)
{
#if NOSCFG_FEATURE_PRINTF != 0
  posSemaGet(print_sema_g);
  SET_PRFUNC(p_putchar);
  n_printf(str, NULL);
  posSemaSignal(print_sema_g);
#else
  char c;
  posSemaGet(print_sema_g);
  c = *str;
  while (c != 0)
  {
    if (c == '\n')
      p_putchar('\r');
    p_putchar(c);
    c = *++str;
  }
  posSemaSignal(print_sema_g);
#endif
}

#endif /* NOSCFG_FEATURE_CONOUT */

/*-------------------------------------------------------------------------*/

#if FEAT_XPRINTF != 0

void n_printf(const char *fmt, NOSARG_t *args)
{
  char   b, c, *s;
  UVAR_t base;
  UINT_t nbr;
  unsigned char a = 0;
  char   fill = 0;
  char   width = 0;
  UVAR_t i = 0;
  char   *f = (char*) fmt;

  while (*f != 0)
  {
    c = *f++;

    /* print usual characters */
    if (c != '%')
    {
      if (c == '\n')
        CALL_PRFUNC('\r');
      CALL_PRFUNC(c);
      continue;
    }

    /* test if next character is a '%' */
    c = *f++;
    if (c == '%')
    {
      CALL_PRFUNC('%');
      continue;
    }

    /* save index to value in parameter list */
    nbr = (UINT_t) args[a];
    s   = (char*)  args[a];
    a++;

    /* get width. width can be '1'-'9', '01'-'09', or '*' */
    width = 0;
    fill  = c;
    base = 10;
    if ((c == ' ') || (c == '0'))
    {
      c = *f++;
    }
    else
    if (c == '.')
    {
      fill = '0';
      c = *f++;
    }
    if ((c >= '1') && (c <= '9'))
    {
      width = c - '0';
      c = *f++;
    }
    else
    if (c == '*')
    {
      width = (char) args[a++];
      c = *f++;
    }

    /* skip size modifiers (we only support INT_t) */
    if ((c == 'h') || (c == 'l'))
    {
      c = *f++;
    }

    /* Get format specifier.
       All not checked specifiers are ignored. */
    if ((c == 'd') || (c == 'i'))
    {
      if ((INT_t)nbr < 0)
      {
        nbr = 0 - nbr;
        CALL_PRFUNC('-');
      }
    }
    else
    if (c == 'u')
    {
    }
    else
    if (c == 'o')
    {
      base = 8;
    }
    else
    if ((c == 'x') || (c == 'X'))
    {
      base = 16;
    }
    else
    if (c == 'c')
    {
      CALL_PRFUNC((char)nbr);
      continue;
    }
    else
    if (c == 's')
    {
      while (*s != 0)
      {
        if (*s == '\n')
          CALL_PRFUNC('\r');
        CALL_PRFUNC(*s++);
      }
      continue;
    }
    else
    {
      continue;
    }

    /* make string from binary number */
    do
    {
      b = (char) (nbr % (UINT_t) base);
      if (b > 9)
      {
        if (c == 'X')
        {
          b += 'A' - 10;
        }
        else
        {
          b += 'a' - 10;
        }
      }
      else
      {
        b += '0';
      }
      nbr /= (UINT_t) base;
      nbrbuf_g[i++] = b;
    }
    while (nbr != 0);

    /* print leading zeros or spaces */
    while (width > i)
    {
      CALL_PRFUNC(fill);
      width--;
    }

    /* print number */
    do
    {
      CALL_PRFUNC(nbrbuf_g[--i]);
    }
    while (i != 0);
  }
}

#endif /* FEAT_XPRINTF */

/*-------------------------------------------------------------------------*/

#if NOSCFG_FEATURE_PRINTF != 0

void n_printFormattedN(const char *fmt, NOSARG_t args)
{
  posSemaGet(print_sema_g);
  SET_PRFUNC(p_putchar);
  n_printf(fmt, args);
  posSemaSignal(print_sema_g);
}

#endif /* NOSCFG_FEATURE_PRINTF */

/*-------------------------------------------------------------------------*/

#if NOSCFG_FEATURE_SPRINTF != 0

static char *sprptr_g;

static void n_updstr(char c)
{
  *sprptr_g++ = c;
}

void n_sprintFormattedN(char *buf, const char *fmt, NOSARG_t args)
{
  posSemaGet(print_sema_g);
  SET_PRFUNC(n_updstr);
  sprptr_g = buf;
  n_printf(fmt, args);
  *sprptr_g = 0;
  posSemaSignal(print_sema_g);
}

#endif /* NOSCFG_FEATURE_SPRINTF */



/*---------------------------------------------------------------------------
 * CONSOLE INPUT FUNCTIONS
 *-------------------------------------------------------------------------*/

#if NOSCFG_FEATURE_CONIN != 0

void c_nos_keyinput(UVAR_t key)
{
  UVAR_t n;
  POS_LOCKFLAGS;

  POS_SCHED_LOCK;
  n = cin_inptr_g + 1;
  if (n > NOSCFG_CONIO_KEYBUFSIZE)
    n = 0;
  if (n != cin_outptr_g)
  {
    cin_buf[cin_inptr_g] = key;
    cin_inptr_g = n;
  }
  POS_SCHED_UNLOCK;

  if (cin_waiting_g != 0)
    posSemaSignal(cin_sema_g);
}

/*-------------------------------------------------------------------------*/

char nosKeyGet(void)
{
  char c;
  POS_LOCKFLAGS;

  POS_SCHED_LOCK;
  for(;;)
  {
    if (cin_inptr_g != cin_outptr_g)
    {
      c = cin_buf[cin_outptr_g];
      if (++cin_outptr_g > NOSCFG_CONIO_KEYBUFSIZE)
        cin_outptr_g = 0;
      break;
    }
    ++cin_waiting_g;
    POS_SCHED_UNLOCK;
    posSemaGet(cin_sema_g);
    POS_SCHED_LOCK;
    --cin_waiting_g;
  }
  POS_SCHED_UNLOCK;
  return c;
}

/*-------------------------------------------------------------------------*/

UVAR_t nosKeyPressed(void)
{
  return (cin_inptr_g != cin_outptr_g) ? 1 : 0;
}

#endif /* NOSCFG_FEATURE_CONIN */



/*---------------------------------------------------------------------------
 * INITIALIZATION
 *-------------------------------------------------------------------------*/

#if (NOSCFG_FEATURE_CONIN != 0) || (FEAT_PRINTOUT != 0)

void nos_initConIO(void)
{
#if NOSCFG_FEATURE_CONIN != 0
  cin_inptr_g   = 0;
  cin_outptr_g  = 0;
  cin_waiting_g = 0;
  cin_sema_g    = posSemaCreate(0);
#if (POSCFG_FEATURE_SOFTINTS != 0)
  posSoftIntSetHandler(0, c_nos_keyinput);
#endif
#endif

#if FEAT_PRINTOUT != 0
  print_sema_g = posSemaCreate(1);
#endif
}

#else

/* this is just a dummy function */
void nos_initConIO(void) {}

#endif
