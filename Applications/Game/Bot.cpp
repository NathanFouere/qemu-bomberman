#include "Bot.h"
#include <drivers/vga.h>

void Bot::moveRandom() {
    move(x, y);
    
}

void Bot::run() {
    while (true) {
        moveRandom();
        Yield();
    }
}