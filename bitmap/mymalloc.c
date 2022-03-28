#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
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
    print_map(_heap, MEMSIZE/8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    int idx = search_map(_heap,MEMSIZE/8,size);
    TData td = {.len = size}; // length data
    if (idx>=0) {
        allocate_map(_heap, idx, size);
        TNode *tn = make_node((unsigned int) &_heap[idx], &td);
        insert_node(&_memlist, tn, 0);
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
        TNode *tn = find_node(_memlist, (unsigned int) idx);
        printf("%p", tn);
        if (tn == NULL) {
            printf("I fked up\n");
        }
        int length = 4;
        // int length = tn->pdata->len;
        // free(tn->pdata);
        // delete_node(&_memlist, tn);
        free_map(_heap, get_index(ptr), length);//how get size to free??
    }
}
