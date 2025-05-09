#include "Board.h"

Board::Board(int w, int h) : width(w), height(h) {
    layout = static_cast<TileType**>(alloc(height * sizeof(TileType*)));
    for (int y = 0; y < height; ++y) {
    layout[y] = static_cast<TileType*>(alloc(width * sizeof(TileType)));
      for (int x = 0; x < width; ++x) {
            layout[y][x] = TileType::Empty;
        }
    }

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if ((x == 0 || y == 0 || y == height - 1) || ((x % 2 == 0) && (y % 2 == 0))) {
                layout[y][x] = TileType::Wall;
            }
            else {
                layout[y][x] = TileType::Empty;
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
            switch (layout[y][x]) {
                case TileType::Wall:
                    draw_sprite(wall_1, 16, 16, x * 16 - 8, y * 16 + 24);
                    break;
                case TileType::Brick:
                    draw_sprite(wall_2, 16, 16, x * 16 - 8, y * 16 + 24);
                    break;
                case TileType::Bomb:
                    // draw_sprite(bomb_sprite, 16, 16, x * 16 - 8, y * 16 + 24);
                    break;
                default:
                    break;
            }
        }
    }
}