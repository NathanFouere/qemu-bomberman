/*
 * Hello.cpp
 *
 *  Created on: 1 oct. 2008
 *      Author: Jean-Marc Menaud
 */
#include "InfiniteHello.h"

void InfiniteHello::init(){
//	initias((void *) this,e);
//	e=ecr;
}

InfiniteHello::InfiniteHello(Ecran* ecr,int i){
	e=ecr;
	monNum=i;
}

InfiniteHello::InfiniteHello(Ecran* ecr,int i,Semaphore *sm1,Semaphore *sm2,char *chaine){
 	e=ecr;
 	monNum=i;
 	sem1=sm1;
 	sem2=sm2;
 	str=chaine;
 }

void InfiniteHello::run(){
	char *temp;
	temp=str;
	while(1) {
	for ( ; *str != '\n' ; str++)
	    {
			e->afficherCaractere(BLANC,NOIR,*str);
	}
	str=temp;
	}
/*	for (int i=0;i<10;i++){
		sem1->P();
		e->afficherMot("Hello");
		e->afficherChiffre(monNum);
		e->afficherMot(" la sem : ");
		e->afficherChiffre(sem1->Valeur());
		sem2->V();
	}*/
}
