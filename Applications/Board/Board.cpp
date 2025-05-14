#include "Board.h"
#include "Tile.h"
#include "EmptyTile.h"
#include "Wall.h"
#include "Brick.h"
#include "Bomb.h"
#include "Explosion.h"
#include "../Utilities/PseudoRand.h"

Board::Board(int w, int h) : width(w), height(h) {
    layout = static_cast<Tile***>(alloc(height * sizeof(Tile**)));
    for (int y = 0; y < height; ++y) {
        layout[y] = static_cast<Tile**>(alloc(width * sizeof(Tile*)));
        for (int x = 0; x < width; ++x) {
            bool isBorder       = (x == 0) || (y == 0) || (y == height - 1);
            bool isCheckerboard = ((x % 2) == 0) && ((y % 2) == 0);
            bool isEntryPoint   = (x == width - 1) && ( (y == 1) || (y == height - 2) );
            bool safeZone       = (x <= 2 && y <= 2);

            Tile* chosenTile;
            if ((isBorder || isCheckerboard) && !isEntryPoint) {
                chosenTile = new Wall();
            }
            else if (safeZone) {
                chosenTile = new EmptyTile();
            }
            else {
                if (pseudoRand() % 100 < 50) {
                    chosenTile = new Brick();
                } else {
                    chosenTile = new EmptyTile();
                }
            }

            int px = x * TILE_SIZE + BOARD_ORIGIN_X;
            int py = y * TILE_SIZE + BOARD_ORIGIN_Y;
            setTileAt(px, py, chosenTile);
        }
    }
}

void Board::setTileAt(int px, int py, Tile* tile) {
    int localX = px - BOARD_ORIGIN_X;
    int localY = py - BOARD_ORIGIN_Y;
    int tx = localX / TILE_SIZE;
    int ty = localY / TILE_SIZE;

    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
        return; 
    }

    delete layout[ty][tx];
    layout[ty][tx] = tile;
}

bool Board::isBlockedAt(int px, int py) const {
    int localX = px - BOARD_ORIGIN_X;
    int localY = py - BOARD_ORIGIN_Y;

    int tx = localX / TILE_SIZE;
    int ty = localY / TILE_SIZE;

    if (tx < 0 || ty < 0 || tx >= width || ty >= height)
        return true;

    return layout[ty][tx]->getType() != TILE_EMPTY;
}

Board::~Board() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            delete layout[y][x];
        }
        free(layout[y]);
    }
    free(layout);
}

void Board::draw() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            layout[y][x]->render(x, y);
        }
    }
}

void Board::deleteTileAt(int px, int py) {
    int localX = px - BOARD_ORIGIN_X;
    int localY = py - BOARD_ORIGIN_Y;
    int tx = localX / TILE_SIZE;
    int ty = localY / TILE_SIZE;

    if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
        return; 
    }

    TileType type = layout[ty][tx]->getType();
    if (type == TILE_BRICK || type == TILE_BOMB || type == TILE_EXPLOSION) {
        if (layout[ty][tx]) {
            delete layout[ty][tx];
        }
    }
}

void Board::bombExploded(int x, int y, int power) {
    // supprime la tuile actuelle
    setTileAt(x, y, new Explosion(this, x, y, ExplosionState::EXPLOSION_CENTER));

    int localX = x - BOARD_ORIGIN_X;
    int localY = y - BOARD_ORIGIN_Y;
    int tx = localX / TILE_SIZE;
    int ty = localY / TILE_SIZE;

    TileType type;
    for (int i = 1; i <= power; ++i) {
        // supprime à gauche
        if (layout[ty][tx-1 * i]->getType() == TILE_BRICK || layout[ty][tx-1 * i]->getType() == TILE_BOMB) {
            setTileAt(x-TILE_SIZE * i, y, new EmptyTile());
        }
        else if (layout[ty][tx-1 * i]->getType() == TILE_EMPTY) {
            if (i == power) {
                setTileAt(x-TILE_SIZE * i, y, new Explosion(this, x-TILE_SIZE * i, y, ExplosionState::EXPLOSION_LEFT_END));
            } else
            {
                setTileAt(x-TILE_SIZE * i, y, new Explosion(this, x-TILE_SIZE * i, y, ExplosionState::EXPLOSION_LEFT));
            }
        }

        // supprime à droite
        if (layout[ty][tx+1 * i]->getType() == TILE_BRICK || layout[ty][tx+1 * i]->getType() == TILE_BOMB) {
            setTileAt(x+TILE_SIZE * i, y, new EmptyTile());
        }
        else if (layout[ty][tx+1 * i]->getType() == TILE_EMPTY) {
            // if i == power ExplosionState::EXPLOSION_RIGHT_END else ExplosionState::EXPLOSION_RIGHT
            if (i == power) {
                setTileAt(x+TILE_SIZE * i, y, new Explosion(this, x+TILE_SIZE * i, y, ExplosionState::EXPLOSION_RIGHT_END));
            } else
            {
                setTileAt(x+TILE_SIZE * i, y, new Explosion(this, x+TILE_SIZE * i, y, ExplosionState::EXPLOSION_RIGHT));
            }
        }

        // supprime en haut
        if (layout[ty-1 * i][tx]->getType() == TILE_BRICK || layout[ty-1 * i][tx]->getType() == TILE_BOMB) {
            setTileAt(x, y-TILE_SIZE * i, new EmptyTile());
        }
        else if (layout[ty-1 * i][tx]->getType() == TILE_EMPTY) {
            // if i == power ExplosionState::EXPLOSION_UP_END else ExplosionState::EXPLOSION_UP
            if (i == power) {
                setTileAt(x, y-TILE_SIZE * i, new Explosion(this, x, y-TILE_SIZE * i, ExplosionState::EXPLOSION_UP_END));
            } else
            {
                setTileAt(x, y-TILE_SIZE * i, new Explosion(this, x, y-TILE_SIZE * i, ExplosionState::EXPLOSION_UP));
            }
        }

        // supprime en dessous
        if (layout[ty+1 * i][tx]->getType() == TILE_BRICK || layout[ty+1 * i][tx]->getType() == TILE_BOMB) {
            setTileAt(x, y+TILE_SIZE * i, new EmptyTile());
        }
        else if (layout[ty+1 * i][tx]->getType() == TILE_EMPTY) {
            // if i == power ExplosionState::EXPLOSION_DOWN_END else ExplosionState::EXPLOSION_DOWN
            if (i == power) {
                setTileAt(x, y+TILE_SIZE * i, new Explosion(this, x, y+TILE_SIZE * i, ExplosionState::EXPLOSION_DOWN_END));
            } else
            {
                setTileAt(x, y+TILE_SIZE * i, new Explosion(this, x, y+TILE_SIZE * i, ExplosionState::EXPLOSION_DOWN));
            }
        }
    }

}

TileType Board::getTileTypeAt(int x, int y) {
    int localX = x - BOARD_ORIGIN_X + 1; // obliger de faire + 1, il me semble que cela vient de la gestion des collisions
    int localY = y - BOARD_ORIGIN_Y + 1; // idem
    int tx = localX / TILE_SIZE;
    int ty = localY / TILE_SIZE;

    return layout[ty][tx]->getType();
}