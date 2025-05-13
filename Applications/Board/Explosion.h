#pragma once

#include "Tile.h"
#include "EmptyTile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <drivers/timer.h>
#include "Board.h"
#include "ExplosionState.h"

class Explosion : public Tile {
    private:
        int animationFrame;
        unsigned char* explosionSprites[8];
        Board* board;
        int x;
        int y;
        ExplosionState state;

    public:
        Explosion(Board* board, int x, int y) : Tile() {}

        Explosion(Board* board, int x, int y, ExplosionState state) : Tile() {
            this->board = board;
            this->x = x;
            this->y = y;
            this->state = state;
            setCorrectSprites();
        }

        TileType getType() const override {
            return TILE_BOMB_EXPLODING;
        }

        void setCorrectSprites() {
            switch (state) {
                case ExplosionState::EXPLOSION_CENTER:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_UP:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_DOWN:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_LEFT:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_RIGHT:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_UP_END:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_DOWN_END:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_LEFT_END:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::EXPLOSION_RIGHT_END:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                default:
                    break;
            }
        }

        ExplosionState getState() const {
            return state;
        }

        void render(int renderX, int renderY) override {
            draw_number(animationFrame, 70, 9, 15);
            draw_sprite(explosionSprites[animationFrame % 8], 16, 16, renderX * 16 - 8, renderY * 16 + 24);
            unsigned int currentTicks = Timer::getInstance().getTicks();
            if (currentTicks - getLastAnimTick() >= 300) {
                animationFrame++;
                setLastAnimTick(currentTicks);
            }

            if (animationFrame >= 8) {
                board->setTileAt(x, y, new EmptyTile());
            }
        }
    };