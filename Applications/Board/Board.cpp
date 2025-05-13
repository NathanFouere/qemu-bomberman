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

    if (layout[ty][tx]) {
        delete layout[ty][tx];
    }
    layout[ty][tx] = tile;
}

bool Board::isBlockedAt(int px, int py) const {
    int localX = px - BOARD_ORIGIN_X;
    int localY = py - BOARD_ORIGIN_Y;

    int tx = localX / TILE_SIZE;
    int ty = localY / TILE_SIZE;

    if (tx < 0 || ty < 0 || tx >= width || ty >= height)
        return true;

    return layout[ty][tx]->getType() != TILE_EMPTY &&
           layout[ty][tx]->getType() != TILE_EXPLOSION;
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

    // if (tx < 0 || ty < 0 || tx >= width || ty >= height) {
    //     return; 
    // }

    TileType type = layout[ty][tx]->getType();
    if (type == TILE_BRICK || type == TILE_BOMB || type == TILE_EXPLOSION) {
        if (layout[ty][tx]) {
            delete layout[ty][tx];
        }
    }
}

void Board::updateExplosion() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (layout[y][x]->getType() == TILE_EXPLOSION && layout[y][x]->getAnimationFrame() == 7) {
                layout[y][x] = new EmptyTile();
            }
        }
    }
}

void Board::bombExploded(int x, int y, int power) {
    // supprime la tuile actuelle
    deleteTileAt(x, y);
    setTileAt(x, y, new Explosion(this, x, y, ExplosionState::EXPLOSION_CENTER));

    int localX = x - BOARD_ORIGIN_X;
    int localY = y - BOARD_ORIGIN_Y;
    int tx = localX / TILE_SIZE;
    int ty = localY / TILE_SIZE;

    // supprime à gauche
    TileType type;
    for (int i = 1; i <= power; ++i) {
        // if (i < power)
        // {
        //     type = layout[y][x-TILE_SIZE * i]->getType();
        //     if (type == TILE_BRICK || type == TILE_BOMB) {
        //         deleteTileAt(x-TILE_SIZE * i, y);
        //         setTileAt(x-TILE_SIZE * i, y, new EmptyTile());
        //     } 
        //     else if (type == TILE_EMPTY) {
        //         deleteTileAt(x-TILE_SIZE * i, y);
        //         setTileAt(x-TILE_SIZE * i, y, new Explosion(this, x-TILE_SIZE * i, y, ExplosionState::LEFT));
        //     }

        //     // supprime à droite
        //     type = layout[y][x+TILE_SIZE * i]->getType();
        //     if (type == TILE_BRICK || type == TILE_BOMB) {
        //         deleteTileAt(x+TILE_SIZE * i, y);
        //         setTileAt(x+TILE_SIZE * i, y, new EmptyTile());
        //     } 
        //     else if (type == TILE_EMPTY) {
        //         deleteTileAt(x+TILE_SIZE *i, y);
        //         setTileAt(x+TILE_SIZE *i, y, new Explosion(this, x+TILE_SIZE *i, y, ExplosionState::RIGHT));
        //     }

        //     // supprime en haut
        //     type = layout[y-TILE_SIZE * i][x]->getType();
        //     if (type == TILE_BRICK || type == TILE_BOMB) {
        //         deleteTileAt(x, y-TILE_SIZE * i);
        //         setTileAt(x, y-TILE_SIZE * i, new EmptyTile());
        //     } 
        //     else if (type == TILE_EMPTY) {
        //         deleteTileAt(x, y-TILE_SIZE * i);
        //         setTileAt(x, y-TILE_SIZE *i, new Explosion(this, x, y-TILE_SIZE *i, ExplosionState::UP));
        //     }
            
        //     // supprime en dessous
        //     type = layout[y+TILE_SIZE * i][x]->getType();
        //     if (type == TILE_BRICK || type == TILE_BOMB) {
        //         deleteTileAt(x, y+TILE_SIZE * i);
        //         setTileAt(x, y+TILE_SIZE * i, new EmptyTile());
        //     } 
        //     else if (type == TILE_EMPTY) {
        //         deleteTileAt(x, y+TILE_SIZE);
        //         setTileAt(x, y+TILE_SIZE, new Explosion(this, x, y+TILE_SIZE * i, ExplosionState::DOWN));
        //     }
        // } 
        // else 
        if (i == power) {
            // supprime à gauche
            if (layout[ty][tx-1 * i]->getType() == TILE_BRICK || layout[ty][tx-1 * i]->getType() == TILE_BOMB) {
                setTileAt(x-TILE_SIZE * i, y, new EmptyTile());
            } else if (layout[ty][tx-1 * i]->getType() == TILE_EMPTY) {
                setTileAt(x-TILE_SIZE * i, y, new Explosion(this, x-TILE_SIZE * i, y, ExplosionState::EXPLOSION_LEFT_END));
            } else {
                break;
            }

            // // supprime à droite
            // type = layout[y][x+TILE_SIZE * i]->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     deleteTileAt(x+TILE_SIZE * i, y);
            //     setTileAt(x+TILE_SIZE * i, y, new EmptyTile());
            // } else if (type == TILE_EMPTY) {
            //     deleteTileAt(x+TILE_SIZE * i, y);
            //     setTileAt(x+TILE_SIZE * i, y, new Explosion(this, x+TILE_SIZE * i, y, ExplosionState::RIGHT_END));
            // }

            // // supprime en haut
            // type = layout[y-TILE_SIZE * i][x]->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     deleteTileAt(x, y-TILE_SIZE * i);
            //     setTileAt(x, y-TILE_SIZE * i, new EmptyTile());
            // } else if (type == TILE_EMPTY) {
            //     deleteTileAt(x, y-TILE_SIZE * i);
            //     setTileAt(x, y-TILE_SIZE * i, new Explosion(this, x, y-TILE_SIZE * i, ExplosionState::UP_END));
            // }
            
            // // supprime en dessous
            // type = layout[y+TILE_SIZE * i][x]->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     deleteTileAt(x, y+TILE_SIZE * i);
            //     setTileAt(x, y+TILE_SIZE * i, new EmptyTile());
            // } else if (type == TILE_EMPTY) {
            //     deleteTileAt(x, y+TILE_SIZE * i);
            //     setTileAt(x, y+TILE_SIZE * i, new Explosion(this, x, y+TILE_SIZE * i, ExplosionState::DOWN_END));
            // }
        }
    }

}