#include "Player.h"
#include "../Board/Board.h"
#include "drivers/Clavier.h"
#include <Applications/Board/Bomb.h>
#include <sextant/Activite/ThreadManager.h>
#include <drivers/timer.h>

void Player::movePlayer() {
    // Don't process input if player is not active, is dead or is in death animation
    if (!active || status == EntityStatus::DEAD || status == EntityStatus::DEAD_ANIMATION) {
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




void Player::handleHitBomb() {
    this->decreaseLives();
    
    // Always trigger death animation, whether player has lives left or not
    status = EntityStatus::DEAD_ANIMATION;
    
    // If player has no lives left, it will become DEAD after animation completes
    animationFrame = 0;
    deathAnimationStartTime = Timer::getInstance().getSeconds();
    lastAnimationTime = Timer::getInstance().getTicks(); // Initialize the animation timer
}

void Player::update() {
    if (status == EntityStatus::DEAD_ANIMATION) {
        // Get current time in milliseconds
        unsigned long currentTime = Timer::getInstance().getTicks();
        
        // Increment animation frame every 550ms using the instance variable
        if (currentTime - lastAnimationTime >= animationFrameInterval) {
            animationFrame++;
            lastAnimationTime = currentTime;
        }
        
        if (Timer::getInstance().getSeconds() - deathAnimationStartTime > DEATH_ANIMATION_TIME) {
            status = EntityStatus::ALIVE;
            // Set invulnerability period after respawn
            invulnerabilityEndTime = Timer::getInstance().getTicks() + INVULNERABILITY_DURATION;
        }
    }
}
