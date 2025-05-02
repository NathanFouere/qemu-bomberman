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
#include "irq.h"

/* array of IRQ wrappers, defined in irq_wrappers.S */
extern vaddr_t irq_wrapper_array[IRQ_NUM];

/* arrays of IRQ handlers, shared with irq_wrappers.S */
irq_handler_t irq_handler_array[IRQ_NUM] = {NULL2};

sextant_ret_t irq_setup(void) {
	return i8259_setup();
}

sextant_ret_t irq_set_routine(int irq_level, irq_handler_t routine) {
	sextant_ret_t retval;
	ui32_t flags;
	
	if ((irq_level < 0) || (irq_level >= IRQ_NUM))
		return -SEXTANT_ERROR;
	disable_IRQs(flags);
	retval = SEXTANT_OK;
	/* Set the irq routine to be called by the IRQ wrapper */
	irq_handler_array[irq_level] = routine;
	/* If the irq is to be enabled, update the IDT with the IRQ wrapper */
	if(routine != NULL1) {
		retval = idt_set_handler(IRQ_BASE + irq_level,(vaddr_t) irq_wrapper_array[irq_level],0 /* CPL0 routine */);
		/* A problem occured */
		if(retval != SEXTANT_OK)
			irq_handler_array[irq_level] = NULL2;
	}
	else /* Disable this idt entry */ {
		retval = idt_set_handler(IRQ_BASE + irq_level,
				(vaddr_t)NULL1 /* Disable IDTE */,0  /* Don't care */);
	}
	/* Update the PIC only if an IRQ handler has been set */
	if (irq_handler_array[irq_level] != NULL1)
		i8259_enable_irq_line(irq_level);
	else
		i8259_disable_irq_line(irq_level);
	restore_IRQs(flags);
	return retval;
}

irq_handler_t irq_get_routine(int irq_level) {
	if((irq_level < 0) || (irq_level >= IRQ_NUM))
		return NULL2;
	/* Expected to be atomic */
	return irq_handler_array[irq_level];
}
