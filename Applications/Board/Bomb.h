#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <drivers/timer.h>
#include "Board.h"

class Bomb : public Tile {
    private:
        int apparitionSecond;
        int animationFrame;
        unsigned char* explosionSprites[11];
        Board* board;
        bool exploded;
        int x;
        int y;
    public:
        Bomb(Board* board, int x, int y);

        TileType getType() const override {
            return TILE_BOMB;
        }

        void render(int x, int y);
        void handleExplosion();
    };