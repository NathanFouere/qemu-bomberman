#include "Player.h"
#include "../Board/Board.h"
#include "drivers/Clavier.h"
#include <Applications/Board/Bomb.h>
#include <drivers/timer.h>

void Player::movePlayer() {
    // Don't process input if player is not active
    if (!active || status == EntityStatus::DEAD) {
        return;
    }
    
    // Rate-limit movement to prevent too rapid movement
    unsigned long currentTime = Timer::getInstance().getTicks();
    if (currentTime - lastMoveTime < moveDelay) {
        return;
    }
    
    if (clavier->testChar()) {
        char c = clavier->getchar();
        
        if (c == 'd') {
            direction = RIGHT;
            move(*board, 3, 0);
            animationFrame = (animationFrame + 1) % 3;
            lastMoveTime = currentTime;
        }
        else if (c == 'q') {
            direction = LEFT;
            move(*board, -3, 0);
            animationFrame = (animationFrame + 1) % 3;
            lastMoveTime = currentTime;
        }
        else if (c == 's') {
            direction = DOWN;
            move(*board, 0, 3);
            animationFrame = (animationFrame + 1) % 3;
            lastMoveTime = currentTime;
        }
        else if (c == 'z') {
            direction = UP;
            move(*board, 0, -3);
            animationFrame = (animationFrame + 1) % 3;
            lastMoveTime = currentTime;
        }
        else if(c == 'n') {
            this->poseBomb();
            lastMoveTime = currentTime;
        }
    }
}

void Player::run() {
    while (true) {
        if (active) {
            movePlayer();
        }
        
        // Sleep for a short time to avoid consuming too much CPU
        sleep(10);
        
        // Yield to other threads
        Yield();
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
    
    Bomb* bombe = new Bomb(board, x, y);
}


