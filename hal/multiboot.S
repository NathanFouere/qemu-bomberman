/* Copyright (C) 1999  Free Software Foundation, Inc.

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

	
/* The operating system is booted by Grub, so we almost have nothing
   to do to boot it. We only have to conform to the Multiboot
   standard, as defined by the Grub documentation */
	
#define ASM 1
/* The multiboot.h header contains a lot of multiboot standard
   definitions */
#include "multiboot.h"

	/* The multiboot header itself. It must come first. */
.section ".multiboot"
	/* Multiboot header must be aligned on a 4-byte boundary */
	.align 4
multiboot_header:
  /* magic=        */ .long MULTIBOOT_HEADER_MAGIC
  /* flags=        */ .long MULTIBOOT_HEADER_FLAGS
  /* checksum=     */ .long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
  /* header_addr=  */ .long multiboot_header
  /* load_addr=    */ .long __b_load
  /* load_end_addr=*/ .long __e_load
  /* bss_end_addr= */ .long __e_kernel
  /* entry_addr=   */ .long multiboot_entry
	
/* Here is the beginning of the code of our operating system */
.text

.globl start, _start
start:
_start:
multiboot_entry:
	/* Set up a stack */
	movl $(stack + MULTIBOOT_STACK_SIZE), %ebp
	movl %ebp, %esp

	/* Set EFLAGS to 0 */
	pushl $0
	/* pop stack into the EFLAGS register */
	popf

	/* Push the magic and the address on the stack, so that they
	will be the parameters of the cmain function */
	pushl %ebx
	pushl %eax

	/* Call the cmain function (os.c) */
	call EXT_C(Sextant_main)

	/* Should never get there */
loop:
	hlt
	jmp loop

/* Here is the stack */
.section ".init_stack", "aw", @nobits
.size stack, MULTIBOOT_STACK_SIZE
stack:
	.space MULTIBOOT_STACK_SIZE

/* Some data characterizing the stack addresses */
.data
	.globl bootstrap_stack_bottom
bootstrap_stack_bottom: .long stack

	.globl bootstrap_stack_size
bootstrap_stack_size: .long MULTIBOOT_STACK_SIZE
