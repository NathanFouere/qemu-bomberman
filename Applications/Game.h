#include <drivers/timer.h>
#include <drivers/Clavier.h>
#include <Applications/Entity/Bot.h>
#include <Applications/Entity/Player.h>
#include <Applications/Board/Board.h>

#define MAX_BOTS 5
#define TARGET_FPS 60
#define TIME_LIMIT 200

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 11

enum class GameState {
    INIT,
    RUNNING,
    GAME_OVER,
    GAME_WIN,
    GAME_RESTART,
    GAME_RESET
};

class Game {
public:
    Game(Clavier* k);
    void init();
    void update();
    void render();
    void run();

    bool displayFPS = true;
    bool multiplayerMode = false;

private:
    Clavier* clavier;

    Player* player1;
    Player* player2;

    Bot* bots[MAX_BOTS];
    Board* board;

    int timeRemaining = TIME_LIMIT;

    GameState gameState = GameState::INIT;

    unsigned long lastFrameTime;
    const int targetFrameTime = 1000 / TARGET_FPS;

    void checkHitBomb(Movable* movable);
    void checkPlayerHitBot(Player* player);

    void resetGame();
    void restartGame();
};