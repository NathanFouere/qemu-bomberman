/*
 * Cons.h
 *
 *  Created on: 6 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef CONS_H_
#define CONS_H_

#include <sextant/Activite/Threads.h>
#include <sextant/Synchronisation/Semaphore/Semaphore.h>


class Cons : public Threads {

	char *tableauProCons;
	Semaphore *sema;
	Ecran *ecran;
public:
	Cons(Semaphore *sem,char *tab,Ecran *ec);
	void run();
};
#endif /* CONS_H_ */
