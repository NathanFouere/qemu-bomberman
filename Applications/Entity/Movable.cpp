#include "Movable.h"
#include "../Board/Board.h"
#include <drivers/vga.h>

const unsigned char* Movable::getSprite() {
    return sprites[direction][animationFrame % 3];
}

void Movable::move(const Board& board, int dx, int dy) {
    int newX = x + dx, newY = y + dy;
    int w = 16;
    int h = 16;

    // check four corners
    int cornersX[2] = { newX,      newX + w - 1 };
    int cornersY[2] = { newY,      newY + h - 1 };

    for (int ix = 0; ix < 2; ++ix) {
        for (int iy = 0; iy < 2; ++iy) {
            if ( board.isBlockedAt(cornersX[ix], cornersY[iy]) ) {
                return;
            }
        }
    }

    x = newX;
    y = newY;
}