#pragma once
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h>
#include "TileType.h"

static constexpr int TILE_SIZE = 16;
static constexpr int BOARD_ORIGIN_X = -8;
static constexpr int BOARD_ORIGIN_Y = 24; 

class Tile {
public:
    virtual ~Tile() {}
    virtual TileType getType() const = 0; 
    virtual void render(int x, int y) = 0;
    const unsigned char* sprites[1]; 
};