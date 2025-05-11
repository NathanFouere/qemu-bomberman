#include "Bot.h"
#include <drivers/vga.h>

void Bot::moveRandom() {
    switch (it % 4) {
        case 0:
            direction = RIGHT;
            move(*board, 1, 0);
            break;
        case 1:
            direction = LEFT;
            move(*board, -1, 0);
            break;
        case 2:
            direction = DOWN;
            move(*board, 0, 1);
            break;
        case 3:
            direction = UP;
            move(*board, 0, -1);
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