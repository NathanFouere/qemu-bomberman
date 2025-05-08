#include "Board.h"

void Board::initializeWalls() {
    for (int x = 0; x < width; ++x) {
        setTile(x, 0, TileType::Wall);
        setTile(x, height - 1, TileType::Wall);
    }
    for (int y = 0; y < height; ++y) {
        setTile(0, y, TileType::Wall);
        setTile(width - 1, y, TileType::Wall);
    }

    for (int y = 2; y < height - 2; y += 2) {
        for (int x = 2; x < width - 2; x += 2) {
            setTile(x, y, TileType::Wall);
        }
    }
}

void Board::draw() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            switch (layout[y][x]) {
                case TileType::Wall:
                    draw_sprite(wall_1, 16, 16, x * 16, y * 16);
                    break;
                case TileType::Brick:
                    // draw_sprite(brick_sprite, 16, 16, x * 16, y * 16);
                    break;
                case TileType::Bomb:
                    // draw_sprite(bomb_sprite, 16, 16, x * 16, y * 16);
                    break;
                default:
                    break; // rien pour Empty
            }
        }
    }
}