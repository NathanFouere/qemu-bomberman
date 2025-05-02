/*
 * Pipeline.h
 *
 *  Created on: 21 oct. 2008
 *      Author: menaud
 */

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <sextant/Activite/Threads.h>
#include <sextant/Synchronisation/Semaphore/Semaphore.h>

class Pipeline : public Threads  {
	Ecran *e;
	int monNum;
	Semaphore *sem1;
	Semaphore *sem2;

public :
	Pipeline(){};
	~Pipeline();

	Pipeline(Ecran *,int,Semaphore *,Semaphore *);
	void Init(Ecran *,int,Semaphore *,Semaphore *);

	void run();

};

#endif /* PIPELINE_H_ */
