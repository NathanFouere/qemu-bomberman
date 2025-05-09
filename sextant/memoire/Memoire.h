#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 4096

#include <drivers/Ecran.h>

struct Block {
    int size;       // Taille utile du bloc
    int free;       // 1 = libre, 0 = occupé
    Block* next;    // Bloc suivant dans la liste
};

extern char memory[MEMORY_SIZE];

// Liste chaînée des blocs libres/occupés
extern Block* free_list;

void memory_init();
void* alloc(int size);
void free(void* ptr);
void debug_memory(Ecran& e);

void* operator new(size_t taille);
void  operator delete(void* ptr);
#endif