#pragma once
#include "../Utilities/Vector.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <sextant/memoire/Memoire.h> // Include your memory manager

enum class TileType {
    Empty,
    Wall,
    Brick,
    Bomb
};

class Board {
public:
    Board(int w, int h);
    ~Board();

    TileType getTile(int x, int y) const {
        return layout[y][x];
    }

    void setTile(int x, int y, TileType type) {
        layout[y][x] = type;
    }

    void draw();
    
    private:
    int width;
    int height;
    TileType** layout;
    
    void initializeWalls();
};