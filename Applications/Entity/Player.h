#include "Applications/Entity/Movable.h"
#include "drivers/Clavier.h"
#include "sextant/Activite/Threads.h"
#include "../Board/Board.h"
#include "Direction.h"
#include <drivers/sprite.h>

enum class PlayerType {
    PLAYER1,
    PLAYER2
};

class Player: public Threads, public Movable {
    private:
        Clavier* clavier;
        int lives = 2;
        int score = 0;

        void setSprites(PlayerType type) {
            if (type == PlayerType::PLAYER1) {
                sprites[UP][0] = player1_back_1;
                sprites[UP][1] = player1_back_1;
                sprites[UP][2] = player1_back_1;
        
                sprites[DOWN][0] = player1_front_1;
                sprites[DOWN][1] = player1_front_1;
                sprites[DOWN][2] = player1_front_1;
        
                sprites[LEFT][0] = player1_left_1;
                sprites[LEFT][1] = player1_left_1;
                sprites[LEFT][2] = player1_left_1;
        
                sprites[RIGHT][0] = player1_right_1;
                sprites[RIGHT][1] = player1_right_1;
                sprites[RIGHT][2] = player1_right_1;
            } else
            {
            }
        }
    public:
        Board* board;
        Player(int x, int y, PlayerType type, Clavier* clavier, Board* boardPtr)
            : Movable(x, y), clavier(clavier), board(boardPtr) {
                setSprites(type);
            }
        void movePlayer();
        void run();
        void render();
        int getX() const { return x; }
        int getY() const { return y; }
        int getLives() const { return lives; }
        int getScore() const { return score; }
};
