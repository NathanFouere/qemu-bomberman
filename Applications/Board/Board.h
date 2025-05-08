//// filepath: /workspaces/qemu-bomberman/Applications/Board/Board.h
#pragma once
#include "../Vector.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>

enum class TileType {
    Empty,
    Wall,
    Brick,
    Bomb
};

class Board {
public:
    Board(int w, int h) : size{static_cast<float>(w), static_cast<float>(h)}, width(w), height(h) {
        layout = new TileType*[height];
        for (int row = 0; row < height; ++row) {
            layout[row] = new TileType[width];
            for (int col = 0; col < width; ++col) {
                layout[row][col] = TileType::Empty;
            }
        }
        initializeWalls();
    }

    ~Board() {
        for (int row = 0; row < height; ++row) {
            delete[] layout[row];
        }
        delete[] layout;
    }

    TileType getTile(int x, int y) const {
        return layout[y][x];
    }

    void setTile(int x, int y, TileType type) {
        layout[y][x] = type;
    }

    void draw();

    void initializeWalls();

private:
    Vector size;
    int width;
    int height;
    TileType** layout;
};