/* Copyright (C) 2003, Thomas Petazzoni

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

/* We generate binary in the ELF format */
OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386");

/* The entry point is _start (defined in boot.S) */
ENTRY(_start)

/* The architecture is i386 */
OUTPUT_ARCH("i386")

SECTIONS
{
    /* our kernel is loaded at 0x200000 */
    . = 0x200000;
    __b_load   = .;

    /* the multiboot header MUST come early enough in the output
       object file */
    .multiboot :
    {
        /* The multiboot section (containing the multiboot header)
           goes here */
	*(.multiboot);
	
	/*
	 * With the following line, we force this section to be
	 * allocated in the output file as soon as possible, no matter
	 * when the file containing the multiboot header (multiboot.S)
	 * is compiled. This is to conform to the multiboot spec, which
	 * says "The Multiboot header must be contained completely
	 * within the first 8192 bytes of the OS image, and must be
	 * longword (32-bit) aligned."
	 */
	LONG(0);
    }

    /* Defines a symbol '__b_kernel to mark the start of the kernel
       code/data */
    . = ALIGN(4096);
    __b_kernel = .;
  
    /* Beginning of the text section */
    .text ALIGN(4096) :
    {   
	/* This section includes the code */
        *(.text*)
	/* Defines the 'etext' and '_etext' at the end */
        PROVIDE(etext = .);
        PROVIDE(_etext = .);
    }

    __b_data = .;

    /* Beginning of the data section */
    .data . :
    {   *(.data*) 
        PROVIDE(edata = .);
	PROVIDE(_edata = .);
    }

    /* Beginning of the read-only data section */
    .rodata . :
    {   *(.rodata*)
        PROVIDE(erodata = .);
        PROVIDE(_erodata = .);
    }
    /* We take note of the end of the data to load */
    __e_load = .;

    /* Beginning of the BSS section (global uninitialized data) */
    .bss SIZEOF(.rodata) + ADDR(.rodata) + 40000 :
    {   *(.bss)
        *(COMMON)

	/* We put the stack of the bootstrap thread on a page
           boundary, because it can be un-allocated later */
	. = ALIGN(4096);
	 __b_stack = .;
	*(.init_stack)

        PROVIDE(ebss = .);
	PROVIDE(_ebss = .);
    }

    /* We take note of the end of the kernel: this is where the GPFM
       will begin */
    __e_kernel = .;
}
