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
    int it = 0;
    Board* board;
  
    Bot(int x, int y, EnemyType type, Board* boardPtr)
        : Movable(x, y) {
        setSprites(type);
    }

    void moveRandom();
    void run();

private:
    void setSprites(EnemyType type) {
        switch (type) {
            case EnemyType::TYPE1:
                sprites[UP][0] = enemy1_right_1;
                sprites[UP][1] = enemy1_right_1;
                sprites[UP][2] = enemy1_right_1;

                sprites[DOWN][0] = enemy1_left_1;
                sprites[DOWN][1] = enemy1_left_1;
                sprites[DOWN][2] = enemy1_left_1;

                sprites[LEFT][0] = enemy1_left_1;
                sprites[LEFT][1] = enemy1_left_1;
                sprites[LEFT][2] = enemy1_left_1;

                sprites[RIGHT][0] = enemy1_right_1;
                sprites[RIGHT][1] = enemy1_right_1;
                sprites[RIGHT][2] = enemy1_right_1;
                break;

            case EnemyType::TYPE2:
                sprites[UP][0] = enemy2_right_1;
                sprites[UP][1] = enemy2_right_1;
                sprites[UP][2] = enemy2_right_1;

                sprites[DOWN][0] = enemy2_left_1;
                sprites[DOWN][1] = enemy2_left_1;
                sprites[DOWN][2] = enemy2_left_1;

                sprites[LEFT][0] = enemy2_left_1;
                sprites[LEFT][1] = enemy2_left_1;
                sprites[LEFT][2] = enemy2_left_1;

                sprites[RIGHT][0] = enemy2_right_1;
                sprites[RIGHT][1] = enemy2_right_1;
                sprites[RIGHT][2] = enemy2_right_1;
                break;

            case EnemyType::TYPE3:
                sprites[UP][0] = enemy3_right_1;
                sprites[UP][1] = enemy3_right_1;
                sprites[UP][2] = enemy3_right_1;

                sprites[DOWN][0] = enemy3_left_1;
                sprites[DOWN][1] = enemy3_left_1;
                sprites[DOWN][2] = enemy3_left_1;

                sprites[LEFT][0] = enemy3_left_1;
                sprites[LEFT][1] = enemy3_left_1;
                sprites[LEFT][2] = enemy3_left_1;

                sprites[RIGHT][0] = enemy3_right_1;
                sprites[RIGHT][1] = enemy3_right_1;
                sprites[RIGHT][2] = enemy3_right_1;
                break;

            case EnemyType::TYPE4:
                sprites[UP][0] = enemy4_right_1;
                sprites[UP][1] = enemy4_right_1;
                sprites[UP][2] = enemy4_right_1;

                sprites[DOWN][0] = enemy4_left_1;
                sprites[DOWN][1] = enemy4_left_1;
                sprites[DOWN][2] = enemy4_left_1;

                sprites[LEFT][0] = enemy4_left_1;
                sprites[LEFT][1] = enemy4_left_1;
                sprites[LEFT][2] = enemy4_left_1;

                sprites[RIGHT][0] = enemy4_right_1;
                sprites[RIGHT][1] = enemy4_right_1;
                sprites[RIGHT][2] = enemy4_right_1;
                break;

            case EnemyType::TYPE5:
                sprites[UP][0] = enemy5_right_1;
                sprites[UP][1] = enemy5_right_1;
                sprites[UP][2] = enemy5_right_1;

                sprites[DOWN][0] = enemy5_left_1;
                sprites[DOWN][1] = enemy5_left_1;
                sprites[DOWN][2] = enemy5_left_1;

                sprites[LEFT][0] = enemy5_left_1;
                sprites[LEFT][1] = enemy5_left_1;
                sprites[LEFT][2] = enemy5_left_1;

                sprites[RIGHT][0] = enemy5_right_1;
                sprites[RIGHT][1] = enemy5_right_1;
                sprites[RIGHT][2] = enemy5_right_1;
                break;
        }
    }
};