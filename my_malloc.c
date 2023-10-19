#include <stdio.h>
#include <stdlib.h>

// Define the structure of a block in the free list
typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

// Define the head of the free list
static Block* free_list = NULL;

// Simple function to split a block if it's larger than needed
static void split_block(Block* block, size_t size) {
    if (block->size > size + sizeof(Block)) {
        Block* new_block = (Block*)((char*)block + size + sizeof(Block));
        new_block->size = block->size - size - sizeof(Block);
        new_block->next = block->next;
        block->size = size;
        block->next = new_block;
    }
}

// Implementation of malloc using a free list
void* my_malloc(size_t size) {
    if (size == 0) return NULL;

    // Ensure size is a multiple of the block size for simplicity
    size = (size + sizeof(Block) - 1) / sizeof(Block) * sizeof(Block);

    // Traverse the free list to find a suitable block
    Block* prev = NULL;
    Block* current = free_list;
    while (current != NULL) {
        if (current->size >= size) {
            if (prev == NULL) {
                // Update the head of the free list
                free_list = current->next;
            } else {
                prev->next = current->next;
            }
            split_block(current, size);
            return (void*)(current + 1); // Return a pointer just past the block header
        }
        prev = current;
        current = current->next;
    }

    // If no suitable block was found, allocate a new one
    current = (Block*)malloc(size + sizeof(Block));
    if (current == NULL) return NULL;
    current->size = size;
    return (void*)(current + 1); // Return a pointer just past the block header
}

// Implementation of free using a free list
void my_free(void* ptr) {
    if (ptr == NULL) return;

    // Point back to the block header
    Block* block = (Block*)((char*)ptr - sizeof(Block);
    block->next = free_list;
    free_list = block;
}

int main() {
    int* a = (int*)my_malloc(sizeof(int));
    int* b = (int*)my_malloc(sizeof(int));
    
    if (a != NULL && b != NULL) {
        *a = 42;
        *b = 10;
        printf("a = %d, b = %d\n", *a, *b);
        
        my_free(a);
        my_free(b);
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
}