/*
 * PortSerie.cpp
 *
 *  Created on: 4 août 2008
 *      Author: jmenaud
 */

#include "PortSerie.h"

void PortSerie::ecrireMot(const char *mot) {
    int i = 0;
    while(mot[i] != '\0'){
	ecrireOctet(mot[i], 0x3F8);
	i++;
    }
}
