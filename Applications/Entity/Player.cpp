#include "Player.h"
#include "drivers/Clavier.h"


void Player::movePlayer() {
    if (clavier->testChar()) {
        animationFrame++;
        char c = clavier->getchar();
        if (c == 'd') {
            direction = RIGHT;
            move(1, 0);
        }
        if (c == 'q') {
            direction = LEFT;
            move(-1, 0);
        }
        if (c == 's') {
            direction = DOWN;
            move(0, 1);
        }
        if (c == 'z') {
            direction = UP;
            move(0, -1);
        }
    }
}

void Player::run() {
    while (true) {
        movePlayer();
        Yield();
    }
}

void Player::render() {
}


