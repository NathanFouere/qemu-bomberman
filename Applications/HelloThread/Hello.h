/*
 * Hello.h
 *
 *  Created on: 1 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#ifndef HELLO_H_
#define HELLO_H_

#include <sextant/Activite/Threads.h>
#include <sextant/Synchronisation/Semaphore/Semaphore.h>

class Hello : public Threads  {
	Ecran *e;
	int monNum;
	Semaphore *sem1;
	Semaphore *sem2;
	char *str;

public :
	/**
	 * @brief horloge comptant d'après les interruptions générerées par l'horloge.
	 * @param e écran pour l'affichage
	 * @param c permet d'utiliser le clavier pour arrêter l'horloge.
	 */
//	Hello(){};
//	~Hello();
	void init();
	Hello(Ecran *,int);
	Hello(Ecran *,int,Semaphore *);
	Hello(Ecran *,int,Semaphore *,Semaphore *);
	Hello(Ecran *,int,Semaphore *,Semaphore *,char *);

	void run();

};
#endif /* HELLO_H_ */
