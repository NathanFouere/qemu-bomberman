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
    board->bombExploded(x, y, power);
}