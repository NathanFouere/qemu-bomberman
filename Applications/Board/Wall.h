#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>

class Wall : public Tile {
public:
    Wall() : Tile() {
        sprites[0] = wall_1; 
    }

    TileType getType() const override {
        return TILE_WALL;
    }

    void render(int x, int y) override {
        const int px = BOARD_ORIGIN_X + x * TILE_SIZE;
        const int py = BOARD_ORIGIN_Y + y * TILE_SIZE;
        draw_sprite(sprites[0], TILE_SIZE, TILE_SIZE, px, py);
    }
};