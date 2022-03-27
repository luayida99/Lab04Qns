#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"

char _heap[MEMSIZE] = {0};

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
    // Implement this to call print_map from bitmap.c
    print_map(_heap, MEMSIZE/8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    int idx = search_map(_heap,MEMSIZE/8,size);
    if (idx>=0) {
        allocate_map(_heap, idx, size);
        return &_heap[idx];//check, idk???
    }
    return NULL;
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long idx=get_index(ptr);
    printf("Pointer:%p\n",ptr);
    printf("Index:%ld\n",idx);
    if (idx>=0) {
        free_map(_heap, get_index(ptr), 4);//how get size to free??
    }
}
