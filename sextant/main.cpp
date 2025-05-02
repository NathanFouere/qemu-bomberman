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
#include <Applications/Horloge/Horloge.h>
// TP3
#include <sextant/memoire/Memoire.h>

// TP4
#include <sextant/ordonnancements/cpu_context.h>
#include <sextant/ordonnancements/preemptif/thread.h>
#include <sextant/types.h>

// TP5
#include <Applications/HelloThread/Hello.h>
#include <Applications/HelloThread/InfiniteHello.h>

#include <sextant/Synchronisation/Spinlock/Spinlock.h>

#include <Applications/ProdCons/Prod.h>
#include <Applications/ProdCons/Cons.h>

// TP6
#include <sextant/memoire/segmentation/gdt.h>
#include <sextant/memoire/Memoire.h>
#include <sextant/memoire/MemoirePion.h>
//#include <sextant/memoire/pagination/memoireliste4k.h>
#include <sextant/memoire/pagination/MemoirePhysique.h>
#include <sextant/memoire/pagination/Pagination.h>
#include <Applications/Pipeline/Pipeline.h>

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

memoire *InterfaceMemoire;


Ecran *monEcran = &ecran;


#define PAGINATION_USE 1



void Sextant_Init(){


	idt_setup();
	irq_setup();
	//Initialisation de la frequence de l'horloge

	timer.i8254_set_frequency(1000);
//	irq_set_routine(IRQ_TIMER, ticTac);
	asm volatile("sti\n");//Autorise les interruptions

	irq_set_routine(IRQ_KEYBOARD, handler_clavier);

	multiboot_info_t* mbi;
	mbi = (multiboot_info_t*)address;

	gdt_setup();

	InterfaceMemoire=memoire::nouveau();

	InterfaceMemoire->mem_setup(& __e_kernel,(mbi->mem_upper<<10) + (1<<20),&ecran);

	thread_subsystem_setup(bootstrap_stack_bottom,bootstrap_stack_size);

	sched_subsystem_setup();
	irq_set_routine(IRQ_TIMER, sched_clk);
}



/*pour question 1*/
int globalCounter=14;
int positionCounter=0;


extern "C" void Sextant_main(unsigned long magic, unsigned long addr){
	Clavier clavier;
	void *temp1;
	address = addr;



	Sextant_Init();


	ecran.effacerEcran(NOIR);

	/* Question 1 */

	/* Sous question 1 */

	Semaphore sem1;
	Pipeline tabPipelineQ1[8];



	/* Sous question 2 */

	globalCounter=14;
	positionCounter=0;

	Semaphore sem[8];
	Pipeline tabPipelineQ2[8];


	while(true);

	/* Question 2 et suivantes */

	InterfaceMemoire->memoireaffiche(&ecran);

	clavier.getchar(); /*1*/

	temp1=(void*)InterfaceMemoire->malloc(4093);
	InterfaceMemoire->memoireaffiche(&ecran);

	clavier.getchar(); /*2*/

	temp1=(void*)InterfaceMemoire->malloc(10);
	memcpy(temp1,"toto",5);
	InterfaceMemoire->memoireaffiche(&ecran);

	clavier.getchar(); /*3*/

	InterfaceMemoire->free((vaddr_t)temp1);
//	memcpy(temp1,"toto",5);

	InterfaceMemoire->memoireaffiche(&ecran);

	clavier.getchar(); /*4*/

	void *temp2,*temp3;

	temp1=(void*)InterfaceMemoire->malloc(10);
	InterfaceMemoire->memoireaffiche(&ecran);
	temp2=(void*)InterfaceMemoire->malloc(10);
	InterfaceMemoire->memoireaffiche(&ecran);
	temp3=(void*)InterfaceMemoire->malloc(10);
	InterfaceMemoire->memoireaffiche(&ecran);

	clavier.getchar(); /*5*/

	InterfaceMemoire->free((vaddr_t)temp2);
	InterfaceMemoire->memoireaffiche(&ecran);

	clavier.getchar(); /*6*/

	temp2=(void*)InterfaceMemoire->malloc(10);
	InterfaceMemoire->memoireaffiche(&ecran);

	clavier.getchar(); /*7*/

	Semaphore *sem3;
	sem3 = new Semaphore(1);
	InterfaceMemoire->memoireaffiche(&ecran);
	clavier.getchar(); /*8*/

	delete sem3;
	InterfaceMemoire->memoireaffiche(&ecran);
	clavier.getchar(); /*9*/

	monEcran->effacerEcran(NOIR);
	InterfaceMemoire->test();
	clavier.getchar(); /*10*/

	while (true);

}
