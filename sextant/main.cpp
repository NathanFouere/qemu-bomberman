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

#include <drivers/vga.h>
#include <Applications/Utilities/PseudoRand.h>

extern char __e_kernel,__b_kernel, __b_data, __e_data,  __b_stack, __e_load ;
int i;

extern vaddr_t bootstrap_stack_bottom;
extern size_t bootstrap_stack_size;

unsigned long address;

char tab1[4096];
char tab2[4096];
char tab[30000];

#define PAGINATION_USE 1


void Sextant_Init(){
	// Interruptions init
	idt_setup();
	irq_setup();

	Timer::getInstance().i8254_set_frequency(1000);
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

    Game game(&clavier);

	game.displayFPS = true;
	game.multiplayerMode = false;

    game.init();
    game.run();
}
