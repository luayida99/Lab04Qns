#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
unsigned char map[MEMSIZE/8] = {0};
TNode *_memlist = NULL; // To maintain information about length

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
    print_map(map, MEMSIZE/8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    long idx = search_map(map,MEMSIZE/8,size);
    TData *td = (TData*) malloc(sizeof(TData));
    td->len = size;
    if (idx>=0) {
        allocate_map(map, idx, size);
        TNode *tn = make_node((unsigned int) idx, td);
        insert_node(&_memlist, tn, 0);
        return &_heap[idx];
    }
    return NULL;
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long idx=get_index(ptr);
    if (idx>=0) {
        TNode *tn = find_node(_memlist, (unsigned int) idx);
        if (tn != NULL) {
            size_t length = tn->pdata->len;
            free(tn->pdata);
            delete_node(&_memlist, tn);
            free_map(map, get_index(ptr), length);
        }
    }
}
