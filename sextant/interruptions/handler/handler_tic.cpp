#include "handler_tic.h"

int a = 0;
/**
 * Permet de compter le nombre d'interruption de l'horloge.
 */
int compt = 0;
int secondes =0;

void ticTac(int o) {
	compt++;
	if(compt%1000==0) {
		secondes++;
		if(a == 9)
			a=0;
		else
			a++;
	}
}
