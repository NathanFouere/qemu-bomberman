#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <drivers/timer.h>

class Bomb : public Tile {
    private:
        int apparitionSecond;
        int animationFrame;
        unsigned char* explosionSprites[3];
    public:
        Bomb();

        TileType getType() const override {
            return TILE_BOMB;
        }

        void render(int x, int y);
    };