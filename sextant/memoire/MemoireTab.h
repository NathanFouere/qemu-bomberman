/*
 * MemoireTab.h
 *
 *  Created on: 30 oct. 2008
 *      Author: menaud
 */

#ifndef MEMOIRETAB_H_
#define MEMOIRETAB_H_

#include "Memoire.h"

class MemoireTab : public memoire {
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

#endif /* MEMOIRETAB_H_ */
