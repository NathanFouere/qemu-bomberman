#include "Game.h"
#include <Applications/Utilities/PseudoRand.h>
#include <Applications/Board/Bomb.h>

Game::Game(Clavier* k) :clavier(k), player1(nullptr), player2(nullptr), board(nullptr) {
    for (int i = 0; i < MAX_BOTS; ++i) {
        bots[i] = nullptr;
    }
    lastFrameTime = 0;
    lastFPSUpdate = 0;
    frameCount = 0;
    currentFPS = 0;
}

void Game::init() {
    set_vga_mode13();
    clear_vga_screen(228);
    set_palette_vga(palette_vga);    
    
    // Display "STAGE 1" message
    clear_frame_buffer(0); // Black background
    draw_text("STAGE 1", 135, 100, 15); // Centered text
    copy_frame_buffer_to_video();
    
    // Sleep for 2 seconds
    Timer& stageTimer = Timer::getInstance();
    unsigned long startStageTime = stageTimer.getTicks();
    while (stageTimer.getTicks() - startStageTime < 2000) {
        thread_yield(); // Yield to other threads while waiting
    }
    
    clear_vga_screen(228);
    
    board = new Board(BOARD_WIDTH, BOARD_HEIGHT);

    player1 = new Player(8, 40, PlayerType::PLAYER1, clavier, board);
    player1->start();

    if (multiplayerMode){
        player2 = new Player(24, 40, PlayerType::PLAYER2, clavier, board);
        player2->start();
    }
    
    Timer& timer = Timer::getInstance();
    unsigned long startTime = timer.getTicks();
    
	for (int i = 0; i < MAX_BOTS; i++) {
        constexpr int EnemyTypeCount = 5;
        while (true) {
            int randX = pseudoRand() % (BOARD_WIDTH - 2) + 1;
            int randY = pseudoRand() % (BOARD_HEIGHT - 2) + 1;

            int px = randX * TILE_SIZE + BOARD_ORIGIN_X;
            int py = randY * TILE_SIZE + BOARD_ORIGIN_Y;

            if (!board->isBlockedAt(px, py)) {
                bots[i] = new Bot(px, py, static_cast<EnemyType>(i % EnemyTypeCount), board);
                bots[i]->start();
                
                // Add small delay between bot thread creation to prevent synchronization issues
                startTime = timer.getTicks();
                while (timer.getTicks() - startTime < 20) {
                    thread_yield();
                }
                break;
            }
        }
    }

    lastFrameTime = Timer::getInstance().getTicks();
    state = GameState::RUNNING;
}

void Game::update() {
    // Check game win/lose conditions
    checkGameConditions();
    // thread_yield();

    for (int i = 0; i < 10; i++) {
        thread_yield();
        
        // Small random delay between yields
        unsigned long startTime = Timer::getInstance().getTicks();
        unsigned long delayTime = (startTime % 5) + 1;  // 1-5ms variation
        
        while (Timer::getInstance().getTicks() - startTime < delayTime) {
        }
    }
}

void Game::checkGameConditions() {
    if (timeRemaining <= 0) {
        gameOver();
        return;
    }
    
    if (player1->getStatus() == EntityStatus::DEAD) {
        if (player1->getLives() <= 0) {
            gameOver();
            return;
        }
    }
    
    if (multiplayerMode && player2->getStatus() == EntityStatus::DEAD) {
        if (player2->getLives() <= 0) {
            gameOver();
            return;
        }
    }
    
    bool allBotsDead = true;
    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i] && bots[i]->getStatus() != EntityStatus::DEAD) {
            allBotsDead = false;
            break;
        }
    }
    
    if (allBotsDead) {
        win();
    }
}

void Game::render() {
    clear_frame_buffer(228);

    plot_rectangle(0, 0, 24, 320, 5);
    draw_text("TIME", 4, 9, 15);
    draw_number(timeRemaining, 68, 9, 15);

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
        if (bots[i] && bots[i]->getStatus() != EntityStatus::DEAD) {
            bots[i]->render();
        }
    }

    if (player1->getStatus() != EntityStatus::DEAD) {
        player1->render();
    }

    if (multiplayerMode && player2->getStatus() != EntityStatus::DEAD) {
        player2->render();
    }

    if (displayFPS) {
        unsigned long current = Timer::getInstance().getTicks();
        frameCount++;
        
        if (current - lastFPSUpdate >= 1000) {
            currentFPS = frameCount;
            frameCount = 0;
            lastFPSUpdate = current;
        }
        
        draw_text("FPS: ", 268, 188, 255);
        draw_number(currentFPS, 316, 188, 255);
    }

    copy_frame_buffer_to_video();
}

void Game::waitForNextFrame(unsigned long frameStart) {
    unsigned long elapsed = Timer::getInstance().getTicks() - frameStart;
    if (elapsed < targetFrameTime) {
        unsigned long sleepTime = targetFrameTime - elapsed;
        Timer& timer = Timer::getInstance();
        unsigned long start = timer.getTicks();
        
        // Divide the waiting time into multiple yield calls
        // This gives more opportunities for bot threads to run
        const int yieldPoints = 10;
        unsigned long timePerYield = sleepTime / yieldPoints;
        
        for (int i = 0; i < yieldPoints && (timer.getTicks() - start < sleepTime); i++) {
            // Yield several times at each point
            for (int j = 0; j < 3; j++) {
                thread_yield();
            }
            
            // Wait for the allocated time slice
            unsigned long sliceStart = timer.getTicks();
            while (timer.getTicks() - sliceStart < timePerYield && 
                   timer.getTicks() - start < sleepTime) {
                // Just wait a bit
            }
        }
    }
}

void Game::run() {
    while (state != GameState::GAME_OVER && state != GameState::WIN) {
        // if (state == GameState::PAUSED) {
        //     // When paused, just yield to other threads and check for unpause
        //     char c = 0;
        //     if (clavier->testChar()) {
        //         c = clavier->getchar();
        //         if (c == 'p') { // "p" to unpause
        //             resume();
        //         }
        //     }
        //     thread_yield();
        //     continue;
        // }

        // Capture frame start time
        frameStartTime = Timer::getInstance().getTicks();
        
        // if (clavier->testChar()) {
        //     char c = clavier->getchar();
        //     if (c == 'p'   //         continue;
        //     }
        //     if (c == 'r') {
        //         restart();
        //         continue;
        //     }
        // }

        update();
        render();

        // Update time remaining
        timeRemaining = TIME_LIMIT - Timer::getInstance().getSeconds();
        // Wait for next frame to maintain target FPS
        waitForNextFrame(frameStartTime);
    }
}

void Game::pause() {
    if (state == GameState::RUNNING) {
        state = GameState::PAUSED;
    }
}

void Game::resume() {
    if (state == GameState::PAUSED) {
        state = GameState::RUNNING;
    }
}

void Game::gameOver() {
    state = GameState::GAME_OVER;
    
    // Display game over screen
    clear_frame_buffer(0); // Black background
    draw_text("GAME OVER", 120, 100, 15); // Centered text
    copy_frame_buffer_to_video();
    
    // Wait for a few seconds
    Timer& timer = Timer::getInstance();
    unsigned long startTime = timer.getTicks();
    while (timer.getTicks() - startTime < 3000) {
        thread_yield();
    }
}

void Game::win() {
    state = GameState::WIN;
    
    // Display win screen
    clear_frame_buffer(0); // Black background
    draw_text("YOU WIN!", 130, 100, 15); // Centered text
    copy_frame_buffer_to_video();
    
    // Wait for a few seconds
    Timer& timer = Timer::getInstance();
    unsigned long startTime = timer.getTicks();
    while (timer.getTicks() - startTime < 3000) {
        thread_yield();
    }
}

void Game::restart() {
    state = GameState::INITIALIZED;
    init();
}
