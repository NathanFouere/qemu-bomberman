#include "Movable.h"
#include <drivers/vga.h>

const unsigned char* Movable::getSprite() {
    return sprites[direction][animationFrame % 3];
}

void Movable::move(int dx, int dy) {
    x += dx;
    y += dy;
}

