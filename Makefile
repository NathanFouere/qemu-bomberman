## Nous recepurons le nom de votre systeme d'exploitation
## Pour windows : pensez a sauvegarder echo.exe et uname.exe dans c:\sextant\ccompil
## Pour Mac : copiez les fichiers :
##    sudo cp /bin/echo /opt/local/bin
##	  sudo cp /usr/bin/uname /opt/local/bin


OS=$(shell uname)
COMPILATEUR=CPP_$(OS)
LIEUR=LD_$(OS)
COMPILATEUR_OPTION=COMPOP_$(OS)
LIEUR_OPTION=LIEUR_$(OS)

#-------------------
##Pour Mac
CPP_Darwin=i386-elf-g++-4.3.2
LD_Darwin=i386-elf-ld

#-------------------
##Pour Linux
CPP_Linux=g++
LD_Linux=ld
COMPOP_Linux=-fno-stack-protector -m32
LIEUR_Linux=-m elf_i386
QEMU=qemu-system-i386 

#-------------------
##POUR WINDOWS
CPP_WindowsNT=g++
LD_WindowsNT=ld
 
#-------------------
## Partie commune a toutes les configurations

CPPFLAGS  = -std=c++11 -gdwarf-2 -g3 -Wall -fno-builtin -fno-rtti -fno-exceptions -nostdinc -fno-threadsafe-statics $($(COMPILATEUR_OPTION))
LDFLAGS = --warn-common -nostdlib -Lbuild/all-o/ $($(LIEUR_OPTION))

#-------------------
## Explication des options de compilation 
# -fno-threadsafe-statics : If the function can only be called by one thread of execution then set the compiler additional option -fno-threadsafe-statics. If the function can be called by multiple threads of execution then you will need to implement a resource lock/unlock when __cxa_guard_acquire/__cxa_guard-release are called.

PWD :=.
DELE = rm -rf
MV = mv -f

#-------------------
## Debut des regles de generation de l'executable

KERNEL_OBJ = build/boot/sextant.elf


OBJECTSNAMES=\
 Cons Clavier cpu_context_switch cpu_context\
 Ecran\
 fonctionsES\
 gdt \
 Horloge handler_tic handler_clavier Hello\
 InfiniteHello irq_wrappers i8259 idt irq\
 Memoire multiboot Mutex MemoirePion MemoireListe main\
 op_memoire\
 Pipeline  Prod PortSerie\
 Semaphore Spinlock  sched\
 timer TestAndSet Threads thread\
 

OBJECTS=$(patsubst %,build/all-o/%.o,$(OBJECTSNAMES))					  		

#variable pour demander à make de chercher les dépendances dans n'importe quel répertoire jusqu'à 3 rep de profondeur :
VPATH=$(wildcard *):$(wildcard */*):$(wildcard */*/*)

# les target all et clean ne sont pas "constructibles" mais appellent des recettes :
.PHONY:all clean run show

# Main target
all: $(KERNEL_OBJ)

$(KERNEL_OBJ): $(OBJECTS)
	$($(LIEUR)) $(LDFLAGS) -T ./support/sextant.lds -o $@ $(OBJECTS)

#compiler tout .cpp dans les repertoires de sources en .o dans le build/all-o . On cree d'abord le repertoire build/all-o s'il n'existe pas.
build/all-o/%.o:%.cpp %.h
	$($(COMPILATEUR)) -I$(PWD) -c $< $(CPPFLAGS) -o $@

# la meme que precedement, si pas de .h correspondant on compile quand meme.
build/all-o/%.o:%.cpp
	$($(COMPILATEUR)) -I$(PWD) -c $< $(CPPFLAGS) -o $@
	
#meme regle que precedente pour les fichiers .s
build/all-o/%.o: %.S
	$($(COMPILATEUR)) -I$(PWD)  -c $< $(CPPFLAGS) -DASM_SOURCE=1 -o $@


# Clean directory
clean:
	$(DELE) build/all-o/*.o

run: $(KERNEL_OBJ)
	$(QEMU) -display curses -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso

run_gui: $(KERNEL_OBJ)
	$(QEMU) -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso

debug: $(KERNEL_OBJ)
	$(QEMU) -display curses -S -s  -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso

debug_gui: $(KERNEL_OBJ)
	$(QEMU) -S -s  -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso

show:
	@echo "objects:$(OBJECTS)"
	@echo "VPATHS:$(VPATH)"
	@echo "compilateur:"$($(COMPILATEUR))" ; lieur:"$($(LIEUR))
