#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>


class Brick : public Tile {
public:
    Brick() : Tile() {
        sprites[0] = wall_2;
    }

    TileType getType() const override {
        return TILE_BRICK;
    }

    void render(int x, int y) override {
        draw_sprite(sprites[0], 16, 16, x * 16 - 8, y * 16 + 24);
    }
};