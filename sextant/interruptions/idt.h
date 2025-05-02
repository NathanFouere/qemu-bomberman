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
#ifndef _IDT_H_
#define _IDT_H_

/**
 * @file idt.h
 *
 * Manage the x86 Interrupt Descriptor Table, the table which maps the
 * hardware interrupt lines, hardware exceptions, and software
 * interrupts, to software routines. We only define "interrupt gate"
 * IDT entries. Don't use it directly; refer instead to interrupt.c,
 * exceptions.c and syscall.c.
 *
 * @see Intel x86 doc, Vol 3, chapter 5
 */

#include <sextant/types.h>
//#include <hal/segment.h>


/* Mapping of the CPU exceptions in the IDT (imposed by Intel
   standards) */
#define EXCEPT_BASE 0
#define EXCEPT_NUM  32
#define EXCEPT_MAX  (HWEXCEPT_BASE + HWEXCEPT_NUM - 1)

/* Mapping of the IRQ lines in the IDT */
#define IRQ_BASE    32
#define IRQ_NUM     16
#define IRQ_MAX     (IRQ_BASE + IRQ_NUM - 1)

/**
 * Number of IDT entries.
 *
 * @note Must be large enough to map the hw interrupts, the exceptions
 * (=> total is 48 entries), and the syscall(s). Since our syscall
 * will be 0x42, it must be >= 0x43. Intel doc limits this to 256
 * entries, we use this limit.
 */
#define IDTE_NUM      256 /* 0x100 */

/**
 * An entry in the IDT, or "IDTE" in the following, ie a reference to
 * a interrupt/trap routine or a task gate to handle the sw/hw
 * interrupts and exceptions.
 *
 * @see figure 5-2, intel x86 doc, vol 3
 */
struct x86_idt_entry
{
  /* Low dword */
  ui16_t offset_low;  /* 15..0, offset of the routine in the segment */
  ui16_t seg_sel;     /* 31..16, the ID of the segment */

  /* High dword */
  ui8_t reserved:5;   /* 4..0 */
  ui8_t flags:3;      /* 7..5 */
  ui8_t type:3;       /* 10..8 (interrupt gate, trap gate...) */
  ui8_t op_size:1;    /* 11 (0=16bits instructions, 1=32bits instr.) */
  ui8_t zero:1;       /* 12 */
  ui8_t dpl:2;        /* 14..13 */
  ui8_t present:1;    /* 15 */
  ui16_t offset_high; /* 31..16 */
} __attribute__((packed));


/**
 * The IDT register, which stores the address and size of the
 * IDT.
 *
 * @see Intel x86 doc vol 3, section 2.4, figure 2-4
 */
struct x86_idt_register
{
  /* The maximum GDT offset allowed to access an entry in the GDT */
  ui16_t  limit;

  /* This is not exactly a "virtual" address, ie an adddress such as
     those of instructions and data; this is a "linear" address, ie an
     address in the paged memory. However, in SOS we configure the
     segmented memory as a "flat" space: the 0-4GB segment-based (ie
     "virtual") addresses directly map to the 0-4GB paged memory (ie
     "linear"), so that the "linear" addresses are numerically equal
     to the "virtual" addresses: this base_addr will thus be the same
     as the address of the gdt array */
  ui32_t base_addr;
} __attribute__((packed, aligned (8)));


static struct x86_idt_entry    idt[IDTE_NUM];

/** Initialization routine: all the IDT entries (or "IDTE") are marked
    "not present". */
sextant_ret_t idt_setup(void);

/**
 * Enable the IDT entry if handler_address != NULL, with the given
 * lowest_priviledge.\ Disable the IDT entry when handler_address ==
 * NULL (the lowest_priviledge parameter is then ignored). Intel doc
 * says that there must not be more than 256 entries.
 *
 * @note IRQ Unsafe
 */
sextant_ret_t idt_set_handler(int index,
			      vaddr_t handler_address,
			      int lowest_priviledge /* 0..3 */);


/**
 * @note IRQ Unsafe
 *
 * @return the handler address and DPL in the 2nd and 3rd
 * parameters
 */
sextant_ret_t idt_get_handler(int index,
			      vaddr_t *handler_address,
			      int *lowest_priviledge);

#endif
