/* Copyright (C) 2004 David Decotigny

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
#ifndef _SCHED_H_
#define _SCHED_H_


/**
 * @file sched.h
 *
 * A basic scheduler with simple FIFO threads' ordering.
 *
 * The functions below manage CPU queues, and are NEVER responsible
 * for context switches (see thread.h for that) or synchronizations
 * (see kwaitq.h or the higher levels primitives [mutex, semaphore,
 * ...] for that).
 *
 * @note IMPORTANT: all the functions below are meant to be called
 * ONLY by the thread/timer/kwaitq subsystems. DO NOT use them
 * directly from anywhere else: use ONLY the thread/kwaitq functions!
 * If you still want to call them directly despite this disclaimer,
 * simply disable interrupts before clling them.
 */

#include <sextant/types.h>
#include <sextant/ordonnancements/preemptif/thread.h>

typedef unsigned char sched_priority_t;

struct uneStruct {
	unsigned int free;
	struct thread *thread;
};

struct uneStructWaiting {
	unsigned int free;
	struct thread *thread;
	unsigned int event;
};


/**
 * Initialize the scheduler
 *
 * @note: The use of this function is RESERVED
 */
sextant_ret_t sched_subsystem_setup();


/**
 * Mark the given thread as ready
 *
 * @note: The use of this function is RESERVED
 */
sextant_ret_t sched_set_ready(struct thread * thr);

sextant_ret_t sched_set_waiting(struct thread *thr,int event);
struct thread * pop_in_waiting_queue(int event);



/**
 * Return the identifier of the next thread to run. Also removes it
 * from the ready list, but does NOT set is as current_thread !
 *
 * @param current_thread TCB of the thread calling the function
 *
 * @param do_yield When TRUE, put the current executing thread at the
 * end of the ready list. Otherwise it is kept at the head of it.
 *
 * @note: The use of this function is RESERVED
 */
struct thread * reschedule(struct thread * current_thread, bool do_yield);
void sched_clk(int);

#endif /* _WAITQUEUE_H_ */
