/*
 * Mutex.h
 *
 *  Created on: 6 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <sextant/Synchronisation/Semaphore/Semaphore.h>

class Mutex : public Semaphore {

struct thread *lActivite;

public :
	Mutex();
	void lock();
	sextant_ret_t unlock();
};
#endif /* MUTEX_H_ */
