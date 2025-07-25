#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <drivers/timer.h>
#include "Board.h"
#include "Explosion.h"
#include "../Utilities/AnimationTimer.h"

class Bomb : public Tile {
    private:
        int apparitionSecond;
        int animationFrame;
        unsigned char* explosionSprites[11];
        Board* board;
        bool exploded;
        int x;
        int y;
        int power = 1;
        AnimationTimer animTimer;

        void processExplosionDirection(int dx, int dy, ExplosionState midState, ExplosionState endState);
    public:
        Bomb(Board* board, int x, int y);

        TileType getType() const override {
            return TILE_BOMB;
        }

        void render(int x, int y);
        void handleExplosion();
    };