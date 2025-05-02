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

#include "time.h"

/**
 * The list of timeout actions waiting for a timeout. The timeout
 * actions are stored in the list in increasing initial timeout
 * order. Actually, the "timeout" field won't reflect this initial
 * timeout: for each element in the list, it stores the timeout
 * _difference_ between the timeout action and the previous in the
 * list.
 */
static struct timeout_action *timeout_action_list;


/**
 * Current resolution of a time tick
 */
static struct time tick_resolution;


/**
 * Time elapsed between boot and last timer tick
 *
 * @note No 'volatile' here because the tick value is NEVER modified
 * while in any of the functions below: it is modified only out of
 * these functions by the IRQ timer handler because these functions
 * are protected against timer IRQ and are "one shot" (no busy waiting
 * for a change in the tick's value).
 */
static struct time last_tick_time;


sextant_ret_t time_inc(struct time *dest,
		       const struct time *to_add)
{
  /* nanosec is always < 1e9 so that their sum is always < 2e9, which
     is smaller than 2^32-1 */
  ui32_t sigma_ns = dest->nanosec + to_add->nanosec;
  
  dest->sec     += to_add->sec;
  dest->sec     += sigma_ns / NS_IN_SEC;
  dest->nanosec  = sigma_ns % NS_IN_SEC;
  return SEXTANT_OK;
}


sextant_ret_t time_dec(struct time *dest,
		       const struct time *to_dec)
{
  /* nanosec is always < 1e9 so that their difference is always in
     (-1e9, 1e9), which is compatible with the (-2^31, 2^31 - 1)
     cpacity of a signed dword */
  si32_t diff_ns = ((si32_t)dest->nanosec)
			- ((si32_t)to_dec->nanosec);

  dest->sec     -= to_dec->sec;
  if (diff_ns > 0)
    dest->sec     += diff_ns / NS_IN_SEC;
  else
    dest->sec     -= ((-diff_ns) / NS_IN_SEC);
  dest->nanosec  = (NS_IN_SEC + diff_ns) % NS_IN_SEC;
  if (diff_ns < 0)
    dest->sec --;
  return SEXTANT_OK;
}


int time_cmp(const struct time *t1,
		 const struct time *t2)
{
  /* Compare seconds */
  if (t1->sec < t2->sec)
    return -1;
  else if (t1->sec > t2->sec)
    return 1;

  /* seconds are equal => compare nanoseconds */
  else if (t1->nanosec < t2->nanosec)
    return -1;
  else if (t1->nanosec > t2->nanosec)
    return 1;

  /* else: sec and nanosecs are equal */
  return 0;
}


bool time_is_zero(const struct time *tm)
{
  return ( (0 == tm->sec) && (0 == tm->nanosec) );
}


sextant_ret_t time_subsysem_setup(const struct time *initial_resolution)
{
  timeout_action_list = (struct timeout_action*)NULL1;
  last_tick_time = (struct time) {0,0};
  memcpy(& tick_resolution, initial_resolution, sizeof(struct time));

  return SEXTANT_OK;
}


sextant_ret_t time_get_tick_resolution(struct time *resolution)
{
  ui32_t flags;
  disable_IRQs(flags);

  memcpy(resolution, & tick_resolution, sizeof(struct time));

  restore_IRQs(flags);
  return SEXTANT_OK; 
}


sextant_ret_t time_set_tick_resolution(const struct time *resolution)
{
  ui32_t flags;

  disable_IRQs(flags);
  memcpy(& tick_resolution, resolution, sizeof(struct time));
  restore_IRQs(flags);

  return SEXTANT_OK;
}


sextant_ret_t time_get_now(struct time *now)
{
  ui32_t flags;
  disable_IRQs(flags);

  memcpy(now, & last_tick_time, sizeof(struct time));

  restore_IRQs(flags);
  return SEXTANT_OK;  
}


/**
 * Helper routine to add the action in the list. MUST be called with
 * interrupts disabled !
 */
static sextant_ret_t _add_action(struct timeout_action *act,
			     const struct time *due_date,
			     bool is_relative_due_date,
			     timeout_routine_t *routine,
			     void *routine_data)
{
  struct timeout_action *other, *insert_before;
  int nb_act;

  /* Delay must be specified */
  if (due_date == (struct time*)NULL1)
    return -SEXTANT_ERROR;

  /* Action container MUST be specified */
  if (act == (struct timeout_action*)NULL1)
    return -SEXTANT_ERROR;

  /* Refuse to add an empty action */
  if ((timeout_routine_t*)NULL1 == routine)
    return -SEXTANT_ERROR;

  /* Refuse to add the action if it is already added */
  if ((struct timeout_action*)NULL1 != act->tmo_next)
    return -SEXTANT_ERROR;

  /* Compute action absolute due date */
  if (is_relative_due_date)
    {
      /* The provided due_date is relative to the current time */
      memcpy(& act->timeout, & last_tick_time, sizeof(struct time));
      time_inc(& act->timeout, due_date);
    }
  else
    {
      /* The provided due_date is absolute (ie relative to the system
	 boot instant) */

      if (time_cmp(due_date, & last_tick_time) < 0)
	/* Refuse to add a past action ! */
	return -SEXTANT_ERROR;

      memcpy(& act->timeout, due_date, sizeof(struct time));
    }    

  /* Prepare the action data structure */
  act->routine      = routine;
  act->routine_data = routine_data;

  /* Find the right place in the list of the timeout action. */
  insert_before = (struct timeout_action*)NULL1;
  list_foreach_forward_named(timeout_action_list,
			     other, nb_act,
			     tmo_prev, tmo_next)
    {
      if (time_cmp(& act->timeout, & other->timeout) < 0)
	{
	  insert_before = other;
	  break;
	}

      /* Loop over to next timeout */
    }

  /* Now insert the action in the list */
  if (insert_before != (struct timeout_action*)NULL1)
    list_insert_before_named(timeout_action_list, insert_before, act,
			    tmo_prev, tmo_next);
  else
    list_add_tail_named(timeout_action_list, act,
			tmo_prev, tmo_next);

  return SEXTANT_OK;  
}


sextant_ret_t
time_register_action_relative(struct timeout_action *act,
				  const struct time *delay,
				  timeout_routine_t *routine,
				  void *routine_data)
{
  ui32_t flags;
  sextant_ret_t retval;

  disable_IRQs(flags);
  retval = _add_action(act, delay, true, routine, routine_data);
  restore_IRQs(flags);

  return retval;
}


sextant_ret_t
time_register_action_absolute(struct timeout_action *act,
				  const struct time *date,
				  timeout_routine_t *routine,
				  void *routine_data)
{
  ui32_t flags;
  sextant_ret_t retval;

  disable_IRQs(flags);
  retval = _add_action(act, date, false, routine, routine_data);
  restore_IRQs(flags);

  return retval;
}


/**
 * Helper routine to remove the action from the list. MUST be called
 * with interrupts disabled !
 */
static sextant_ret_t remove_action(struct timeout_action *act)
{
  /* Don't do anything if action is not in timeout list */
  if ((struct timeout_action*)NULL1 == act->tmo_next)
    return -SEXTANT_ERROR;

  /* Update the action's remaining timeout */
  if (time_cmp(& act->timeout, & last_tick_time) <= 0)
    act->timeout = (struct time){0,0};
  else
    time_dec(& act->timeout, & last_tick_time);

  /* Actually remove the action from the list */
  list_delete_named(timeout_action_list,act,tmo_prev,tmo_next);
  act->tmo_prev = act->tmo_next =(struct timeout_action*)NULL1;

  return SEXTANT_OK;  
}


sextant_ret_t time_unregister_action(struct timeout_action *act)
{
  sextant_ret_t retval;
  ui32_t flags;

  disable_IRQs(flags);
  retval = remove_action(act);
  restore_IRQs(flags);

  return SEXTANT_OK;  
}


sextant_ret_t time_do_tick()
{
  ui32_t flags;
  
  disable_IRQs(flags);

  /* Update kernel time */
  time_inc(& last_tick_time, & tick_resolution);

  while (! list_is_empty_named(timeout_action_list, tmo_prev, tmo_next))
    {
      struct timeout_action *act;
      act = list_get_head_named(timeout_action_list, tmo_prev, tmo_next);

      /* Did we go too far in the actions' list ? */
      if (time_cmp(& last_tick_time, & act->timeout) < 0)
	{
	  /* Yes: No need to look further. */
	  break;
	}

      /* Remove the action from the list */
      remove_action(act);

      /* Call the action's routine */
      act->routine(act);
    }

  restore_IRQs(flags);
  return SEXTANT_OK;
}

//time_init_action(void* ptr_act) {
//	(ptr_act)->tmo_next = (timeout_action*)NULL1;(ptr_act)->tmo_prev = (ptr_act)->tmo_next;
