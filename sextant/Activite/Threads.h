/*
 * Thread.h
 *
 *  Created on: 1 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef THREAD_H_
#define THREAD_H_
#define THREAD_STACK_SIZE 1024

#include <sextant/ordonnancements/preemptif/thread.h>
#include <sextant/types.h>

class Threads {
	struct thread *maThread;
	int pid;

protected:

	void Yield(){
		 thread_yield();
	 };

	 void Exit(){
		 thread_exit();
	 };

public :

	void start(){
		maThread = create_kernel_thread((kernel_thread_start_routine_t) Threads::startme, this);
		pid = maThread->pid;
	};

	static void *startme(void *thread) {
        ((Threads*)thread)->run();
        ((Threads*)thread)->Exit();
        return NULL;
	};

	 virtual void run(){};

};

#endif /* THREAD_H_ */
