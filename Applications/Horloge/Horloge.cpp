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
	int temps_secondes=0;
	int minutes=0;
	int temp=0;

	temp=t->getSecondes();

	temps_secondes  = temp % 60;

	minutes = temp / 60  ;


	e->afficherMot(0,39,"::",BLANC);

	if (minutes<10) {e->afficherChiffre(0,37,0);e->afficherChiffre(0,38,minutes);}
	else e->afficherChiffre(0,37,minutes);

	if (temps_secondes<10) {e->afficherChiffre(0,41,0);e->afficherChiffre(0,42,temps_secondes);}
	else e->afficherChiffre(0,41,temps_secondes);
}
