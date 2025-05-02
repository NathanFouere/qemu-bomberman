/*
 * Hello.cpp
 *
 *  Created on: 1 oct. 2008
 *      Author: Jean-Marc Menaud
 */
#include "Hello.h"

void Hello::init(){
//	initias((void *) this,e);
//	e=ecr;
}

 Hello::Hello(Ecran* ecr,int i){
	e=ecr;
	monNum=i;
}

 Hello::Hello(Ecran* ecr,int i,Semaphore *sm1,Semaphore *sm2,char *chaine){
 	e=ecr;
 	monNum=i;
 	sem1=sm1;
 	sem2=sm2;
 	str=chaine;
 }

void Hello::run(){

	for ( ; *str != '\n' ; str++)
	    {
			sem1->P();
			e->afficherCaractere(BLANC,NOIR,*str);
			sem2->V();
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
