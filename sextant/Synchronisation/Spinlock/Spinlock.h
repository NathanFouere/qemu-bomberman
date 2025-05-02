/*
 * Spinlock.h
 *
 *  Created on: 3 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include <sextant/interruptions/i8259.h>

extern "C" {
	int TestAndSet(int *lock);
}

class Spinlock {
public:
	void Take(int *lck);
	void Release(int *lck);
};

#endif /* SPINLOCK_H_ */
