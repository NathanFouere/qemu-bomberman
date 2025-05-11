#pragma once
#include "Applications/Utilities/Vector.h"
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

    void setTileAt(int px, int py, Tile* tile);

    bool isBlockedAt(int px, int py) const;

    void draw();

private:
    int width;
    int height;
    Tile*** layout;
};