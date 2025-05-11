#include "Memoire.h"
#include <drivers/Ecran.h>

char memory[MEMORY_SIZE];
Block* free_list = nullptr;

void memory_init() {
    free_list = (Block*)memory;
    free_list->size = MEMORY_SIZE - sizeof(Block);
    free_list->free = 1;
    free_list->next = nullptr;
}

void* alloc(int size) {
    int alignedSize = (size + 3) & ~3;

    Block* current = free_list;
    while (current) {
        if (current->free && current->size >= alignedSize) {
            if (current->size >= alignedSize + (int)sizeof(Block) + 1) {
                Block* new_block = (Block*)((char*)current + sizeof(Block) + alignedSize);
                new_block->size = current->size - alignedSize - sizeof(Block);
                new_block->free = 1;
                new_block->next = current->next;

                current->size = alignedSize;
                current->next = new_block;
            }
            current->free = 0;
            return (void*)((char*)current + sizeof(Block));
        }
        current = current->next;
    }
    return nullptr; 
}

void free(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->free = 1;

    // Fusion avec le bloc suivant si libre
    if (block->next && block->next->free) {
        block->size += sizeof(Block) + block->next->size;
        block->next = block->next->next;
    }

    // (Améliorable : fusion avec bloc précédent)
}

// Override global new operator
void* operator new(size_t size) {
    void* ptr = alloc(size);
    if (!ptr) {
        // Handle allocation failure (e.g., return nullptr or halt execution)
    }
    return ptr;
}

// Override global delete operator
void operator delete(void* ptr) noexcept {
    free(ptr);
}
