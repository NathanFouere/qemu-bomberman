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
            if (exploded)
            {
                return TILE_BOMB_EXPLODED;
            }
            
            return TILE_BOMB_EXPLODING;
        }

        void render(int x, int y);
        void handleExplosion();
    };