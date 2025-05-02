/*
 * Mutex.cpp
 *
 *  Created on: 6 oct. 2008
 *      Author: Jean-Marc Menaud
 */

#include "Mutex.h"


Mutex::Mutex(){
	Semaphore(1);
};

void Mutex::lock(){
	lActivite = thread_get_current();
	P();
};

sextant_ret_t Mutex::unlock(){
	if (lActivite == thread_get_current()) {
		V();
		return SEXTANT_OK;
	}
	else return SEXTANT_ERROR;
};
