/*
 * Semaphore.cpp
 *
 *  Created on: 3 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#include "Semaphore.h"

int Semaphore::sem = 0;
int Semaphore::lock = 0;

void Semaphore::P(){
	mySpinlock.Take(&lock);
	value=value-1;
	mySpinlock.Release(&lock);

	if (value<0) {
		sched_set_waiting(thread_get_current(),sem_id);
		thread_wait();
	}

};

int Semaphore::Valeur(){
	return value;
}

void Semaphore::V(){
	if (value<0) {
		mySpinlock.Take(&lock);
		value=value+1;
		mySpinlock.Release(&lock);

		sched_set_ready(pop_in_waiting_queue(sem_id));
	}
	else value=value+1;
};


