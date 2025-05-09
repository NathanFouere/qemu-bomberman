#pragma once
#include <drivers/sprite.h>
#include <Applications/Board/Tile.h>
#include "TileType.h"

class Bomb: public Tile {
public:
    int x;
    int y;
    int animationFrame;
    Bomb(int x, int y) : Tile(TileType::Bomb) {
        this->y = y;
        this->x = x;

        this->sprites[0] = bomb_1;
        this->sprites[1] = bomb_2;
        this->sprites[2] = bomb3;
        this->nbSprites = 3;
    }
    const unsigned char* getSprite();
    void render();
};
