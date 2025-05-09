#pragma once
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h>
#include "TileType.h"

class Tile {
private:
    TileType type;
public:
    Tile(TileType type = TileType::Empty);
    TileType getType() const { return type; }
    void setType(TileType newType) { type = newType; }
    const unsigned char* sprites;
    void render(int x, int y) {
        // draw_sprite(sprites, 16, 16, x * 16 - 8, y * 16 + 24);
    }
};