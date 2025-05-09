#pragma once

#include "../Utilities/Vector.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h>
#include "Tile.h"

class Board {
public:
    Board(int w, int h);
    ~Board();

    Tile* getTile(int x, int y) const {
        return layout[y][x];
    }

    void setTile(int x, int y, Tile* tile) {
        if (layout[y][x]) {
            delete layout[y][x];
        }
        layout[y][x] = tile;
    }

    void draw();

private:
    int width;
    int height;
    Tile*** layout;
};