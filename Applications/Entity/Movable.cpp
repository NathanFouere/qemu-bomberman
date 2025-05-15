#include "Movable.h"
#include "../Board/Board.h"
#include <drivers/vga.h>

const unsigned char* Movable::getSprite() {
    if (status == EntityStatus::DEAD_ANIMATION) {
        return deathSprites[(animationFrame / 30) % 4];
    }
    
    return sprites[direction][animationFrame % 3];
}

void Movable::move(const Board& board, int dx, int dy) {
    constexpr int w = 16, h = 16;
    constexpr int maxSlide = 8;

    // HORIZONTAL
    if (dx != 0) {
        int nx = x + dx;
        auto clearAtY = [&](int testY) {
            int cx = nx + (dx > 0 ? w - 1 : 0);
            return !board.isBlockedAt(cx, testY)
                && !board.isBlockedAt(cx, testY + h - 1);
        };

        if (clearAtY(y)) {
            x = nx;
            return;
        }
        
        for (int offset = 1; offset <= maxSlide; ++offset) {
            if (clearAtY(y - offset)
             && !board.isBlockedAt(x, y - offset)
             && !board.isBlockedAt(x + w - 1, y - offset + h - 1)) {
                y -= 1;
                return;
            }
           
            if (clearAtY(y + offset)
             && !board.isBlockedAt(x, y + offset + h - 1)
             && !board.isBlockedAt(x + w - 1, y + offset)) {
                y += 1;
                return;
            }
        }
        return;
    }

    // VERTICAL
    if (dy != 0) {
        int ny = y + dy;
        auto clearAtX = [&](int testX) {
            int cy = ny + (dy > 0 ? h - 1 : 0);
            return !board.isBlockedAt(testX, cy)
                && !board.isBlockedAt(testX + w - 1, cy);
        };

        if (clearAtX(x)) {
            y = ny;
            return;
        }
        for (int offset = 1; offset <= maxSlide; ++offset) {
            if (clearAtX(x - offset)
             && !board.isBlockedAt(x - offset, y)
             && !board.isBlockedAt(x - offset + w - 1, y + h - 1)) {
                x -= 1;
                return;
            }
            if (clearAtX(x + offset)
             && !board.isBlockedAt(x + offset + w - 1, y)
             && !board.isBlockedAt(x + offset, y + h - 1)) {
                x += 1;
                return;
            }
        }
    }
}