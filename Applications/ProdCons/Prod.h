/*
 * Prod.h
 *
 *  Created on: 6 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef PROD_H_
#define PROD_H_

#include <sextant/Activite/Threads.h>
#include <sextant/Synchronisation/Semaphore/Semaphore.h>


class Prod : public Threads {
int n;
char *tableauProCons;
Semaphore *sema;
Ecran *ecran;

public:
	Prod(Semaphore *sem,char *tab,Ecran *ec);
	void run();
};
#endif /* PROD_H_ */
