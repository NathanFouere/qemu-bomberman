#pragma once
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h>
#include "TileType.h"

const int MAX_SPRITES = 10;
class Tile {
private:
    TileType type;
public:
    Tile(TileType type = TileType::Empty);
    Tile();
    TileType getType() const { return type; }
    void setType(TileType newType) { type = newType; }
    unsigned char* sprites[MAX_SPRITES];
    int nbSprites;
    void render(int x, int y) {
        // draw_sprite(sprites, 16, 16, x * 16 - 8, y * 16 + 24);
    }
};