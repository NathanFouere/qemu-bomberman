#pragma once
#include "../Utilities/Vector.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h> // Include your memory manager

class Board {
public:
    Board(int w, int h);
    ~Board();

    Tile getTile(int x, int y) const {
        return layout[y][x];
    }

    void setTile(int x, int y, TileType type) {
        layout[y][x] = type;
    }

    void draw();
    
    private:
    int width;
    int height;
    Tile** layout;
    
    void initializeWalls();
};