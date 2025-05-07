#include "Horloge.h"

/**
 * Variable déclarée dans le fichier tic.cpp servant à compter les tics de l'horloge.
 */

//JMMM R75

Horloge::Horloge(Ecran* ecr,Timer *timer) {
	e=ecr;
	t=timer;
}

void Horloge::afficheHeure() {
	int totalSecondes = t->getSeconds();

	int minutes = totalSecondes / 60;
	int secondes = totalSecondes % 60;

	char buffer[6];

	buffer[0] = '0' + (minutes / 10);
	buffer[1] = '0' + (minutes % 10);
	buffer[2] = ':';
	buffer[3] = '0' + (secondes / 10);
	buffer[4] = '0' + (secondes % 10);
	buffer[5] = '\0';

	e->afficherMot(0, (COLONNES-3)/2 , buffer);
}