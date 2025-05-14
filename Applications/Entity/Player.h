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
        bool active = true;
        unsigned long lastMoveTime = 0;
        const int moveDelay = 10;

        void setSprites(PlayerType type) {
            if (type == PlayerType::PLAYER1) {
                sprites[UP][0] = player1_back_1;
                sprites[UP][1] = player1_back_2;
                sprites[UP][2] = player1_back_3;
        
                sprites[DOWN][0] = player1_front_1;
                sprites[DOWN][1] = player1_front_2;
                sprites[DOWN][2] = player1_front_3;
        
                sprites[LEFT][0] = player1_left_1;
                sprites[LEFT][1] = player1_left_2;
                sprites[LEFT][2] = player1_left_3;
        
                sprites[RIGHT][0] = player1_right_1;
                sprites[RIGHT][1] = player1_right_2;
                sprites[RIGHT][2] = player1_right_3;

                deathSprites[0] = player1_death_1;
                deathSprites[1] = player1_death_2;
                deathSprites[2] = player1_death_3;
                deathSprites[3] = player1_death_4;
                deathSprites[4] = player1_death_5;
                // Si le temps, gérer les autres sprites de morts
            } else
            {
            }
        }
    public:
        Board* board;
        Player(int x, int y, PlayerType type, Clavier* clavier, Board* boardPtr)
            : Movable(x, y), clavier(clavier), board(boardPtr) {
                setSprites(type);
                status = EntityStatus::ALIVE;
            }
        void movePlayer();
        void run();
        void render();
        int getX() const { return x; }
        int getY() const { return y; }
        int getLives() const { return lives; }
        int getScore() const { return score; }
        void setActive(bool isActive) { active = isActive; }
        bool isActive() const { return active; }
        void poseBomb();
        void addScore(int points) { score += points; }
        void decreaseLives() { if (lives > 0) lives--; }
        void handleHitBomb();
        void update();
};
