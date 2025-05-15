#pragma once

#include "Tile.h"
#include "EmptyTile.h"
#include <drivers/vga.h>
#include <drivers/sprite.h>
#include <drivers/timer.h>
#include "Board.h"
#include "ExplosionState.h"
#include "../Utilities/AnimationTimer.h"

class Explosion : public Tile {
    private:
        int animationFrame;
        unsigned char* explosionSprites[8];
        Board* board;
        int x;
        int y;
        ExplosionState state;
        AnimationTimer animTimer;

    public:
        Explosion(Board* board, int x, int y) : Tile(), animTimer(100) {
            animationFrame = 0;
        }

        Explosion(Board* board, int x, int y, ExplosionState state) : Tile(), animTimer(100) {
            this->board = board;
            this->x = x;
            this->y = y;
            this->state = state;
            this->animationFrame = 0;
            setCorrectSprites();
        }

        TileType getType() const override {
            return TILE_EXPLOSION;
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
            draw_sprite(explosionSprites[animationFrame % 8], 16, 16, renderX * 16 - 8, renderY * 16 + 24);
            
            // Use AnimationTimer instead of direct tick calculation
            if (animTimer.shouldUpdate()) {
                animationFrame++;
                
                // When animation is complete, replace with empty tile
                if (animationFrame >= 8) {
                    board->setTileAt(x, y, new EmptyTile());
                }
            }
        }
    };