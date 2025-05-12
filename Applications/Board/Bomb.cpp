#include "Bomb.h"
#include "EmptyTile.h"

Bomb::Bomb(Board* board, int x, int y) : Tile() {
    apparitionSecond = Timer::getInstance().getSeconds();
    animationFrame = 0;
    sprites[0] = bomb_1;
    sprites[1] = bomb_2;
    sprites[2] = bomb3;
    // TODO => il faudra fixer pour que ça apparaisse autour du joueur là c'est pas censé marcher comme ça
    explosionSprites[0] = exp_start_1;
    explosionSprites[1] = exp_start_2;
    explosionSprites[2] = exp_start_3;
    explosionSprites[3] = exp_start_4;
    explosionSprites[4] = exp_mid_1;
    explosionSprites[5] = exp_mid_2;
    explosionSprites[6] = exp_mid_3;
    explosionSprites[7] = exp_mid_4;
    explosionSprites[8] = exp_end_1;
    explosionSprites[9] = exp_end_2;
    explosionSprites[10] = exp_end_3;
    explosionSprites[11] = exp_end_4;
    this->board = board;
    this->board->setTileAt(x, y, this);
    this->exploded = false;
    this->x = x;
    this->y = y;
}

void Bomb::render(int x, int y) {      
    exploded = (Timer::getInstance().getSeconds() - this->apparitionSecond >= 3);  
    // TODO => il faudra supprimer la bombe quand elle aura exploser
    if (exploded)
    {
        this->handleExplosion();
        draw_sprite(explosionSprites[animationFrame % 12], 16, 16, x * 16 - 8, y * 16 + 24);

        bool erase = (Timer::getInstance().getSeconds() - this->apparitionSecond >= 5);
        if (erase) {
            this->board->setTileAt(x, y, new EmptyTile());
        }
    }else {
        draw_sprite(sprites[animationFrame % 3], 16, 16, x * 16 - 8, y * 16 + 24);
    }
    animationFrame++;
}

void Bomb::handleExplosion() {

    // supprime à gauche
    this->board->deleteTileAt(x - TILE_SIZE, y);

        // supprime à droite (marche pas)
    this->board->deleteTileAt(x + TILE_SIZE, y);

    // supprime en haut (marche)
    this->board->deleteTileAt(x, y+TILE_SIZE);

    // supprime en dessous (marche pas)
    this->board->deleteTileAt(x, y-TILE_SIZE);
}