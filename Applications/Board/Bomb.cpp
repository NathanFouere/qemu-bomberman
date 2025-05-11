#include "Bomb.h"

Bomb::Bomb() : Tile() {
    apparitionSecond = Timer::getInstance().getSeconds();
    animationFrame = 0;
    sprites[0] = bomb_1;
    sprites[1] = bomb_2;
    sprites[2] = bomb3;
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
}

void Bomb::render(int x, int y) {        
    bool exploded = (Timer::getInstance().getSeconds() - this->apparitionSecond >= 3);    
    if (exploded)
    {
        draw_sprite(explosionSprites[animationFrame % 12], 16, 16, x * 16 - 8, y * 16 + 24);
    }else {
        draw_sprite(sprites[animationFrame % 3], 16, 16, x * 16 - 8, y * 16 + 24);
    }
    animationFrame++;
}