/* Copyright (C) 2004  The KOS Team
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
#define ASM_SOURCE 1

.file "irq_wrappers.S"

.text

/* The address of the table of handlers (defined in irq.c) */
.extern irq_handler_array

/* The address of the table of wrappers (defined below, and shared
   with irq.c */
.globl irq_wrapper_array


/* These pre-handlers are for IRQ (Master PIC) */
.irp id, 0,1,2,3,4,5,6,7
/* Aligne sur 32 bits et ajoute si necessaire des bits de bourage ici : 0x90 soit l'opcode P*/
	.p2align 2, 0x90

	irq_wrapper_\id:
	.type irq_wrapper_\id,@function

		/*
		 * Backup the CPU context
		 */

		/* Fake error code */
		pushl $0

		/* Backup the actual context */
		pushl %ebp
		movl %esp, %ebp

		pushl %edi
		pushl %esi
		pushl %edx
		pushl %ecx
		pushl %ebx
		pushl %eax
		subl  $2,%esp
		pushw %ss
		pushw %ds
		pushw %es
		pushw %fs
		pushw %gs

		/* Send EOI to PIC. See Intel 8259 datasheet
		   available on Kos website */
		movb  $0x20, %al
		outb  %al, $0x20

		/*
		 * Call the handler with IRQ number as argument
		 */
		pushl $\id
		leal  irq_handler_array,%edi
		call  *\id*4(%edi)
		addl  $4, %esp

		/* Restore the context */
		popw  %gs
		popw  %fs
		popw  %es
	        popw  %ds
		popw  %ss
		addl  $2,%esp
		popl  %eax
		popl  %ebx
		popl  %ecx
		popl  %edx
		popl  %esi
		popl  %edi
		popl  %ebp

		/* Remove fake error code */
		addl  $4, %esp

		iret
.endr


/* These pre-handlers are for IRQ (Slave PIC) */
.irp id, 8,9,10,11,12,13,14,15

	.p2align 2, 0x90

	irq_wrapper_\id:
	.type irq_wrapper_\id,@function

		/*
		 * Backup the CPU context
		 */

		/* Fake error code */
		pushl $0

		/* Backup the actual context */
		pushl %ebp
		movl %esp, %ebp

		pushl %edi
		pushl %esi
		pushl %edx
		pushl %ecx
		pushl %ebx
		pushl %eax
		subl  $2,%esp
		pushw %ss
		pushw %ds
		pushw %es
		pushw %fs
		pushw %gs

		/* Send EOI to PIC. See Intel 8259 datasheet
		   available on Kos website */
		movb  $0x20, %al
		outb  %al, $0xa0
		outb  %al, $0x20

		/*
		 * Call the handler with IRQ number as argument
		 */
		pushl $\id
		leal  irq_handler_array,%edi
		call  *\id*4(%edi)
		addl  $4, %esp

		/* Restore the context */
		popw  %gs
		popw  %fs
		popw  %es
	        popw  %ds
		popw  %ss
		addl  $2,%esp
		popl  %eax
		popl  %ebx
		popl  %ecx
		popl  %edx
		popl  %esi
		popl  %edi
		popl  %ebp

		/* Remove fake error code */
		addl  $4, %esp

		iret
	.endr
/* Build the irq_wrapper_array, shared with irq.c */
.section ".rodata"
.p2align 5, 0x0
irq_wrapper_array:
	.irp id, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
		.long (irq_wrapper_\id)
	.endr
