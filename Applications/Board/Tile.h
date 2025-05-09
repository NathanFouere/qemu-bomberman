#pragma once
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h>
#include "TileType.h"

class Tile {
public:
    virtual ~Tile() {}
    virtual TileType getType() const = 0; 
    virtual void render(int x, int y) = 0;
    const unsigned char* sprites[1]; 
};