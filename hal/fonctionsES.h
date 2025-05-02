#ifndef SEXTANT_PROCESSEUR_BUS_PORTS_TP1_FONCTIONSES_H
#define SEXTANT_PROCESSEUR_BUS_PORTS_TP1_FONCTIONSES_H

/**
 * Fonctions d'accés au bus d'entrées-sorties
 */



#include <sextant/types.h>


// write value (word) on port
void ecrireMot(ui16_t value,int port);

// read one word from port
ui16_t lireMot(int port);

/**
 * Fonction permettant d'écrire un octet (caractère non signé) sur un port d'entrées/sorties
 */
void ecrireOctet(unsigned char value, int port);
unsigned char lireOctet(int port);

#endif
