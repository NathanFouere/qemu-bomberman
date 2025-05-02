/*
 * MemoirePion.cpp
 *
 *  Created on: 23 oct. 2008
 *      Author: menaud
 */

#include "MemoirePionAmelioree.h"
#include "op_memoire.h"

#define DEBUG_MEMOIREPIONAMELIOREE 1

void MemoirePionAmelioree::memoireaffiche(Ecran *ec){
}


void MemoirePionAmelioree::mem_setup(void * begin, int size,Ecran *ecran) {
	debut=begin;
	taille=size;
	ec=ecran;
}

vaddr_t MemoirePionAmelioree::malloc(size_t nbytes)
{
	void *adresse;
	int *p;
	int i = (int) debut;
	int j;
	p=(int*)debut;

	/* Question 4 */
	/* ??? */

	if(DEBUG_MEMOIREPIONAMELIOREE==1) {
		ec->sautDeLigne();
		ec->afficherMot("Adresse de debut : ");
		ec->afficherBase((unsigned int) j,10,BLANC);
		ec->sautDeLigne();
		ec->afficherMot("Taille passee en param de malloc : ");
		ec->afficherBase((unsigned int) nbytes,10,BLANC);
		ec->sautDeLigne();
		ec->afficherMot("Taille enregistree : ");
		ec->afficherBase((unsigned int) *p,10,BLANC);
		ec->sautDeLigne();
		ec->afficherMot("Al'adresse : ");
		ec->afficherBase((unsigned int) p,10,BLANC);
		ec->sautDeLigne();
		ec->afficherMot("Adresse de fin : ");
		ec->afficherBase((unsigned int) i,10,BLANC);
		ec->sautDeLigne();
	}

	/* ??? */

	return (vaddr_t) adresse;

}



sextant_ret_t MemoirePionAmelioree::free(vaddr_t addr) {
	int *p;
	p=(int*)(addr);
	p=p-1;

	/* Question 4 */
	/* ????? */
	
	if(DEBUG_MEMOIREPIONAMELIOREE==1) {
		ec->sautDeLigne();
		ec->afficherMot("Adresse de debut de free : ");
		ec->afficherBase((unsigned int) addr,10,BLANC);
		ec->sautDeLigne();
		ec->afficherMot("Taille retrouvee : ");
		ec->afficherBase((unsigned int) *p,10,BLANC);
		ec->sautDeLigne();
		ec->afficherMot("A l'adresse : ");
		ec->afficherBase((unsigned int) p,10,BLANC);
		ec->sautDeLigne();
	}

	return SEXTANT_OK;
}

