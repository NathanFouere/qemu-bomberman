/*
 * Memoire.h
 *
 *  Created on: 23 oct. 2008
 *      Author: menaud
 */

#ifndef MEMOIRE_H_
#define MEMOIRE_H_

#include <drivers/Ecran.h>
#include <sextant/types.h>

class memoire {

protected :
	memoire();

public:
	int i; // pour test

	virtual void mem_setup(void *begin, int size,Ecran *ec);
	virtual vaddr_t malloc(size_t taille) ;
	virtual sextant_ret_t free(vaddr_t addr) ;
	virtual void memoireaffiche(Ecran *ec);
	virtual void test();
	static memoire* nouveau();
};


void* operator new(size_t taille);
void  operator delete(void* ptr);

#endif /* MEMOIRE_H_ */
