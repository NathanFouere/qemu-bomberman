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
#include "idt.h"


sextant_ret_t idt_setup()
{
  struct x86_idt_register idtr;
  int i;

  for (i = 0 ;
       i < IDTE_NUM ;
       i++)
    {
      struct x86_idt_entry *idte = idt + i;

      /* Setup an empty IDTE interrupt gate, see figure 5-2 in Intel
	 x86 doc, vol 3 */
//      idte->seg_sel   = BUILD_SEGMENT_REG_VALUE(0, false, SEG_KCODE);
      idte->seg_sel   = 0x08; // numéro segment de code définit par défaut par Grub lors de l'initailiasation flat mode de la GDT
      idte->reserved  = 0;
      idte->flags     = 0;
//     idte->type      = 0x6; /* Interrupt gate (110b) 16 bits */
      idte->type      = 0xE; /* Interrupt gate (110b) */
      idte->op_size   = 1;   /* 32bits instructions */
      idte->zero      = 0;

      /* Disable this IDT entry for the moment */
      idt_set_handler(i, (vaddr_t)NULL1, 0/* Don't care */);
    }

  /*
   * Setup the IDT register, see Intel x86 doc vol 3, section 5.8.
   */

  /* Address of the IDT */
  idtr.base_addr  = (ui32_t) idt;

  /* The limit is the maximum offset in bytes from the base address of
     the IDT */
  idtr.limit      = sizeof(idt) - 1;

  /* Commit the IDT into the CPU */
  asm volatile ("lidt %0\n"::"m"(idtr):"memory");

  return SEXTANT_OK;
}


sextant_ret_t idt_set_handler(int index,
			      vaddr_t handler_address,
			      int lowest_priviledge /* 0..3 */)
{
  struct x86_idt_entry *idte;

  if ((index < 0) || (index >= IDTE_NUM))
    return -SEXTANT_ERROR;
  if ((lowest_priviledge < 0) || (lowest_priviledge > 3))
    return -SEXTANT_ERROR;

  idte = idt + index;
  if (handler_address != (vaddr_t)NULL1)
    {
      idte->offset_low  = handler_address & 0xffff;
      idte->offset_high = (handler_address >> 16) & 0xffff;
      idte->dpl         = lowest_priviledge;
      idte->present     = 1;   /* Yes, there is a handler */
    }
  else /* Disable this IDT entry */
    {
      idte->offset_low  = 0;
      idte->offset_high = 0;
      idte->dpl         = 0;
      idte->present     = 0;   /* No, there is no handler */
    }

  return SEXTANT_OK;
}


sextant_ret_t idt_get_handler(int index,
			      vaddr_t *handler_address,
			      int *lowest_priviledge)
{
  if ((index < 0) || (index >= IDTE_NUM))
    return -SEXTANT_ERROR;

  if (handler_address != NULL1)
    *handler_address = idt[index].offset_low
                       | (idt[index].offset_high << 16);
  if (lowest_priviledge != NULL1)
    *lowest_priviledge = idt[index].dpl;

  return SEXTANT_OK;
}
