#include "Tile.h"

Tile::Tile(TileType type) : type(type), nbSprites(0) {
    for (int i = 0; i < MAX_SPRITES; ++i) sprites[i] = nullptr;
}

Tile::Tile() : Tile(TileType::Empty) {}
