#include "Bot.h"
#include <drivers/vga.h>

void Bot::moveRandom() {
    switch (it % 4) {
        case 0:
            move(0, 10);
            break;
        case 1:
            move(-10, 0);
            break;
        case 2:
            move(10, 0);
            break;
        case 3:
            move(0, -10);
            break;
    }
    it++;
}

void Bot::run() {
    while (true) {
        moveRandom();
        Yield();
    }
}