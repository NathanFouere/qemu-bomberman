#ifndef CLAVIER_H
#define CLAVIER_H

#include "../drivers/Ecran.h"
#include "../hal/fonctionsES.h"
/**
 * @file Clavier.h
 * @class Clavier
 * @brief Permet la récupération des caractères saisis au clavier.
 */

class Clavier {
public :
	/***
	 * Recupere le premier caractere tape.
	 ***/
	char getchar();
	/***
	 * Recupere tous les caracteres tapes.
	 ***/
	char* getString();
	/***
	 * Verifie s'il y a au moins un caractere qui a ete tape.
	 ***/
	bool testChar();

	void set_leds(void);

};

#endif
