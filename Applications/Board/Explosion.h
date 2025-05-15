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
                    explosionSprites[0] = exp_mid_1;
                    explosionSprites[1] = exp_mid_2;
                    explosionSprites[2] = exp_mid_3;
                    explosionSprites[3] = exp_mid_4;
                    explosionSprites[4] = exp_mid_4;
                    explosionSprites[5] = exp_mid_3;
                    explosionSprites[6] = exp_mid_2;
                    explosionSprites[7] = exp_mid_1;
                    break;
                    
                case ExplosionState::EXPLOSION_DOWN:
                    explosionSprites[0] = exp_mid_1;
                    explosionSprites[1] = exp_mid_2;
                    explosionSprites[2] = exp_mid_3;
                    explosionSprites[3] = exp_mid_4;
                    explosionSprites[4] = exp_mid_4;
                    explosionSprites[5] = exp_mid_3;
                    explosionSprites[6] = exp_mid_2;
                    explosionSprites[7] = exp_mid_1;
                    break;
                    
                case ExplosionState::EXPLOSION_LEFT:
                    // Horizontal version
                    explosionSprites[0] = exp_mid_1;
                    explosionSprites[1] = exp_mid_2;
                    explosionSprites[2] = exp_mid_3;
                    explosionSprites[3] = exp_mid_4;
                    explosionSprites[4] = exp_mid_4;
                    explosionSprites[5] = exp_mid_3;
                    explosionSprites[6] = exp_mid_2;
                    explosionSprites[7] = exp_mid_1;
                    break;
                    
                case ExplosionState::EXPLOSION_RIGHT:
                    explosionSprites[0] = exp_mid_1;
                    explosionSprites[1] = exp_mid_2;
                    explosionSprites[2] = exp_mid_3;
                    explosionSprites[3] = exp_mid_4;
                    explosionSprites[4] = exp_mid_4;
                    explosionSprites[5] = exp_mid_3;
                    explosionSprites[6] = exp_mid_2;
                    explosionSprites[7] = exp_mid_1;
                    break;
                    
                case ExplosionState::EXPLOSION_UP_END:
                    explosionSprites[0] = exp_end_1;
                    explosionSprites[1] = exp_end_2;
                    explosionSprites[2] = exp_end_3;
                    explosionSprites[3] = exp_end_4;
                    explosionSprites[4] = exp_end_4;
                    explosionSprites[5] = exp_end_3;
                    explosionSprites[6] = exp_end_2;
                    explosionSprites[7] = exp_end_1;
                    break;
                    
                case ExplosionState::EXPLOSION_DOWN_END:
                    explosionSprites[0] = exp_end_1;
                    explosionSprites[1] = exp_end_2;
                    explosionSprites[2] = exp_end_3;
                    explosionSprites[3] = exp_end_4;
                    explosionSprites[4] = exp_end_4;
                    explosionSprites[5] = exp_end_3;
                    explosionSprites[6] = exp_end_2;
                    explosionSprites[7] = exp_end_1;
                    break;
                    
                case ExplosionState::EXPLOSION_LEFT_END:
                    // Horizontal end
                    explosionSprites[0] = exp_end_1;
                    explosionSprites[1] = exp_end_2;
                    explosionSprites[2] = exp_end_3;
                    explosionSprites[3] = exp_end_4;
                    explosionSprites[4] = exp_end_4;
                    explosionSprites[5] = exp_end_3;
                    explosionSprites[6] = exp_end_2;
                    explosionSprites[7] = exp_end_1;
                    break;
                    
                case ExplosionState::EXPLOSION_RIGHT_END:
                    explosionSprites[0] = exp_end_1;
                    explosionSprites[1] = exp_end_2;
                    explosionSprites[2] = exp_end_3;
                    explosionSprites[3] = exp_end_4;
                    explosionSprites[4] = exp_end_4;
                    explosionSprites[5] = exp_end_3;
                    explosionSprites[6] = exp_end_2;
                    explosionSprites[7] = exp_end_1;
                    break;
                    
                default:
                    break;
            }
        }

        ExplosionState getState() const {
            return state;
        }

        void render(int renderX, int renderY) override {
            RotationType rotation = RotationType::NONE;
            
            // Set rotation based on explosion state
            switch(state) {
                case ExplosionState::EXPLOSION_DOWN:
                case ExplosionState::EXPLOSION_DOWN_END:
                    rotation = RotationType::ROTATE_180;
                    break;
                    
                case ExplosionState::EXPLOSION_LEFT:
                case ExplosionState::EXPLOSION_LEFT_END:
                    rotation = RotationType::ROTATE_270;
                    break;
                    
                case ExplosionState::EXPLOSION_RIGHT:
                case ExplosionState::EXPLOSION_RIGHT_END:
                    rotation = RotationType::ROTATE_90;
                    break;
                    
                default:
                    rotation = RotationType::NONE;
                    break;
            }
            
            // Draw with rotation if needed
            draw_sprite_rotated(explosionSprites[animationFrame % 8], 16, 16, 
                              renderX * 16 - 8, renderY * 16 + 24,
                              rotation);

            // Use the AnimationTimer for consistent timing
            if (animTimer.shouldUpdate()) {
                // Only increment frame counter if we haven't reached the end
                if (animationFrame < 7) {
                    animationFrame++;
                } else {
                        board->setTileAt(x, y, new EmptyTile());
                }
            }
        }
    };
        
    
