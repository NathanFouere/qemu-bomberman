#include <hal/multiboot.h>
#include <drivers/Ecran.h>
//#include <Applications/SuperPong/SuperPong.h>
#include <drivers/PortSerie.h>

// TP2
#include <sextant/interruptions/idt.h>
#include <sextant/interruptions/irq.h>
#include <sextant/interruptions/handler/handler_tic.h>
#include <sextant/interruptions/handler/handler_clavier.h>
#include <drivers/timer.h>
#include <drivers/Clavier.h>

// TP3
#include <sextant/memoire/Memoire.h>

// TP4
#include <sextant/ordonnancements/cpu_context.h>
#include <sextant/ordonnancements/preemptif/thread.h>
#include <sextant/types.h>

// TP5
#include <sextant/Synchronisation/Spinlock/Spinlock.h>

// TP6
#include <sextant/memoire/segmentation/gdt.h>
#include <sextant/memoire/Memoire.h>
#include <sextant/memoire/pagination/MemoirePhysique.h>
#include <sextant/memoire/pagination/Pagination.h>
#include <drivers/vga.h>
#include <drivers/sprite.h>

#include <Applications/Entity/Bot.h>
#include <Applications/Entity/Player.h>
#include <Applications/Board/Board.h>
#include <Applications/Utilities/Vector.h>
#include <Applications/Board/Bomb.h>
// tile

extern char __e_kernel,__b_kernel, __b_data, __e_data,  __b_stack, __e_load ;
int i;

extern vaddr_t bootstrap_stack_bottom; //Adresse de début de la pile d'exécution
extern size_t bootstrap_stack_size;//Taille de la pile d'exécution

Timer timer;
//paddr_t kernel_core_base,kernel_core_top;
unsigned long address;

struct cpu_state *ctxt_hello1;
struct cpu_state *ctxt_hello2;
struct cpu_state *ctxt_main;
vaddr_t hello1_stack, hello2_stack;
char tab1[4096];
char tab2[4096];
char tab[30000];

Ecran ecran;


Ecran *monEcran = &ecran;


#define PAGINATION_USE 1


void Sextant_Init(){
	idt_setup();
	irq_setup();

	//Initialisation de la frequence de l'horloge
	timer.i8254_set_frequency(1000);
	irq_set_routine(IRQ_TIMER, ticTac);
	asm volatile("sti\n");//Autorise les interruptions

	irq_set_routine(IRQ_KEYBOARD, handler_clavier);

	memory_init();

	multiboot_info_t* mbi;
	mbi = (multiboot_info_t*)address;

	gdt_setup();

	thread_subsystem_setup(bootstrap_stack_bottom,bootstrap_stack_size);

	sched_subsystem_setup();
	//irq_set_routine(IRQ_TIMER, sched_clk);
}

char* itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) {
        *str = '\0'; // Invalid base
        return str;
    }
    char* ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return str;
}

extern "C" void Sextant_main(unsigned long magic, unsigned long addr) {
    Sextant_Init();
    Clavier clavier;
    address = addr;

    set_vga_mode13();
    clear_vga_screen(228);

    Bot *bot = new Bot(50, 50);
    Player *player = new Player(125, 125, &clavier);
    Board board(20, 11);

    player->start();
    bot->start();

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

		draw_text("Hello World !", 100, 100, 255);
	
		draw_sprite(player->getSprite(), 16, 16, player->getX(), player->getY());
		draw_sprite(bot->getSprite(), 16, 16, bot->getX(), bot->getY());
	
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
