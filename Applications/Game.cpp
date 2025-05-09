#include "Game.h"
#include "Applications/Utilities/Utils.cpp"

Game::Game(Timer* t, Clavier* k) : timer(t), clavier(k), player(nullptr), board(nullptr) {
    for (int i = 0; i < MAX_BOTS; ++i) {
        bots[i] = nullptr;
    }
    lastFrameTime = 0;
}

void Game::init() {
    set_vga_mode13();
    clear_vga_screen(228);
    set_palette_vga(palette_vga);

    board = new Board(20, 11);
    player = new Player(125, 125, clavier);
    player->start();

    for (int i = 0; i < MAX_BOTS; ++i) {
        bots[i] = new Bot(40 + i * 30, 40 + i * 20); // Adjust spawn spacing
        bots[i]->start();
    }

    lastFrameTime = timer->getTicks();
}

void Game::update() {
    thread_yield(); // Yielding in case you're using preemptive threading
}

void Game::render() {
    clear_frame_buffer(228);

    unsigned long current = timer->getTicks();
    int fps = (current > lastFrameTime) ? (1000 / (current - lastFrameTime)) : 0;
    lastFrameTime = current;

    char fpsStr[20];
    itoa(fps, fpsStr, 10);

    plot_rectangle(0, 0, 24, 320, 5);
    

    board->draw();
    draw_sprite(player->getSprite(), 16, 16, player->getX(), player->getY());

    for (int i = 0; i < MAX_BOTS; ++i) {
        if (bots[i]) {
            draw_sprite(bots[i]->getSprite(), 16, 16, bots[i]->getX(), bots[i]->getY());
        }
    }

    draw_text("FPS: ", 268, 188, 255);
    draw_text(fpsStr, 300, 188, 255);

    copy_frame_buffer_to_video();
}

void Game::run() {
    while (true) {
        unsigned long frameStart = timer->getTicks();
        update();
        render();

        unsigned long elapsed = timer->getTicks() - frameStart;
        while (elapsed < targetFrameTime) {
            elapsed = timer->getTicks() - frameStart;
        }
    }
}
