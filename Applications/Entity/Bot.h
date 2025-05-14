#include "Applications/Entity/Movable.h"
#include "drivers/Clavier.h"
#include "../Board/Board.h"
#include "sextant/Activite/Threads.h"
#include <drivers/sprite.h>

enum class EnemyType {
    TYPE1,
    TYPE2,
    TYPE3,
    TYPE4,
    TYPE5
};

class Bot : public Threads, public Movable {
public:
    bool alive = true;
    EnemyType type = EnemyType::TYPE1;
    Board* board;
  
    Bot(int x, int y, EnemyType type, Board* boardPtr)
        : Movable(x, y), type(type), board(boardPtr) {
        setSprites(type);
        status = EntityStatus::ALIVE;
    }

    void moveRandom();
    void run();
    void render();
    bool isValidMove(Board& board, int dx, int dy);

private:
    // Rate limiting for movement
    const unsigned long moveDelay = 75;  // milliseconds between moves
    const unsigned long directionChangeInterval = 1000; // Time between direction changes
    
    // Per-instance variables for movement tracking
    unsigned long lastChangeTime = 0;  // Last time direction was changed
    unsigned long lastMoveTime = 0;    // Last time bot moved
    int directionIndex = -1;           // Current direction
    void setSprites(EnemyType type) {
        switch (type) {
            case EnemyType::TYPE1:
                sprites[UP][0] = enemy1_right_1;
                sprites[UP][1] = enemy1_right_2;
                sprites[UP][2] = enemy1_right_3;

                sprites[DOWN][0] = enemy1_left_1;
                sprites[DOWN][1] = enemy1_left_2;
                sprites[DOWN][2] = enemy1_left_3;

                sprites[LEFT][0] = enemy1_left_1;
                sprites[LEFT][1] = enemy1_left_2;
                sprites[LEFT][2] = enemy1_left_3;

                sprites[RIGHT][0] = enemy1_right_1;
                sprites[RIGHT][1] = enemy1_right_2;
                sprites[RIGHT][2] = enemy1_right_3;

                deathSprites[0] = enemy1_death_1;
                deathSprites[1] = enemy1_death_2;
                deathSprites[2] = enemy1_death_3;
                deathSprites[3] = enemy1_death_4;
                deathSprites[4] = enemy1_death_5;
                break;

            case EnemyType::TYPE2:
                sprites[UP][0] = enemy2_right_1;
                sprites[UP][1] = enemy2_right_2;
                sprites[UP][2] = enemy2_right_3;

                sprites[DOWN][0] = enemy2_left_1;
                sprites[DOWN][1] = enemy2_left_2;
                sprites[DOWN][2] = enemy2_left_3;

                sprites[LEFT][0] = enemy2_left_1;
                sprites[LEFT][1] = enemy2_left_2;
                sprites[LEFT][2] = enemy2_left_3;

                sprites[RIGHT][0] = enemy2_right_1;
                sprites[RIGHT][1] = enemy2_right_2;
                sprites[RIGHT][2] = enemy2_right_3;
                deathSprites[0] = enemy2_death_1;
                deathSprites[1] = enemy2_death_2;
                deathSprites[2] = enemy2_death_3;
                deathSprites[3] = enemy2_death_4;
                deathSprites[4] = enemy2_death_5;
                break;

            case EnemyType::TYPE3:
                sprites[UP][0] = enemy3_right_1;
                sprites[UP][1] = enemy3_right_2;
                sprites[UP][2] = enemy3_right_3;

                sprites[DOWN][0] = enemy3_left_1;
                sprites[DOWN][1] = enemy3_left_2;
                sprites[DOWN][2] = enemy3_left_3;

                sprites[LEFT][0] = enemy3_left_1;
                sprites[LEFT][1] = enemy3_left_2;
                sprites[LEFT][2] = enemy3_left_3;

                sprites[RIGHT][0] = enemy3_right_1;
                sprites[RIGHT][1] = enemy3_right_2;
                sprites[RIGHT][2] = enemy3_right_3;

                deathSprites[0] = enemy3_death_1;
                deathSprites[1] = enemy3_death_2;
                deathSprites[2] = enemy3_death_3;
                deathSprites[3] = enemy3_death_4;
                deathSprites[4] = enemy3_death_5;
                break;

            case EnemyType::TYPE4:
                sprites[UP][0] = enemy4_right_1;
                sprites[UP][1] = enemy4_right_2;
                sprites[UP][2] = enemy4_right_3;

                sprites[DOWN][0] = enemy4_left_1;
                sprites[DOWN][1] = enemy4_left_2;
                sprites[DOWN][2] = enemy4_left_3;

                sprites[LEFT][0] = enemy4_left_1;
                sprites[LEFT][1] = enemy4_left_2;
                sprites[LEFT][2] = enemy4_left_3;

                sprites[RIGHT][0] = enemy4_right_1;
                sprites[RIGHT][1] = enemy4_right_2;
                sprites[RIGHT][2] = enemy4_right_3;

                deathSprites[0] = enemy4_death_1;
                deathSprites[1] = enemy4_death_1;
                deathSprites[2] = enemy4_death_1;
                deathSprites[3] = enemy4_death_1;
                deathSprites[4] = enemy4_death_1;
                break;

            case EnemyType::TYPE5:
                sprites[UP][0] = enemy5_right_1;
                sprites[UP][1] = enemy5_right_2;
                sprites[UP][2] = enemy5_right_3;

                sprites[DOWN][0] = enemy5_left_1;
                sprites[DOWN][1] = enemy5_left_2;
                sprites[DOWN][2] = enemy5_left_3;

                sprites[LEFT][0] = enemy5_left_1;
                sprites[LEFT][1] = enemy5_left_2;
                sprites[LEFT][2] = enemy5_left_3;

                sprites[RIGHT][0] = enemy5_right_1;
                sprites[RIGHT][1] = enemy5_right_2;
                sprites[RIGHT][2] = enemy5_right_3;

                deathSprites[0] = enemy5_death_1;
                deathSprites[1] = enemy5_death_2;
                deathSprites[2] = enemy5_death_3;
                deathSprites[3] = enemy5_death_4;
                deathSprites[4] = enemy5_death_5;
                break;
        }
    }
};