/*
 * Pipeline.cpp
 *
 *  Created on: 21 oct. 2008
 *      Author: menaud
 */

#include "Pipeline.h"
extern int globalCounter;
extern int positionCounter;


 Pipeline::Pipeline(Ecran* ecr,int i,Semaphore *sm1,Semaphore *sm2){
 	e=ecr;
 	monNum=i;
 	sem1=sm1;
 	sem2=sm2;
 }

 void Pipeline::Init(Ecran* ecr,int i,Semaphore *sm1,Semaphore *sm2){
 	e=ecr;
 	monNum=i;
 	sem1=sm1;
 	sem2=sm2;
 }

void Pipeline::run(){

	int position = 7-positionCounter;
	sem1->P();

	e->afficherCaractere(0,position, BLANC,NOIR,(const char)((globalCounter%2)+48));
	globalCounter= globalCounter >> 1;
	positionCounter++;
//	e->afficherMot(positionCounter,1,"Le calcul ");
	e->afficherMot("Le calcul ");
	e->afficherChiffre(positionCounter);
	e->afficherMot(" a ete realise par l'activite ");
	e->afficherChiffre(monNum);
	e->afficherMot("\n");
	sem2->V();


	}

