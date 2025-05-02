/*
 * Semaphore.h
 *
 *  Created on: 3 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_


#include <sextant/ordonnancements/preemptif/sched.h>
#include <sextant/ordonnancements/preemptif/thread.h>
#include <sextant/Synchronisation/Spinlock/Spinlock.h>

/*
struct sem {
	int free;
	int cpt;
}; */

//void *allsem[100];



class Semaphore {
	static int sem;
	int sem_id;
	int value;
	static int lock;
	Spinlock mySpinlock;

//	Semaphore() {if (sem_id==0) for (int i=0;i<MAXSEM;i++) allsem[i].free=1; sem_id++;value=0;};
//	Semaphore(int v) {if (sem_id==0) for (int i=0;i<MAXSEM;i++) allsem[i].free=1; sem_id++;value=v;};
public:

	Semaphore() {sem++; sem_id = sem; value=0; /*allsem[sem_id]=this;*/};
	Semaphore(int v) {sem++; sem_id = sem; value=v;/*allsem[sem_id]=this;*/};

	void P();
	void V();
	int Valeur();

};


#endif /* SEMAPHORE_H_ */
