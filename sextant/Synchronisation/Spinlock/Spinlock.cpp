/*
 * Spinlock.cpp
 *
 *  Created on: 3 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#include "Spinlock.h"

void Spinlock::Take(int *lck){

	for (int i=0;i<16;i++)
		if (i!=2)  i8259_disable_irq_line(i);

	//while((*lck)<1){};

	while ( (TestAndSet(lck) )!=0);

//	--(*lck);
}


void Spinlock::Release(int *lck){
	--(*lck);
	for (int i=0;i<16;i++)
		if (i!=2) i8259_enable_irq_line(i);
}
