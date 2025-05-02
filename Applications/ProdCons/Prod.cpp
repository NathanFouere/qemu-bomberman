/*
 * Prod.cpp
 *
 *  Created on: 6 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#include "Prod.h"

Prod::Prod(Semaphore *sem,char *tab,Ecran *ec){
	sema=sem;
	tableauProCons=tab;
	ecran=ec;
};

void Prod::run(){
	int n=0;
	sema->V();
	for (int i=0;i<70;i++) {
		sema->V();
		tableauProCons[i]='$';
		ecran->afficherCaractere(9,i,BLANC,NOIR,tableauProCons[i]);
	}
};
