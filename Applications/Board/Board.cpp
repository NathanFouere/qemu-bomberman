#include "Board.h"
#include "Tile.h"

Board::Board(int w, int h) : width(w), height(h) {
    layout = static_cast<Tile**>(alloc(height * sizeof(Tile*)));
    for (int y = 0; y < height; ++y) {
    layout[y] = static_cast<Tile*>(alloc(width * sizeof(Tile)));
      for (int x = 0; x < width; ++x) {
            layout[y][x].setType(TileType::Empty);
        }
    }

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if ((x == 0 || y == 0 || y == height - 1) || ((x % 2 == 0) && (y % 2 == 0))) {
                layout[y][x].setType(TileType::Wall);
            }
            else {
                layout[y][x].setType(TileType::Empty);
            }
        }
    }
}

Board::~Board() {
    for (int row = 0; row < height; ++row) {
        free(layout[row]);
    }
    free(layout);
}

void Board::initializeWalls() {
    
}

void Board::draw() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            layout[y][x].render(x, y);
        }
    }
}