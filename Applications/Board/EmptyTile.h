#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>

class EmptyTile : public Tile {
public:
    EmptyTile() {}

    TileType getType() const override {
        return TILE_EMPTY;
    }

    void render(int x, int y) override {
        // No rendering needed for empty tiles
    }
};