/*
 * Cons.cpp
 *
 *  Created on: 6 oct. 2008
 *      Author: Jean-Marc Menaud
 */
#include "Cons.h"

Cons::Cons(Semaphore *sem,char *tab,Ecran *ec){
	sema=sem;
	tableauProCons=tab;
	ecran=ec;
};
void Cons::run(){
	int i=0;
	char c;
	while (true) {
		sema->P();
		c=tableauProCons[i++];
		ecran->afficherCaractere(10,(i-1),BLANC,NOIR,c);
	}
};
