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
#ifndef TIME_H
#define TIME_H

/**
 * @file time.h
 *
 * Primitives and callbacks related to kernel time management (timer
 * IRQ)
 */

#include <sextant/types.h>
#include <sextant/list.h>
#include <interruption/irq.h>
#include <memoire/memoire.h>

/**
 * Number of nanoseconds in 1 second
 */
#define NS_IN_SEC 1000000000UL

/**
 * Initialize a timeout action. MUST be called immediately after
 * (stack or kmalloc) allocation of the action.
 *
 * @param ptr_act Pointer to the action to initialize.
 */
#define time_init_action(ptr_act) \
  ({ (ptr_act)->tmo_prev = (ptr_act)->tmo_next = (struct timeout_action*)NULL1; /* return */ SEXTANT_OK; })

/* =======================================================================
 * Library of time manipulation functions
 */
struct time {
  ui32_t sec;
  ui32_t nanosec;
};

/* =======================================================================
 * Routines to schedule future execution of routines: "timeout" actions
 */

/* Forward declaration */
struct timeout_action;

/**
 * Prototype of a timeout routine. Called with IRQ disabled !
 */
typedef void (timeout_routine_t)(struct timeout_action *);


/**
 * The structure of a timeout action. This structure should have been
 * opaque to the other parts of the kernel. We keep it public here so
 * that struct timeout_action can be allocated on the stack from
 * other source files in the kernel. However, all the fields should be
 * considered read-only for modules others than time.{ch}.
 *
 * @note After an action has been allocated (on the stack or kmalloc),
 * it MUST be initialized with time_init_action below !
 */
struct timeout_action {
  /** PUBLIC: Address of the timeout routine */
  timeout_routine_t *routine;

  /** PUBLIC: (Custom) data available for this routine */
  void                  *routine_data;
  
  /** PUBLIC: 2 meanings:
   *  - before and while in the timeout list: absolute due date of the
   *    timeout action
   *  - once removed from timeout list: the time remaining in the
   *    initial timeout (might be 0 if timeout expired) at removal
   *    time
   */
  struct time           timeout;

  /** PRIVATE: To chain the timeout actions */
  struct timeout_action *tmo_prev, *tmo_next;
};

sextant_ret_t time_inc(struct time *dest,
		       const struct time *to_add);

sextant_ret_t time_dec(struct time *dest,
		       const struct time *to_dec);

int time_cmp(const struct time *t1,
		 const struct time *t2);

bool time_is_zero(const struct time *tm);



/* =======================================================================
 * Kernel time management. This is not the same as the "system-time",
 * ie it does not not take into account the system-time adjustments
 * (NTP, daylight saving times, etc...): this is the job of a
 * system-time subsystem.
 */


/**
 * Initialize kernel time subsystem.
 *
 * @param initial_resolution The initial time resolution. MUST be
 * consistent with that of the hardware timer
 */
sextant_ret_t time_subsysem_setup(const struct time *initial_resolution);


/**
 * Value of the interval between 2 time ticks. Should be consistent
 * with the configuration of the hardware timer.
 */
sextant_ret_t time_get_tick_resolution(struct time *resolution);


/**
 * Change the value of the interval between 2 time ticks. Must be
 * called each time the hardware timer is reconfigured.
 *
 * @note MUST be consistent with that of the hardware timer
 */
sextant_ret_t time_set_tick_resolution(const struct time *resolution);


/**
 * Get the time elapsed since system boot. Does not take into account
 * the system-time adjustment (NTP, daylight saving times, etc...):
 * this is the job of a system-time subsystem.
 */
sextant_ret_t time_get_now(struct time *now);

/**
 * Add the given action in the timeout list, so that it will be
 * triggered after the specified delay RELATIVE to the time when the
 * function gets called. The action is always inserted in the list.
 *
 * @param act The action to be initialized by the function upon
 * insertion in the timeout list.
 *
 * @param delay Delay until the action is fired. If 0, then it is
 * fired at next timer IRQ. The action will be fired in X ticks, with
 * X integer and >= delay.
 *
 * @param routine The timeout routine to call when the timeout will be
 * triggered.
 *
 * @param routine_data The data available to the routine when it will
 * be called.
 *
 * @note 'act' MUST remain valid until it is either fired or removed
 * (with time_remove_action)
 */
sextant_ret_t
time_register_action_relative(struct timeout_action *act,
				  const struct time *delay,
				  timeout_routine_t *routine,
				  void *routine_data);


/**
 * Add the given action in the timeout list, so that it will be
 * triggered after the specified ABSOLUTE date (relative to system
 * boot time). The action is always inserted in the list.
 *
 * @param act The action to be initialized by the function upon
 * insertion in the timeout list.
 *
 * @param date Absolute date (relative to system boot time) when the
 * action will be triggered.
 *
 * @param routine The timeout routine to call when the timeout will be
 * triggered.
 *
 * @param routine_data The data available to the routine when it will
 * be called.
 *
 * @note 'act' MUST remain valid until it is either fired or removed
 * (with time_remove_action)
 */
sextant_ret_t time_register_action_absolute(struct timeout_action *act,
				  const struct time *date,
				  timeout_routine_t *routine,
				  void *routine_data);


/**
 * The action is removed and its timeout is updated to reflect the
 * time remaining.
 */
sextant_ret_t time_unregister_action(struct timeout_action *act);


/**
 * Timer IRQ callback. Call and remove expired actions from the list.
 *
 * @note The use of this function is RESERVED (to timer IRQ)
 */
sextant_ret_t time_do_tick(void);


#endif
