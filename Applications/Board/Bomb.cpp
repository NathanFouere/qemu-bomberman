#include "Bomb.h"

const unsigned char* Bomb::getSprite() {
    return sprites[this->animationFrame % this->nbSprites];
}

void Bomb::render() {
    this->animationFrame++;
}