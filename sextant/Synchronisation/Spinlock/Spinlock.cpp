/*
 * Spinlock.cpp
 *
 *  Created on: 3 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#include "Spinlock.h"

void Spinlock::Take(int *lck) {
    // Save flags and disable interrupts
    asm volatile("cli");
    
    while (TestAndSet(lck) != 0) {
        // If lock is held, briefly enable interrupts to prevent deadlock
        asm volatile("sti; nop; cli");
    }
}

void Spinlock::Release(int *lck) {
    *lck = 1;  // Release the lock
    
    // Re-enable interrupts
    asm volatile("sti");
}
