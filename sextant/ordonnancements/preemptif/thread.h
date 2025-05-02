/* Copyright (C) 2004,2005 David Decotigny

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
#ifndef _THREAD_H_
#define _THREAD_H_

/**
 * @file thread.h
 *
 * SOS Thread management API
 */

#include <sextant/types.h>
#include <drivers/Ecran.h>
#include <sextant/interruptions/irq.h>
#include <sextant/memoire/Memoire.h>
#include <sextant/ordonnancements/cpu_context.h>
#include <sextant/ordonnancements/preemptif/sched.h>
//#include <sextant/ordonnancements/time.h>


#define THREAD_KERNEL_STACK_SIZE (1024)
#define MAX_THREAD 10

/* Forward declaration */
struct thread;
typedef enum { YIELD_MYSELF, BLOCK_MYSELF, WAIT_MYSELF } switch_type_t;

typedef struct {
	char stack[THREAD_KERNEL_STACK_SIZE];
} stack_t;
/**
 * The possible states of a valid thread
 */
typedef enum { THR_CREATED, /**< Thread created, not fully initialized */
	       THR_READY,   /**< Thread fully initialized or
				     waiting for CPU after having been
				     blocked or preempted */
	       THR_RUNNING, /**< Thread currently running on CPU */
	       THR_BLOCKED, /**< Thread waiting for I/O (+ in at LEAST
				     one kwaitq) and/or sleeping (+ in NO
				     kwaitq) */
	       THR_WAITING, /**SEM*/
	       THR_ZOMBIE,  /**< Thread terminated execution, waiting to
				     be deleted by kernel */
               EMPTY,
             } thread_state_t;


/**
 * TCB (Thread Control Block): structure describing a thread. Don't
 * access these fields directly: prefer using the accessor functions
 * below.
 */
struct thread
{
#define THR_MAX_NAMELEN 32
  char name[THR_MAX_NAMELEN];
  int pid;

  thread_state_t  state;

  /**
   * The hardware context of the thread.
   *
   * It will reflect the CPU state of the thread:
   *  - From an interrupt handler: the state of the thread at the time
   *    of the OUTERMOST irq. An IRQ is not allowed to make context
   *    switches, so this context will remain valid from the begining of
   *    the outermost IRQ handler to the end of it, no matter if there
   *    are other IRQ handlers nesting in one another. You may safely
   *    use it from IRQ handlers to query the state of the interrupted
   *    thread, no matter if there has been other IRQ handlers
   *    executing meanwhile.
   *  - From normal kernel code, exceptions and syscall: the state of
   *    the thread the last time there was a context switch from this
   *    thread to another one. Thus this field WON'T reflect the
   *    current's thread cpu_state in these cases. So, in these cases,
   *    simply DO NOT USE IT outside thread.c ! Note: for syscall and
   *    exception handlers, the VALID state of the interrupted thread is
   *    passed as an argument to the handlers.
   */
  struct cpu_state *cpu_state;

  /* Kernel stack parameters */
  vaddr_t kernel_stack_base_addr;
  size_t  kernel_stack_size;

  /* Data specific to each state */
  union
  {
    struct
    {
      struct thread     *rdy_prev, *rdy_next;
    } ready;
  }; /* Anonymous union (gcc extenion) */
};


/**
 * Definition of the function executed by a kernel thread
 */
typedef void (*kernel_thread_start_routine_t)(void *arg);


/**
 * Initialize the subsystem responsible for thread management
 *
 * Initialize the primary kernel thread so that it can be handled the
 * same way as an ordinary thread created by thread_create().
 */
sextant_ret_t thread_subsystem_setup(vaddr_t init_thread_stack_base_addr,
				     size_t init_thread_stack_size);


/**
 * Create a new kernel thread
 */
struct thread *
create_kernel_thread(kernel_thread_start_routine_t start_func,
			 void *start_arg);


/**
 * Terminate the execution of the current thread. For kernel threads,
 * it is called by default when the start routine returns.
 */
void thread_exit() __attribute__((noreturn));


/**
 * Get the identifier of the thread currently running on CPU. Trivial
 * function.
 */
struct thread *thread_get_current();


/**
 * If thr == NULL, get the state of the current thread. Trivial
 * function.
 *
 * @note NOT protected against interrupts
 */
thread_state_t thread_get_state(struct thread *thr);


/**
 * Yield CPU to another ready thread.
 *
 * @note This is a BLOCKING FUNCTION
 */
typedef unsigned char sched_priority_t;
sextant_ret_t thread_yield();
void sleep_timeout(struct sos_timeout_action*);
sextant_ret_t thread_force_unblock(struct thread *);
sextant_ret_t thread_sleep(struct time *);

void sched_clk(int intid);
sextant_ret_t thread_wait();



#endif /* _THREAD_H_ */
