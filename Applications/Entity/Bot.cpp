#include "Bot.h"
#include <drivers/vga.h>
#include <drivers/timer.h>

void Bot::moveRandom() {
    // Don't move if bot is dead
    if (status == EntityStatus::DEAD) {
        return;
    }

    // Use instance variables instead of static ones to ensure each bot moves independently
    unsigned long currentTime = Timer::getInstance().getTicks();
    
    // Rate-limit movement for smoother gameplay
    if (currentTime - lastMoveTime < moveDelay) {
        return;
    }
    
    lastMoveTime = currentTime;
    
    // Change direction periodically or if we're starting
    if (currentTime - lastChangeTime >= directionChangeInterval || directionIndex == -1) {
        // Add entropy based on bot's position for unique behavior
        directionIndex = (currentTime + x + y) % 4;
        lastChangeTime = currentTime;
    }

    int dx = 0, dy = 0;
    switch (directionIndex) {
        case 0: dx = 2; dy = 0; direction = RIGHT; break;
        case 1: dx = -2; dy = 0; direction = LEFT; break;
        case 2: dx = 0; dy = 2; direction = DOWN; break;
        case 3: dx = 0; dy = -2; direction = UP; break;
    }

    move(*board, dx, dy);
    animationFrame = (animationFrame + 1) % 3;
}

void Bot::render(){
    if (status != EntityStatus::DEAD) {
        draw_sprite(this->getSprite(), 16, 16, this->getX(), this->getY());
    }
}

void Bot::run() {   
    while (true) {
        // Move the bot
        if (!(EntityStatus::DEAD_ANIMATION == status)) {
            moveRandom();
        }

        
        // Sleep for a short time to avoid too much CPU usage
        // Using different sleep values for different bot types to avoid synchronized movement
        unsigned int sleepTime = 30 + (static_cast<int>(type) * 5);
        sleep(sleepTime);
        
        // Yield to other threads multiple times to ensure fair scheduling
        for (int i = 0; i < 3; i++) {
            Yield();
        }
    }
}

void Bot::handleHitBomb() {
    status = EntityStatus::DEAD_ANIMATION;
    animationFrame = 0;
    deathAnimationStartTime = Timer::getInstance().getSeconds();
    lastAnimationTime = Timer::getInstance().getTicks(); // Initialize the animation timer
}


void Bot::update() {
    if (status == EntityStatus::DEAD_ANIMATION) {
        // Get current time in milliseconds
        unsigned long currentTime = Timer::getInstance().getTicks();
        
        // Increment animation frame every 500ms using the instance variable
        if (currentTime - lastAnimationTime >= animationFrameInterval) {
            animationFrame++;
            lastAnimationTime = currentTime;
        }
        
        if (Timer::getInstance().getSeconds() - deathAnimationStartTime > DEATH_ANIMATION_TIME) {
            status = EntityStatus::DEAD;
        }
    }
}