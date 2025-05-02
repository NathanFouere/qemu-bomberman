/* Copyright (C) 2005  David Decotigny
   Copyright (C) 2000-2004, The KOS team

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

         
.file "cpu_context_switch.S"

.text

	
.globl cpu_context_switch
.type cpu_context_switch, @function
cpu_context_switch:
        // arg2= to_context    --    esp+64
        // arg1= from_context  --    esp+60
        // caller ip           --    esp+56
  pushf              // (eflags)     esp+52
  pushl %cs          // (cs)         esp+48
  pushl $resume_pc   // (ip)         esp+44
  pushl $0           // (error code) esp+40
  pushl %ebp         //              esp+36
  pushl %edi         //              esp+32
  pushl %esi         //              esp+28
  pushl %edx         //              esp+24
  pushl %ecx         //              esp+20
  pushl %ebx         //              esp+16
  pushl %eax         //              esp+12
  subl $2, %esp      // (alignment)  esp+10
  pushw %ss          //              esp+8
  pushw %ds          //              esp+6
  pushw %es          //              esp+4
  pushw %fs          //              esp+2
  pushw %gs          //              esp

  /*
   * Now that the original eax/ebx are stored, we can use them safely
   */
	
  /* Store the address of the saved context */
  movl  60(%esp), %ebx
  movl  %esp, (%ebx)

  /* This is the proper context switch ! We change the stack here */
  movl 64(%esp), %esp

  /* Restore the CPU context */
  popw %gs
  popw %fs
  popw %es
  popw %ds
  popw %ss
  addl $2,%esp
  popl %eax
  popl %ebx
  popl %ecx
  popl %edx
  popl %esi
  popl %edi
  popl %ebp
  addl $4, %esp /* Ignore "error code" */

  /* This restores the eflags, the cs and the eip registers */
  iret /* equivalent to: popfl ; ret */

resume_pc:
        // Same context as that when cpu_context_switch got called
        // arg2= to_context    -- esp+8
        // arg1= from_context  -- esp+4
        // caller ip           -- esp
  ret



/* ------------------------- */
.globl cpu_context_exit_to
.type cpu_context_exit_to, @function
cpu_context_exit_to:
        // arg3= reclaiming_arg  -- esp+12
        // arg2= reclaiming_func -- esp+8
        // arg1= to_context      -- esp+4
        // caller ip             -- esp

  /* Store the current SP in a temporary register */
  movl %esp, %eax

  /* This is the proper context switch ! We change the stack here */
  movl 4(%eax), %esp

  /* Call the reclaiming function (remember: the old frame address
     is stored in eax) */
  pushl 12(%eax)
  call  *8(%eax)
  addl  $4, %esp

  /* Restore the CPU context */
  popw %gs
  popw %fs
  popw %es
  popw %ds
  popw %ss
  addl $2,%esp
  popl %eax
  popl %ebx
  popl %ecx
  popl %edx
  popl %esi
  popl %edi
  popl %ebp
  addl $4, %esp /* Ignore "error code" */

  /* This restores the eflags, the cs and the eip registers */
  iret /* equivalent to: popfl ; ret */
