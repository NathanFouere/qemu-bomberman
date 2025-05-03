#include "Player.h"
#include "drivers/Clavier.h"


void Player::movePlayer() {
    if (clavier->testChar()) {
        char c = clavier->getchar();
        if (c == 'd') {
            move(1, 0);
        }
        if (c == 'q') {
            move(-1, 0);
        }
        if (c == 's') {
            move(0, 1);
        }
        if (c == 'z') {
            move(0, -1);
        }
    }
}