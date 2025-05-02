/* Copyright (C) 2004  David Decotigny
   Copyright (C) 1999  Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA.
*/
#ifndef _HWINTR_H_
#define _HWINTR_H_

/**
 * @file irq.c
 *
 * Hardware interrupts routines management.
 */

#include <sextant/types.h>
#include <sextant/interruptions/idt.h>
#include <drivers/timer.h>
#include <sextant/interruptions/i8259.h>

#define save_flags(flags) \
  asm volatile("pushfl ; popl %0":"=g"(flags)::"memory")
#define restore_flags(flags) \
  asm volatile("push %0; popfl"::"g"(flags):"memory")

#define disable_IRQs(flags)    \
  ({ save_flags(flags); asm("cli\n"); })
#define restore_IRQs(flags)    \
  restore_flags(flags)

/* Usual IRQ levels */
#define IRQ_TIMER         0
#define IRQ_KEYBOARD      1
#define IRQ_SLAVE_PIC     2
#define IRQ_COM2          3
#define IRQ_COM1          4
#define IRQ_LPT2          5
#define IRQ_FLOPPY        6
#define IRQ_LPT1          7
#define IRQ_8_NOT_DEFINED 8
#define IRQ_RESERVED_1    9
#define IRQ_RESERVED_2    10
#define IRQ_RESERVED_3    11
#define IRQ_RESERVED_4    12
#define IRQ_COPROCESSOR   13
#define IRQ_HARDDISK      14
#define IRQ_RESERVED_5    15

typedef void (*irq_handler_t)(int irq_level);

/** Setup the PIC */
sextant_ret_t irq_setup(void);

/**
 * If the routine is not NULL, the IDT is setup to call an IRQ
 * wrapper upon interrupt, which in turn will call the routine, and
 * the PIC is programmed to raise an irq.\ If the routine is
 * NULL, we disable the irq line.
 */
sextant_ret_t irq_set_routine(int irq_level,
			      irq_handler_t routine);

irq_handler_t irq_get_routine(int irq_level);

#endif
