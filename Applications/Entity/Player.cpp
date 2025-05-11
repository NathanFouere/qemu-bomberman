#include "Player.h"
#include "../Board/Board.h"
#include "drivers/Clavier.h"
#include <Applications/Board/Bomb.h>


void Player::movePlayer() {
    if (clavier->testChar()) {
        char c = clavier->getchar();
        animationFrame++;
        if (c == 'd') {
            direction = RIGHT;
            move(*board, 1, 0);
        }
        if (c == 'q') {
            direction = LEFT;
            move(*board, -1, 0);
        }
        if (c == 's') {
            direction = DOWN;
            move(*board, 0, 1);
        }
        if (c == 'z') {
            direction = UP;
            move(*board, 0, -1);
        }
        if(c == 'n') {
            this->poseBomb();
        }
    }
}

void Player::run() {
    while (true) {
        movePlayer();
        animationFrame++;
        Yield();
    }
}

void Player::render() {
}

void Player::poseBomb() {
    Bomb* bombe = new Bomb(board, x ,y);
    this->board->setTileAt(x, y, bombe);
}


