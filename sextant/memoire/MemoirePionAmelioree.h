/*
 * MemoirePion.h
 *
 *  Created on: 23 oct. 2008
 *      Author: menaud
 */

#ifndef MEMOIREPIONAMELIOREE_H_
#define MEMOIREPIONAMELIOREE_H_
#include "Memoire.h"


class MemoirePionAmelioree : public memoire {
private:
	void *debut;
	int taille;
	Ecran *ec;
public:
	 void mem_setup(void *begin, int size,Ecran *ec);
	 vaddr_t malloc(size_t taille);
	 sextant_ret_t free(vaddr_t addr);
	 void memoireaffiche(Ecran *ec);
};



#endif /* MEMOIREPION_H_ */
