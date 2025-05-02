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
#include "gdt.h"

/** The actual GDT */
static struct x86_segment_descriptor gdt[15];

sextant_ret_t gdt_setup(void) {
	gdt[0] = (struct x86_segment_descriptor){ 0 };
	gdt[1] = BUILD_GDTE(0, 1);
	gdt[2] = BUILD_GDTE(0, 0);
	gdt[3] = BUILD_GDTE(3, 1);
	gdt[4] = BUILD_GDTE(3, 0);
	gdt[5] = (struct x86_segment_descriptor){ 0 };
	struct x86_gdt_register gdtr;
	
	/* Put some garbage in the padding field of the GDTR */
	gdtr.padding   = ~0;	

  /* Address of the GDT */
  gdtr.base_addr = (ui32_t) gdt;
  /* The limit is the maximum offset in bytes from the base address of
     the GDT */
  gdtr.limit     = sizeof(gdt) - 1;
  /* Commit the GDT into the CPU, and update the segment
     registers. The CS register may only be updated with a long jump
     to an absolute address in the given segment (see Intel x86 doc
     vol 3, section 4.8.1). */
  asm volatile ("lgdt %0          \n\
		  ljmp %1,$1f      \n\
		  1:               \n\
		  movw %2, %%ax    \n\
		  movw %%ax,  %%ss \n\
		  movw %%ax,  %%ds \n\
		  movw %%ax,  %%es \n\
		  movw %%ax,  %%fs \n\
		  movw %%ax,  %%gs"
		  :
		  :"m"(gdtr.limit),
		  "i"(BUILD_SEGMENT_REG_VALUE(0, false, SEG_KCODE)),
		  "i"(BUILD_SEGMENT_REG_VALUE(0, false, SEG_KDATA))
		  :"memory","eax");
  return SEXTANT_OK;
}
