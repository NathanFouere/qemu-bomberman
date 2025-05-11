#include "Player.h"
#include "../Board/Board.h"
#include "drivers/Clavier.h"


void Player::movePlayer() {
    if (clavier->testChar()) {
        animationFrame++;
        char c = clavier->getchar();
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
        if (c == 'n')
        {
            //this->board->setTile(this->x, this->y, *this->dropBomb());
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

Bomb* Player::dropBomb() {
    return new Bomb(this->x, this->y);
}


