#pragma once
#include "../Utilities/Vector.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h>

enum class TileType {
    Empty,
    Wall,
    Brick,
    Bomb
};

class Tile {
public:
    Tile(TileType type) {}
    const unsigned char* sprites;
    void render(int x, int y) {
        draw_sprite(sprites, 16, 16, x * 16 - 8, y * 16 + 24);
    }
};