/* Copyright (C) 2004  The SOS Team
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
#ifndef _HWSEGS_H_
#define _HWSEGS_H_

/**
 * @file segments.h
 *
 * Global and local (GDT/LDT) segment descriptor definition and
 * structure. These segments map virtual addresses (ie
 * data/instruction addresses, relative to these segment descriptors)
 * to linear addresses (ie addresses in the paged-memory space).
 *
 * @see Intel x86 doc, vol 3 chapter 3.
 */

#include <sextant/types.h>

/*
 * Global segment selectors (GDT) for SOS/x86.
 *
 * @see gdt.h
 */
#define SEG_NULL  0 /* NULL segment, unused by the procesor */
#define SEG_KCODE 1 /* Kernel code segment */
#define SEG_KDATA 2 /* Kernel data segment */


/**
 * Helper macro that builds a segment register's value
 */
#define BUILD_SEGMENT_REG_VALUE(desc_privilege,in_ldt,seg_index) \
  (  (((desc_privilege) & 0x3)  << 0) \
   | (((in_ldt)?1:0)            << 2) \
   | ((seg_index)               << 3) )


/*
 * Local segment selectors (LDT) for SOS/x86
 */
/* None */

#endif /* _HWSEGS_H_ */
