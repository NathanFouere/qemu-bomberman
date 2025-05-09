#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>

class Bomb : public Tile {
    public:
        Bomb() : Tile() {}

        TileType getType() const override {
            return TILE_BOMB;
        }

        void render(int x, int y) override {
            // draw_sprite(sprites_for_bomb, 16, 16, x * 16 - 8, y * 16 + 24);
        }
    };