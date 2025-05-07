#include "handler_tic.h"

int a = 0;
/**
 * Permet de compter le nombre d'interruption de l'horloge.
 */
int ticks = 0;
int seconds =0;

void ticTac(int o) {
	ticks++;
	if(ticks%1000==0) {
		seconds++;
		if(a == 9)
			a=0;
		else
			a++;
	}
}
