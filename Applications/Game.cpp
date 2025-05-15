#include "Game.h"
#include <Applications/Utilities/PseudoRand.h>
#include <Applications/Board/Bomb.h>
#include <sextant/Activite/ThreadManager.h>

Game::Game(Clavier* k) : clavier(k) {
    ServiceLocator::provideKeyboard(k);
    lastFrameTime = 0;
}

Game::~Game() {
    // Resources will auto-clean via RAII
}

 void Game::init() {
    set_vga_mode13();
    clear_vga_screen(16);
    set_palette_vga(palette_vga);
    
    // Reset the player hit time
    playerHitTime = 0;

    bool modeSelected = false;

    while (!modeSelected) {
        clear_frame_buffer(0);

        draw_text("BOMBERMAN", 120, 60, 15);
        draw_text("1. SOLO MODE", 100, 100, 15);
        draw_text("2. MULTIPLAYER", 100, 120, 15);
        draw_text("PRESS 1 OR 2 TO SELECT", 75, 160, 14);
        copy_frame_buffer_to_video();
        while (!clavier->testChar()) {
            thread_yield();
        }
        char key = clavier->getchar();
        if (key == '1') {
            multiplayerMode = false;
            modeSelected = true;
        } else if (key == '2') {
            multiplayerMode = true;
            modeSelected = true;
        }
    }

    // Display "STAGE 1"
    clear_frame_buffer(0);
    draw_text("STAGE 1", 135, 100, 15);
    copy_frame_buffer_to_video();

    unsigned int startStageTime = Timer::getInstance().getTicks();
    while (Timer::getInstance().getTicks() - startStageTime < 1500) {
        thread_yield();
    }

    board = new Board(BOARD_WIDTH, BOARD_HEIGHT);

    // Create player using Resource
    player1.reset(new Player(8, 40, PlayerType::PLAYER1, clavier, board.get()));
    player1->start();

    if (multiplayerMode) {
        player2.reset(new Player(8, 40, PlayerType::PLAYER2, clavier, board.get()));
        player2->start();
    }
    
    for (int i = 0; i < MAX_BOTS; i++) {
        constexpr int EnemyTypeCount = 5;
        while (true) {
            int randX = pseudoRand() % (BOARD_WIDTH - 2) + 1;
            int randY = pseudoRand() % (BOARD_HEIGHT - 2) + 1;

             int px = randX * TILE_SIZE + BOARD_ORIGIN_X;
            int py = randY * TILE_SIZE + BOARD_ORIGIN_Y;

             if (!board->isBlockedAt(px, py)) {
                bots[i] = new Bot(px, py, static_cast<EnemyType>(pseudoRand() % EnemyTypeCount), board.get());
                bots[i]->start();
                break;
            }
        }
    }

    lastFrameTime = Timer::getInstance().getTicks();
    gameStartTime = Timer::getInstance().getSeconds(); // Record when the game actually starts
}

 void Game::update() {
    // Get current time
    unsigned long currentTime = Timer::getInstance().getTicks();
    
    bool gameIsFrozen = (playerHitTime > 0 && (currentTime - playerHitTime) < FREEZE_DURATION);
    
    player1->update();
    if (multiplayerMode) {
        player2->update();
    }
    
    if (gameIsFrozen) {
        thread_yield();
        return;
    }
    
    this->checkHitBombPlayer(player1.get());
    this->checkPlayerHitBot(player1.get());
    
    if (multiplayerMode) {
        this->checkHitBombPlayer(player2.get());
        this->checkPlayerHitBot(player2.get());
    }
    
    for (int i = 0; i < MAX_BOTS; ++i) {
        this->checkHitBombBot(bots[i].get());
        bots[i]->update();
    }

    checkGameWinAndLose();
    ThreadManager::getInstance().yieldThread();
}

 void Game::checkGameWinAndLose(){
    if (player1->getLives() <= 0) {
        gameState = GameState::GAME_OVER;
    }
    if (multiplayerMode && player2->getLives() <= 0 && player1->getLives() <= 0) {
        gameState = GameState::GAME_OVER;
    }

    if (timeRemaining <= 0) {
        gameState = GameState::GAME_OVER;
    }

    bool allBotsDead = true;
    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i].get() != nullptr && bots[i]->getStatus() != EntityStatus::DEAD) {
            allBotsDead = false;
            break;
        }
    }
    if (allBotsDead) {
        gameState = GameState::GAME_WIN;
    }
}

 void Game::render() {
    unsigned long currentTime = Timer::getInstance().getTicks();
    bool playerJustHit = (playerHitTime > 0 && (currentTime - playerHitTime) < FREEZE_DURATION);

    clear_frame_buffer(228);

    plot_rectangle(0, 0, 24, 320, 5);
    draw_text("TIME", 4, 9, 15);
    draw_number(timeRemaining, 68, 9, 15);

     // TODO get score + lives from player
    if (multiplayerMode){
        draw_number(player1->getScore(), 150, 9, 153);
        draw_number(player2->getScore(), 200, 9, 16);

        draw_text("LEFT", 250, 9, 15);
        draw_number(player1->getLives(), 300, 9, 153);
        draw_number(player2->getLives(), 312, 9, 16);

     } else {
        draw_number(player1->getScore(), 200, 9, 15);
        draw_text("LEFT", 250, 9, 15);
        draw_number(player1->getLives(), 300, 9, 15);
    }
    
    board->draw();

    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i].get() != nullptr && bots[i]->getStatus() != EntityStatus::DEAD) {
            draw_sprite(bots[i]->getSprite(), 16, 16, bots[i]->getX(), bots[i]->getY());
        }
    }

     if (player1->getStatus() != EntityStatus::DEAD) {
        draw_sprite(player1->getSprite(), 16, 16, player1->getX(), player1->getY());
    }

     if (multiplayerMode && player2->getStatus() != EntityStatus::DEAD) {
        draw_sprite(player2->getSprite(), 16, 16, player2->getX(), player2->getY());
    }

     if (displayFPS) {
        unsigned long current = Timer::getInstance().getTicks();
        int fps = (current > lastFrameTime) ? (1000 / (current - lastFrameTime)) : 0;
        lastFrameTime = current;
        
        draw_text("FPS: ", 268, 188, 255);
        draw_number(fps, 316, 188, 255);
    }
    
    // Show hit message when player is hit
    if (playerJustHit) {
        draw_text("HIT!!", 140, 99, 0);
        draw_text("HIT!!", 141, 100, 0);
        draw_text("HIT!!", 140, 101, 0);
        draw_text("HIT!!", 139, 100, 0);
        draw_text("HIT!!", 140, 100, 15);
    }

    copy_frame_buffer_to_video();
}

 void Game::run() {
    while (true) {
        if (gameState == GameState::GAME_OVER) {
            clear_frame_buffer(0);
            draw_text("GAME OVER", 120, 80, 15);
            draw_text("PRESS ENTER TO RESTART", 70, 100, 15);
            copy_frame_buffer_to_video();

            while (!clavier->testChar()) {
                thread_yield();
            }
            char key = clavier->getchar();
            if (key == '\r' || key == '\n') {
                clear_frame_buffer(0);
                copy_frame_buffer_to_video();
                this->resetGame();
                continue; // Skip the rest of this loop iteration and start fresh
            }
        }
        else if (gameState == GameState::GAME_WIN) {
            clear_frame_buffer(0);
            draw_text("YOU WIN", 120, 60, 15);
            draw_text("PRESS ENTER TO RESTART", 80, 100, 15);
            copy_frame_buffer_to_video();

            while (!clavier->testChar()) {
                thread_yield();
            }
            char key = clavier->getchar();
            if (key == '\r' || key == '\n') {
                clear_frame_buffer(0);
                copy_frame_buffer_to_video();
                this->resetGame();
                continue; // Skip the rest of this loop iteration and start fresh
            }
        }

        unsigned long frameStart = Timer::getInstance().getTicks();

        update();
        render();

        // Calculate time based on the difference between current time and game start time
        timeRemaining = TIME_LIMIT - (Timer::getInstance().getSeconds() - gameStartTime);

        // Handle frame timing with ThreadManager
        unsigned long targetTime = frameStart + targetFrameTime;
        while (Timer::getInstance().getTicks() < targetTime) {
            ThreadManager::getInstance().yieldThread();
        }
    }
}



void Game::checkHitBombBot(Bot* movable) {
    int startX = movable->getX();
    int startY = movable->getY();
    int endX = startX + TILE_SIZE;
    int endY = startY + TILE_SIZE;

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            TileType tileType = this->board->getTileTypeAt(x, y);
            if (tileType == TILE_EXPLOSION) {
                movable->handleHitBomb();
                return;
            }
        }
    }
}

 void Game::checkPlayerHitBot(Player* player) {
    // Ne pas vérifier les collisions si le joueur est déjà dans une animation de mort
    if (player->getStatus() == EntityStatus::DEAD_ANIMATION) {
        return;
    }

    int playerX = player->getX();
    int playerY = player->getY();

     for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i].get() != nullptr && bots[i]->getStatus() != EntityStatus::DEAD) {

            int botX = bots[i]->getX();
            int botY = bots[i]->getY();

            if (playerX < botX + TILE_SIZE && playerX + TILE_SIZE > botX &&
                playerY < botY + TILE_SIZE && playerY + TILE_SIZE > botY) {
                player->handleHitBomb();
                playerHitTime = Timer::getInstance().getTicks(); // Record when player was hit by bot
                return;
            }
        }
    }
}

void Game::checkHitBombPlayer(Player* movable) {
    // Ne pas vérifier les collisions si le joueur est déjà dans une animation de mort
    // ou s'il est en période d'invulnérabilité après une réapparition
    if (movable->getStatus() == EntityStatus::DEAD_ANIMATION || 
        Timer::getInstance().getTicks() < playerHitTime + FREEZE_DURATION) {
        return;
    }

    int startX = movable->getX()  + 1; // obliger de faire + 1, il me semble que cela vient de la gestion des collisions
    int startY = movable->getY() + 1;
    int endX = startX + TILE_SIZE;
    int endY = startY + TILE_SIZE;

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            TileType tileType = this->board->getTileTypeAt(x, y);
            if (tileType == TILE_EXPLOSION) {
                movable->handleHitBomb();
                playerHitTime = Timer::getInstance().getTicks(); // Record when player was hit
                return;
            }
        }
    }
}

void Game::resetGame() {
    // Delete all bots
    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i].get()) {
            delete bots[i].get();
            bots[i] = nullptr;
        }
    }

     // Delete players
    if (player1.get()) {
        delete player1.get();
        player1 = nullptr;
    }

     if (player2.get()) {
        delete player2.get();
        player2 = nullptr;
    }

     // Delete board
    if (board.get()) {
        delete board.get();
        board = nullptr;
    }
    
    // Reset game state
    gameState = GameState::INIT;
    timeRemaining = TIME_LIMIT; // Reset time remaining
    playerHitTime = 0; // Reset player hit time
    
    // Reinitialize the game (this will also reset gameStartTime)
    init();
}

bool Game::isPlayerInDeathAnimation() const {
    if (player1.get() && player1.get()->getStatus() == EntityStatus::DEAD_ANIMATION) {
        return true;
    }
    
    if (multiplayerMode && player2.get() && player2.get()->getStatus() == EntityStatus::DEAD_ANIMATION) {
        return true;
    }
    
    return false;
}