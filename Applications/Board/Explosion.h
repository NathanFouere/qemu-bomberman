#pragma once

#include "Tile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <drivers/timer.h>
#include "Board.h"
#include "ExplosionState.h"

class Explosion : public Tile {
    private:
        int apparitionSecond;
        int animationFrame;
        unsigned char* explosionSprites[8];
        Board* board;
        int x;
        int y;
        ExplosionState state;

    public:
        Explosion(Board* board, int x, int y) : Tile() {
            this->board = board;
            this->x = x;
            this->y = y;
            this->state = state;
            animationFrame = 0;
        }

        Explosion(Board* board, int x, int y, ExplosionState state) : Tile() {
            this->board = board;
            this->x = x;
            this->y = y;
            this->state = state;
            animationFrame = 0;
            setCorrectSprites();
        }

        TileType getType() const override {
            return TILE_EXPLOSION;
        }

        void setCorrectSprites() {
            switch (state) {
                case ExplosionState::CENTER:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::UP:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::DOWN:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::LEFT:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::RIGHT:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::UP_END:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::DOWN_END:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::LEFT_END:
                    explosionSprites[0] = exp_start_1;
                    explosionSprites[1] = exp_start_2;
                    explosionSprites[2] = exp_start_3;
                    explosionSprites[3] = exp_start_4;
                    explosionSprites[4] = exp_start_4;
                    explosionSprites[5] = exp_start_3;
                    explosionSprites[6] = exp_start_2;
                    explosionSprites[7] = exp_start_1;
                    break;
                case ExplosionState::RIGHT_END:
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

        void render(int x, int y) override {
            if (animationFrame < 8) {
                draw_sprite(explosionSprites[animationFrame % 8], 16, 16, x * 16 - 8, y * 16 + 24);
            }
            animationFrame++;
            if (animationFrame >= 8) {
                board->deleteTileAt(x, y);
                board->setTileAt(x, y, new EmptyTile());
            }
        }
    };