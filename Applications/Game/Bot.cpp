#include "Bot.h"
#include <drivers/vga.h>

void Bot::moveRandom() {
    x=x+1;
    y=y+1;
    move(x, y);
}

void Bot::run() {
    while (true) {
        moveRandom();
        Yield();
    }
}