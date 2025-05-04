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
	 * Verifie si un caractere a ete tape.
	 ***/
	bool isPressed(char c);

	void set_leds(void);

	enum Key {
		Up = 'w',
		Down = 's',
		Left = 'a',
		Right = 'd',
		PlaceBomb = 'x',
		Enter = 0x1C
	};
	
};

#endif
