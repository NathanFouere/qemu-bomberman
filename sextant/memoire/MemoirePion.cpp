/*
 * MemoirePion.cpp
 *
 *  Created on: 23 oct. 2008
 *      Author: menaud
 */

#include "MemoirePion.h"
#include "op_memoire.h"

#define DEBUG_MEMOIREPION 0

void MemoirePion::memoireaffiche(Ecran *ec){
}


void MemoirePion::mem_setup(void * begin, int size,Ecran *ecran) {
	debut=begin;
	taille=size;
	ec=ecran;
}

vaddr_t MemoirePion::malloc(size_t nbytes)
{
	void *adresse;
	int *p;
	int i = (int) debut;
	int j;
	p=(int*)debut;

	i=i+nbytes;
	adresse = debut;
	debut = (char*)debut + nbytes;
	
	return (vaddr_t) adresse;

}


sextant_ret_t MemoirePion::freemem(vaddr_t addr, int taille) {
	memset((void *) addr,0,taille);
}

sextant_ret_t MemoirePion::free(vaddr_t addr) {
	int *p;
	p=(int*)(addr);
	p=p-1;
	memset((void*)p,0,*p+4);


	return SEXTANT_OK;
}

