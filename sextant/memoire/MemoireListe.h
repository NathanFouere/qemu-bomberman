/*
 * MemoireListe.h
 *
 *  Created on: 24 oct. 2008
 *      Author: menaud
 */

#ifndef MEMOIRELISTE_H_
#define MEMOIRELISTE_H_


class MemoireListe : public memoire {
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


#endif /* MEMOIRELISTE_H_ */
