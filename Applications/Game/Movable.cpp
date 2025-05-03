#include "Movable.h"
#include <drivers/vga.h>

// Constructeur avec paramètres
Movable::Movable(int x, int y, unsigned char* data)
    : x(x), y(y), spriteData(data) {}

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

