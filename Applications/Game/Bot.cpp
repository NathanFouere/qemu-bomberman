#include "Bot.h"
#include <drivers/vga.h>

void Bot::moveRandomly() {    
    static int step = 0;
    int dir = step % 4;
    step++;

    switch (dir) {
        case 0: move(1, 0); break;   // droite
        case 1: move(-1, 0); break;  // gauche
        case 2: move(0, 1); break;   // bas
        case 3: move(0, -1); break;  // haut
    }
    
}