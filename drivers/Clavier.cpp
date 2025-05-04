#include "Clavier.h"

extern bool modifBuf;
extern char buf[256];
extern int posBuf;

bool Clavier::isPressed(char c){
	if (modifBuf) {
		for (int i = 0; i < posBuf; i++) {
			if (buf[i] == c) {
				return true;
			}
		}
	}
	return modifBuf;
}

char Clavier::getchar(){
	while(!modifBuf);
	modifBuf = false;
	posBuf = 0;
	return buf[0];
}

char* Clavier::getString(){
	while(!modifBuf);
	modifBuf = false;
	posBuf = 0;
	return buf;
}

void Clavier::set_leds(void) {
	ecrireOctet(0x60, 0xED);
}
