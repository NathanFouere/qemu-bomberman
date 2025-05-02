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
#ifndef GDT_H
#define GDT_H

/**
 * @file gdt.h
 *
 * The routines that manage the GDT, the table that maps the virtual
 * addresses (data/instructions, segment-relative), to "linear"
 * addresses (ie paged-memory). In SOS/x86, we use a "flat" virtual
 * space, ie the virtual and linear spaces are equivalent.
 *
 * @see Intel x86 doc vol 3, chapter 3
 */

#include <hal/segment.h>
#include <sextant/types.h>

/**
 * Helper macro that builds a Segment descriptor for the virtual
 * 0..4GB addresses to be mapped to the linear 0..4GB linear
 * addresses.
 */
#define BUILD_GDTE(descr_privilege_level,is_code)               \
  ((struct x86_segment_descriptor) {                            \
      0xffff,                           \
      0,                                \
      0,                                \
      ((is_code)?0xb:0x3),              \
             /* With descriptor_type (below) = 1 (code/data),   \
	      * see Figure 3-1 of section 3.4.3.1 in Intel      \
	      * x86 vol 3:                                      \
	      *   - Code (bit 3 = 1):                           \
	      *     bit 0: 1=Accessed                           \
	      *     bit 1: 1=Readable                           \
	      *     bit 2: 0=Non-Conforming                     \
	      *   - Data (bit 3 = 0):                           \
	      *     bit 0: 1=Accessed                           \
	      *     bit 1: 1=Writable                           \
	      *     bit 2: 0=Expand up (stack-related)          \
	      * For Conforming/non conforming segments, see     \
	      * Intel x86 Vol 3 section 4.8.1.1                 \
	      */                                                \
      1,  /* 1=Code/Data */             \
      ((descr_privilege_level) & 0x3),  \
      1,                                \
      0xf,                              \
      0,                                \
      0,                                \
      1,  /* 32 bits instr/data */      \
      1,   /* limit is in 4kB Pages */  \
      0  						        \
  })

/**
 * The sructure of a segment descriptor.
 *
 * @see Intel x86 doc, Vol 3, section 3.4.3, figure 3-8. For segment
 * types, see section 3.5
 */
struct x86_segment_descriptor {
  /* Lowest dword */
  ui16_t limit_15_0;            /* Segment limit, bits 15..0 */
  ui16_t base_paged_addr_15_0;  /* Base address, bits 15..0 */
  /* Highest dword */
  ui8_t  base_paged_addr_23_16; /* Base address bits 23..16 */
  ui8_t  segment_type:4;        /* Section 3.4.3.1 (code/data) and 3.5 (system) of Intel x86 vol 3 */
  ui8_t  descriptor_type:1;     /* 0=system, 1=Code/Data */
  ui8_t  dpl:2;
  ui8_t  present:1;
  ui8_t  limit_19_16:4;         /* Segment limit, bits 19..16 */
  ui8_t  custom:1;
  ui8_t  zero:1;
  ui8_t  op_size:1;             /* 0=16bits instructions, 1=32bits */
  ui8_t  granularity:1;         /* 0=limit in bytes, 1=limit in pages */
  ui8_t  base_paged_addr_31_24; /* Base address bits 31..24 */
} __attribute__ ((packed,aligned(8)));

/**
 * The GDT register, which stores the address and size of the
 * GDT.
 *
 * @see Intel x86 doc vol 3, section 2.4, figure 2-4; and section
 * 3.5.1
 */
struct x86_gdt_register {
	/** Intel doc says that the real GDT register (ie the "limit" field)
	     should be odd-word aligned. That's why we add a padding here.
	      Credits to Romain for having signalled this to us. */
	ui16_t  padding;
	/** The maximum GDT offset allowed to access an entry in the GDT */
	ui16_t  limit;
	/**
	 * This is not exactly a "virtual" address, ie an adddress such as
	 * those of instructions and data; this is a "linear" address, ie an
	 * address in the paged memory. However, in SOS we configure the
	 * segmented memory as a "flat" space: the 0-4GB segment-based (ie
	 * "virtual") addresses directly map to the 0-4GB paged memory (ie
	 * "linear"), so that the "linear" addresses are numerically equal
	 * to the "virtual" addresses: this base_addr will thus be the same
	 * as the address of the gdt array
	 */
	ui32_t base_addr;
} __attribute__((packed, aligned(4)));

/**
 * Configure the virtual space as a direct mapping to the linear
 * address space (ie "flat" virtual space).
 */
sextant_ret_t gdt_setup(void);
#endif
