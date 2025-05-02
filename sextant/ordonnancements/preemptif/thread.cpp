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


#include "thread.h"


/**
 * The identifier of the thread currently running on CPU.
 *
 * We only support a SINGLE processor, ie a SINGLE thread
 * running at any time in the system. This greatly simplifies the
 * implementation of the system, since we don't have to complicate
 * things in order to retrieve the identifier of the threads running
 * on the CPU. On multiprocessor systems the current_thread below is
 * an array indexed by the id of the CPU, so that the challenge is to
 * retrieve the identifier of the CPU. This is usually done based on
 * the stack address (Linux implementation) or on some form of TLS
 * ("Thread Local Storage": can be implemented by way of LDTs for the
 * processes, accessed through the fs or gs registers).
 */
static volatile struct thread *current_thread = NULL_THREAD;

extern "C" {
	void cpu_context_switch(cpu_state**, cpu_state*);
	void cpu_context_exit_to(struct cpu_state *,cpu_kstate_function_arg1_t *,
			ui32_t reclaiming_arg) __attribute__((noreturn));
}

static struct thread thread_list[MAX_THREAD];
stack_t thread_stack[MAX_THREAD];

void sched_clk(int intid) {
   thread_yield();
}

struct thread *thread_get_current(){
	(current_thread->state == THR_RUNNING);
	return (struct thread*)current_thread;
}

inline static sextant_ret_t set_current(struct thread *thr){
	current_thread = thr;
	current_thread->state = THR_RUNNING;
	return SEXTANT_OK;
}

sextant_ret_t thread_subsystem_setup(vaddr_t init_thread_stack_base_addr,size_t init_thread_stack_size) {
	struct thread *myself;
	int i;

	for (i=0;i<MAX_THREAD;i++)
		thread_list[i].state = EMPTY;
	/* Allocate a new thread structure for the current running thread */
	i=0;
	myself=NULL_THREAD;
	while (myself==NULL_THREAD && i < MAX_THREAD) {
		if (thread_list[i].state == EMPTY)
			myself = &(thread_list[i]);
		i++;
	}
	if (! myself)
		return -1;
	/* Initialize the thread attributes */
	myself->state = THR_CREATED;
	myself->kernel_stack_base_addr = init_thread_stack_base_addr;
	myself->kernel_stack_size = init_thread_stack_size;
	/* Ok, now pretend that the running thread is ourselves */
	myself->state = THR_READY;
	set_current(myself);
	return SEXTANT_OK;
}

struct thread * create_kernel_thread(kernel_thread_start_routine_t start_func,void *start_arg){
	int i;
	struct thread *new_thread;
	void *temp;

	if (! start_func)
		return NULL_THREAD;
	/* Allocate a new thread structure for the current running thread */
	i=0;
	new_thread=NULL_THREAD;
	while(new_thread==NULL_THREAD && i < MAX_THREAD) {
		if(thread_list[i].state == EMPTY)
			new_thread = &(thread_list[i]);
		else i++;
	}
	if (!new_thread)
		return NULL_THREAD;
	/* Initialize the thread attributes */
	new_thread->state = THR_CREATED;
	new_thread->pid = i;
	/* Allocate the stack for the new thread */
	temp = thread_stack[i].stack;
	new_thread->kernel_stack_base_addr = (vaddr_t) temp;
	new_thread->kernel_stack_size = THREAD_KERNEL_STACK_SIZE;
	if (! new_thread->kernel_stack_base_addr) {
		new_thread->state = EMPTY;
		return NULL_THREAD;
	}
	/* Initialize the CPU context of the new thread */
	if (SEXTANT_OK != cpu_kstate_init(& new_thread->cpu_state,(cpu_kstate_function_arg1_t*) start_func,
			(ui32_t) start_arg,new_thread->kernel_stack_base_addr,new_thread->kernel_stack_size,
			(cpu_kstate_function_arg1_t*) thread_exit,(ui32_t) NULL_THREAD)) {
		new_thread->state = EMPTY;
		return NULL_THREAD;
	}
	/* Mark the thread ready */
	if(SEXTANT_OK != sched_set_ready(new_thread)) {
		new_thread->state = EMPTY;
		return NULL_THREAD;
	}
	/* Normal non-erroneous end of function */
	return new_thread;
}

/** Function called after thr has terminated. Called from inside the context
 *     of another thread, interrupts disabled */
static void delete_thread(struct thread *thr){
	thr->state = EMPTY;
}

void thread_exit() {
	struct thread *myself, *next_thread;
	myself = thread_get_current();
	/* Prepare to run the next thread */
	myself->state = THR_ZOMBIE;
	next_thread = reschedule(myself, false);
	/* Immediate switch to next thread */
	set_current(next_thread);
	cpu_context_exit_to(next_thread->cpu_state,(cpu_kstate_function_arg1_t*) delete_thread,(ui32_t) myself);
}

thread_state_t thread_get_state(struct thread *thr) {
	if (! thr)
		thr = (struct thread*)current_thread;
	return thr->state;
}

/**
 * Helper function to initiate a context switch in case the current
 * thread becomes blocked, waiting for a timeout, or calls yield.
 */
static sextant_ret_t switch_to_next_thread(switch_type_t operation) {
	struct thread *myself, *next_thread;
	(current_thread->state == THR_RUNNING);
	/* Interrupt handlers are NOT allowed to block ! */
	myself = (struct thread*)current_thread;
	/* Make sure that if we are to be marked "BLOCKED", we have any reason of effectively being blocked */
	if(BLOCK_MYSELF == operation)
		myself->state = THR_BLOCKED;
	/* Identify the next thread */
	next_thread = reschedule(myself,(YIELD_MYSELF == operation));
	/* Avoid context switch if the context does not change */
	if (myself != next_thread) {
		//Actual CPU context switch
		set_current(next_thread);
		cpu_context_switch(&myself->cpu_state, next_thread->cpu_state);
	}
	else/* No context switch but still update ID of current thread */
		set_current(next_thread);
	return SEXTANT_OK;
}

sextant_ret_t thread_yield() {
	sextant_ret_t retval;
	retval = switch_to_next_thread(YIELD_MYSELF);
	return retval;
}

sextant_ret_t thread_wait() {
	sextant_ret_t retval;
	retval = switch_to_next_thread(WAIT_MYSELF);
	return retval;
}

/**
 * Internal sleep timeout management
 */
struct sleep_timeout_params {
	struct thread *thread_to_wakeup;
	bool timeout_triggered;
};


