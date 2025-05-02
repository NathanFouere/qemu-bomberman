#include "Movable.h"
#include "drivers/sprite.h" // Pour sprite_data etc.

// Constructeur par défaut
Movable::Movable()
    : x(0), y(0), spriteData(sprite_data) // sprite_data est défini dans sprite.h
{}

// Constructeur avec paramètres
Movable::Movable(int x, int y, unsigned char* data)
    : x(x), y(y)
{
    if (data)
        spriteData = data;
}

void Movable::setSprite(unsigned char* data) {
    spriteData = data;
}

unsigned char* Movable::getSprite() const {
    return spriteData;
}

void Movable::move(int dx, int dy) {
    x += dx;
    y += dy;
}

