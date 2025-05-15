#include "Player.h"
#include "../Board/Board.h"
#include "drivers/Clavier.h"
#include <Applications/Board/Bomb.h>
#include <sextant/Activite/ThreadManager.h>
#include <drivers/timer.h>

void Player::movePlayer() {
    // Don't process input if player is not active
    if (!active || status == EntityStatus::DEAD) {
        return;
    }

    // Test keyboard directly
    if (clavier->testChar()) {
        // Show we have input
        draw_text("INPUT", 200, 30, 14);
        
        char c = clavier->getchar();
        // Show the character
        draw_text(&c, 250, 30, 14);

        if (c == 'd') {
            direction = RIGHT;
            move(*board, 2, 0);
            animationFrame = (animationFrame + 1) % 3;
        }
        else if (c == 'q') {
            direction = LEFT;
            move(*board, -2, 0);
            animationFrame = (animationFrame + 1) % 3;
        }
        else if (c == 's') {
            direction = DOWN;
            move(*board, 0, 2);
            animationFrame = (animationFrame + 1) % 3;
        }
        else if (c == 'z') {
            direction = UP;
            move(*board, 0, -2);
            animationFrame = (animationFrame + 1) % 3;
        }
        else if(c == 'n') {
            this->poseBomb();
        }
    }
}

void Player::run() {
    while (true) {
        if (active) {
            movePlayer();
        }

        ThreadManager::getInstance().yieldThread();
        
        // Short wait to avoid consuming too much CPU
        unsigned long endTime = Timer::getInstance().getTicks() + 10;
        while (Timer::getInstance().getTicks() < endTime) {
            ThreadManager::getInstance().yieldThread();
        }
    }
}

void Player::render() {
    if (status != EntityStatus::DEAD) {
        draw_sprite(this->getSprite(), 16, 16, this->getX(), this->getY());
    }
}

void Player::poseBomb() {
    // Don't allow placing bombs if player is dead
    if (status == EntityStatus::DEAD) {
        return;
    }

    Bomb* bombe = new Bomb(board, x+8 ,y+8);
    board->setTileAt(x+8, y+8, bombe);
}


