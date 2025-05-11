#include <hal/multiboot.h>
#include <drivers/Ecran.h>
#include <drivers/PortSerie.h>

#include <sextant/interruptions/idt.h>
#include <sextant/interruptions/irq.h>
#include <sextant/interruptions/handler/handler_tic.h>
#include <sextant/interruptions/handler/handler_clavier.h>
#include <drivers/timer.h>
#include <drivers/Clavier.h>

#include <sextant/memoire/segmentation/gdt.h>
#include <sextant/memoire/Memoire.h>

#include <sextant/ordonnancements/cpu_context.h>
#include <sextant/ordonnancements/preemptif/thread.h>
#include <sextant/types.h>

#include <Applications/Game.h>

extern char __e_kernel,__b_kernel, __b_data, __e_data,  __b_stack, __e_load ;
int i;

extern vaddr_t bootstrap_stack_bottom;
extern size_t bootstrap_stack_size;

Timer timer;
unsigned long address;

char tab1[4096];
char tab2[4096];
char tab[30000];

#define PAGINATION_USE 1


void Sextant_Init(){
	// Interruptions init
	idt_setup();
	irq_setup();

	timer.i8254_set_frequency(1000);
	irq_set_routine(IRQ_TIMER, ticTac);
	asm volatile("sti\n");//Autorise les interruptions

	irq_set_routine(IRQ_KEYBOARD, handler_clavier);

	// Memoire init
	memory_init();
	gdt_setup();

	multiboot_info_t* mbi;
	mbi = (multiboot_info_t*)address;

	// Thread init
	thread_subsystem_setup(bootstrap_stack_bottom,bootstrap_stack_size);
	sched_subsystem_setup();

	// VGA init
	set_vga_mode13();
    clear_vga_screen(228);
	set_palette_vga(palette_vga);
}


extern "C" void Sextant_main(unsigned long magic, unsigned long addr) {
	Sextant_Init();

    Clavier clavier;
    Timer timer;

    Game game(&timer, &clavier);

    Board board(20, 11);
    Bot *bot = new Bot(50, 50, &board);
    Player *player = new Player(40, 50, &clavier, &board);

    game.init();
    game.run();

    const int targetFrameTime = 1000 / 60; // 
    unsigned long lastFrameTime = timer.getTicks();
    char fpsStr[20]; // Buffer to hold the FPS string

	while (true) {
		unsigned long frameStart = timer.getTicks(); // Start of the frame
	
		set_palette_vga(palette_vga);
		clear_frame_buffer(228); // Clear the frame buffer with the background color
	
		// Calculate FPS
		unsigned long frameTime = frameStart - lastFrameTime;
		lastFrameTime = frameStart;
		int fps = (frameTime > 0) ? (1000 / frameTime) : 0; // Avoid division by zero 
                                                                                                                                                                                                                                                                                                   	
		// Convert FPS to string and display
		itoa(fps, fpsStr, 10);
		draw_text("FPS: ", 10, 10, 255); // Display "FPS: " label
		draw_text(fpsStr, 50, 10, 255); // Display FPS value next to the label
	    board.draw();

		// draw_text("Hello World !", 100, 100, 255);
	
		draw_sprite(player->getSprite(), 16, 16, player->getX(), player->getY());
		// draw_sprite(bot->getSprite(), 16, 16, bot->getX(), bot->getY());
	
		// Copy the frame buffer to video memory
		copy_frame_buffer_to_video();
	
		thread_yield();
	
		// Calculate elapsed frame time
		unsigned long elapsedFrameTime = timer.getTicks() - frameStart;
	
		// Busy-wait if the frame finished too early
		while (elapsedFrameTime < targetFrameTime) {
			elapsedFrameTime = timer.getTicks() - frameStart;
		}
	}
}
