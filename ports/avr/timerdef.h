/*
 *  Copyright (c) 2004, Swen Moczarski.
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
 * CVS-ID $Id: arch_c.c,v 1.4 2004/05/15 19:09:58 smocz Exp $
 */

#ifndef TIMERDEF_H
#define TIMERDEF_H 1

#if defined (__AVR_ATmega32__) || defined (__AVR_ATmega323__)

/**
 * The flags for the prescaler in the TCCR1B register.
 * 
 *  CS12  CS11  CS10            prescaler value
 *    0     0     1    (0x01)   CRYSTAL_CLOCK / 1
 *    0     1     0    (0x02)   CRYSTAL_CLOCK / 8
 *    0     1     1    (0x03)   CRYSTAL_CLOCK / 64
 *    1     0     0    (0x04)   CRYSTAL_CLOCK / 256
 *    1     0     1    (0x05)   CRYSTAL_CLOCK / 1024
 * 
 * This value defines the possible range for the timer tick (HZ).
 */
// set WGM12 for Clear Timer on Compare match (CTC) mode
#  define TIMER_CONFIG_VALUE           _BV(WGM12) | 0x04
#  define TIMER_CONFIG_REG             TCCR1B

/**
 * The resulting value for the presacler.
 */
#  define TIMER_PRESCALER_VALUE        256

#  define TIMER_COUNTER_VALUE          ((CRYSTAL_CLOCK / TIMER_PRESCALER_VALUE) / HZ)
#  define TIMER_COUNTER_REG            OCR1A

#  define TIMER_INTERRUPT_REG          TIMSK
#  define TIMER_INTERRUPT_ENABLE_BIT   OCIE1A

#else

#  error no timer configuration for this MCU type

#endif


#endif
