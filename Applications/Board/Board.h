#pragma once
#include "Applications/Utilities/Vector.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h>
#include <Applications/Board/Tile.h>

class Board {
public:
    Board(int w, int h);
    ~Board();

    Tile getTile(int x, int y) const {
        return layout[y][x];
    }

    void setTile(int x, int y, Tile type) {
        layout[y][x] = type;
    }

    void draw();

private:
    int width;
    int height;
    Tile** layout;

    void initializeWalls();
};