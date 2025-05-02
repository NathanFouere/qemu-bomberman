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

#include "sched.h"

/**
 * The definition of the scheduler queue. We could have used a normal
 * kwaitq here, it would have had the same properties. But, in the
 * definitive version (O(1) scheduler), the structure has to be a bit
 * more complicated. So, in order to keep the changes as small as
 * possible between this version and the definitive one, we don't use
 * kwaitq here.
 */
#define MAX_THREAD 10

int number_thread_ready;
int current_pos;
int number_thread_waiting;
int wait_current_pos;
struct uneStruct ready_thread[MAX_THREAD];
struct uneStructWaiting waiting_thread[MAX_THREAD];



sextant_ret_t sched_subsystem_setup() {
	int i;

	for (i=0;i<MAX_THREAD;i++) {
		ready_thread[i].free=1;
		waiting_thread[i].free=1;
	}
	number_thread_ready=0;
	number_thread_waiting = 0;
	current_pos=0;
	wait_current_pos = 0;

	return SEXTANT_OK;
}

/**
 * Helper function to add a thread in a ready queue AND to change the
 * state of the given thread to "READY".
 *
 * @param insert_at_tail TRUE to tell to add the thread at the end of
 * the ready list. Otherwise it is added at the head of it.
 */
static sextant_ret_t add_in_ready_queue(struct thread *thr,bool insert_at_tail){
	int i;
	int fin=0;

	if (number_thread_ready<MAX_THREAD) {
		/* Ok, thread is now really ready to be (re)started */
		thr->state = THR_READY;
		number_thread_ready++;
		fin=0;
		i=0;
		while (i<MAX_THREAD && fin==0) {
			if (ready_thread[i].free==1) {
				ready_thread[i].free=0;
				ready_thread[i].thread = thr;
				fin=1;
			}
			i++;
		}
	}
	return SEXTANT_OK;
}

struct thread * pop_in_ready_queue(){
	int found=0;

	while (!found) {
		if (current_pos>=MAX_THREAD) current_pos=0;
		if (ready_thread[current_pos].free==0) {
			found=1;
			ready_thread[current_pos].free=1;
			number_thread_ready--;
		}
		current_pos++;
	}
	return ready_thread[current_pos-1].thread;
}

static sextant_ret_t add_in_waiting_queue(struct thread *thr,bool insert_at_tail,int event){
	int i;
	int fin=0;

	if (number_thread_waiting<MAX_THREAD) {
		/* Ok, thread is now really ready to be (re)started */
		thr->state = THR_WAITING;
		number_thread_waiting++;
		fin=0;
		i=0;
		while (i<MAX_THREAD && fin==0) {
			if (waiting_thread[i].free==1) {
				waiting_thread[i].free=0;
				waiting_thread[i].thread = thr;
				waiting_thread[i].event = event;
				fin=1;
			}
			i++;
		}
	}
	return SEXTANT_OK;
}

struct thread * pop_in_waiting_queue(int event){
	int found=0;

	while (!found) {
		if (wait_current_pos>=MAX_THREAD) wait_current_pos=0;
		if ((waiting_thread[wait_current_pos].free==0) && (waiting_thread[wait_current_pos].event==event)) {
			found=1;
			waiting_thread[wait_current_pos].free=1;
			number_thread_waiting--;
		}
		wait_current_pos++;
	}
	return waiting_thread[wait_current_pos-1].thread;
}

sextant_ret_t sched_set_ready(struct thread *thr){
	sextant_ret_t retval;

	/* Don't do anything for already ready threads */
	if (THR_READY == thr->state)
		return SEXTANT_OK;
	/* Real-time thread: schedule it for the present turn */
	retval = add_in_ready_queue(thr, true);
	return retval;
}

sextant_ret_t sched_set_waiting(struct thread *thr,int event){
	sextant_ret_t retval;

	/* Don't do anything for already ready threads */
	if (THR_WAITING == thr->state)
		return SEXTANT_OK;
	/* Real-time thread: schedule it for the present turn */
	retval = add_in_waiting_queue(thr, true,event);
	return retval;
}

struct thread * reschedule(struct thread *current_thread,bool do_yield) {
	if(THR_ZOMBIE == current_thread->state)
	{
		/* Don't think of returning to this thread since it is
	 terminated */
		/* Nop */
	}
	else
	if (THR_BLOCKED != current_thread->state)
			if (THR_WAITING != current_thread->state)
				{
				/* Take into account the current executing thread unless it is
				 marked blocked */
				if (do_yield)/* Ok, reserve it for next turn */
					add_in_ready_queue(current_thread, true);
				else	/* Put it at the head of the active list */
					add_in_ready_queue(current_thread, false);
	}
	/* The next thread is that at the head of the ready list */
	if (number_thread_ready > 0)
	{
		struct thread *next_thr;
		/* Queue is not empty: take the thread at its head */
		next_thr = pop_in_ready_queue();
		return next_thr;
	}
	return NULL_THREAD;
}
