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
#ifndef _i8259_H
#define _i8259_H

#include <sextant/types.h>
#include <hal/fonctionsES.h>

#define PIC_MASTER 0x20
#define PIC_SLAVE  0xa0

/**
 * @file i8259.h PIC
 *
 * PIC Management routines. See the Intel 8259A datasheet (on kos
 * website), page 9+. Should be not be used directly: only interrupt.c
 * should use this.
 *
 * @see i8259A datasheet on Kos website.
 */

/** Setup PIC and Disable all IRQ lines */

sextant_ret_t i8259_setup();

sextant_ret_t i8259_enable_irq_line(int numirq);

sextant_ret_t i8259_disable_irq_line(int numirq);

#endif /* _i8259_H */
