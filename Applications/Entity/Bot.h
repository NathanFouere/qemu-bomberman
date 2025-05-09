#include "Applications/Entity/Movable.h"
#include "drivers/Clavier.h"
#include "sextant/Activite/Threads.h"
#include <drivers/sprite.h>

class Bot: public Threads, public Movable {
    public:
      int it = 0;
    void moveRandom();
    void run();
    Bot(int x, int y)
        : Movable(x, y) {
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
        }
};
