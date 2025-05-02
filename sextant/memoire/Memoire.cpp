/*
 * Memoire.cpp
 *
 *  Created on: 23 oct. 2008
 *      Author: menaud
 */


#include "Memoire.h"
#include "MemoirePion.h"
#include "MemoireListe.h"
#include "MemoirePaginee.h"

#include "op_memoire.h"

MemoirePion uneMemoire;

memoire::memoire(){
	i=0;
}
void memoire::memoireaffiche(Ecran *ec){
	uneMemoire.memoireaffiche(ec);
}


void memoire::mem_setup(void * begin, int size,Ecran *ecran) {
	uneMemoire.mem_setup(begin, size, ecran);
}

vaddr_t memoire::malloc(size_t nbytes){
	return uneMemoire.malloc(nbytes);
}

void memoire::test(){
	return uneMemoire.test();
}
sextant_ret_t memoire::free(vaddr_t addr) {
	return uneMemoire.free(addr);
}

// Ce pattern singleton n'est pas guarantie pour le multi-threading lors de la creation de
// InterfaceMemoireIn. Cependant nous ne pouvons utiliser ici de new().
// C'est pour cette raison qu'il faut creer l'objet avant l'initialisation du multitaches.

memoire* memoire::nouveau(){
	static memoire InterfaceMemoireIn;
	return &InterfaceMemoireIn;
}


void* operator new(size_t taille) { return (void*) (*(memoire::nouveau())).malloc(taille);};

void  operator delete(void* ptr)  { (*(memoire::nouveau())).free((vaddr_t)ptr); return;};
