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
    INITIALIZED,
    RUNNING,
    PAUSED,
    GAME_OVER,
    WIN
};

class Game {
public:
    Game(Clavier* k);

    // Game lifecycle
    void init();
    void update();
    void render();
    void run();
    
    // Game state
    void pause();
    void resume();
    void gameOver();
    void win();
    void restart();
    
    bool isRunning() const { return state == GameState::RUNNING; }
    GameState getState() const { return state; }

    bool displayFPS = true;
    bool multiplayerMode = false;

private:
    Clavier* clavier;

    Player* player1;
    Player* player2;

    Bot* bots[MAX_BOTS];
    Board* board;

    int timeRemaining = TIME_LIMIT;
    
    GameState state = GameState::INITIALIZED;
    bool GameRestart = false;
    bool GameReset = false;

    unsigned long lastFrameTime;
    unsigned long frameStartTime;
    const int targetFrameTime = 1000 / TARGET_FPS;
    
    int frameCount = 0;
    unsigned long lastFPSUpdate = 0;
    int currentFPS = 0;
    
    void waitForNextFrame(unsigned long frameStart);
    void checkGameConditions();
};
