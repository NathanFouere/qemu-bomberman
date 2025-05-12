#include "Bomb.h"
#include "EmptyTile.h"
#include "Explosion.h"
#include "Tile.h"

Bomb::Bomb(Board* board, int x, int y) {
    apparitionSecond = Timer::getInstance().getSeconds();
    animationFrame = 0;
    sprites[0] = bomb_1;
    sprites[1] = bomb_2;
    sprites[2] = bomb3;
    this->board = board;
    this->board->setTileAt(x, y, this);
    this->exploded = false;
    this->x = x;
    this->y = y;
}

void Bomb::render(int x, int y) {      
    if (Timer::getInstance().getSeconds() - apparitionSecond > 3) {
        exploded = true;
        handleExplosion();
    } else {
        draw_sprite(sprites[animationFrame % 3], 16, 16, x * 16 - 8, y * 16 + 24);
    }
    animationFrame++;
}

void Bomb::handleExplosion() {
    // supprime la tuile actuelle
    board->deleteTileAt(x, y);
    board->setTileAt(x, y, new Explosion(board, x, y, ExplosionState::CENTER));

    // supprime à gauche
    for (int i = 1; i <= power; ++i) {
        if (i < power)
        {
            // type = board->getTile(x-TILE_SIZE * i, y)->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x-TILE_SIZE * i, y);
            //     board->setTileAt(x-TILE_SIZE * i, y, new EmptyTile());
            // } 
            // else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x-TILE_SIZE * i, y);
            //     board->setTileAt(x-TILE_SIZE * i, y, new Explosion(board, x-TILE_SIZE * i, y, ExplosionState::LEFT));
            // }

            // // supprime à droite
            // type = board->getTile(x+TILE_SIZE * i, y)->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x+TILE_SIZE * i, y);
            //     board->setTileAt(x+TILE_SIZE * i, y, new EmptyTile());
            // } 
            // else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x+TILE_SIZE *i, y);
            //     board->setTileAt(x+TILE_SIZE *i, y, new Explosion(board, x+TILE_SIZE *i, y, ExplosionState::RIGHT));
            // }

            // // supprime en haut
            // type = board->getTile(x, y-TILE_SIZE * i)->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x, y-TILE_SIZE * i);
            //     board->setTileAt(x, y-TILE_SIZE * i, new EmptyTile());
            // } 
            // else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x, y-TILE_SIZE * i);
            //     board->setTileAt(x, y-TILE_SIZE *i, new Explosion(board, x, y-TILE_SIZE *i, ExplosionState::UP));
            // }
            
            // // supprime en dessous
            // type = board->getTile(x, y+TILE_SIZE * i)->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x, y+TILE_SIZE * i);
            //     board->setTileAt(x, y+TILE_SIZE * i, new EmptyTile());
            // } 
            // else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x, y+TILE_SIZE);
            //     board->setTileAt(x, y+TILE_SIZE, new Explosion(board, x, y+TILE_SIZE * i, ExplosionState::DOWN));
            // }
        } 
        else if (i == power) {
            // supprime à gauche
            // if (board->getTile(x, y-TILE_SIZE)->getType() == TILE_EMPTY) {
                
            // }
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x-TILE_SIZE * i, y);
            //     board->setTileAt(x-TILE_SIZE * i, y, new EmptyTile());
            // } else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x-TILE_SIZE * i, y);
            //     board->setTileAt(x-TILE_SIZE * i, y, new Explosion(board, x-TILE_SIZE * i, y, ExplosionState::LEFT_END));
            // }

            // supprime à droite
            // type = board->getTile(x+TILE_SIZE * i, y)->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x+TILE_SIZE * i, y);
            //     board->setTileAt(x+TILE_SIZE * i, y, new EmptyTile());
            // } else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x+TILE_SIZE * i, y);
            //     board->setTileAt(x+TILE_SIZE * i, y, new Explosion(board, x+TILE_SIZE * i, y, ExplosionState::RIGHT_END));
            // }

            // // supprime en haut
            // type = board->getTile(x, y-TILE_SIZE * i)->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x, y-TILE_SIZE * i);
            //     board->setTileAt(x, y-TILE_SIZE * i, new EmptyTile());
            // } else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x, y-TILE_SIZE * i);
            //     board->setTileAt(x, y-TILE_SIZE * i, new Explosion(board, x, y-TILE_SIZE * i, ExplosionState::UP_END));
            // }
            
            // // supprime en dessous
            // type = board->getTile(x, y+TILE_SIZE * i)->getType();
            // if (type == TILE_BRICK || type == TILE_BOMB) {
            //     board->deleteTileAt(x, y+TILE_SIZE * i);
            //     board->setTileAt(x, y+TILE_SIZE * i, new EmptyTile());
            // } else if (type == TILE_EMPTY) {
            //     board->deleteTileAt(x, y+TILE_SIZE * i);
            //     board->
            //     setTileAt(x, y+TILE_SIZE * i, new Explosion(board, x, y+TILE_SIZE * i, ExplosionState::DOWN_END));
            // }
        }
    }
}