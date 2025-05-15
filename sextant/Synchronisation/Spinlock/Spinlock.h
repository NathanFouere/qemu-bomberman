/*
 * Spinlock.h
 *
 *  Created on: 3 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include <sextant/interruptions/i8259.h>

class Spinlock {
public:
    int TestAndSet(int *ptr) {
        int old = *ptr;
        *ptr = 0;
        return old;
    }
    
    void Take(int *lck);
    void Release(int *lck);
};

#endif /* SPINLOCK_H_ */
