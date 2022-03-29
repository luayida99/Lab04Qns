#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

// setting up first node in LinkedList
//TData *startingData = (TData*) malloc(sizeof(TData));
//startingData->val = MEMSIZE;
//startingData->isOccupied = false;
//TNode *tn = make_node((unsigned int) 0, startingData);
//insert_node(&_memlist, tn, 0);

void print_memlist() {
    //sth wrong try running
    reset_traverser(_memlist, 0);
    TNode *currNode = _memlist;
    int length = 0;
    while (currNode->next != NULL) {
        printf("Status: ALLOCATED Start index:%d Length:%d\n", currNode->key, currNode->pdata->val);
        currNode = currNode->next;
        length++;
    }
    printf("Status: FREE Start index:%d Length:%d\n", currNode->key, currNode->pdata->val);
}

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    if (_memlist == NULL) {
        //check if memlist not instantiated and add starting node
        TData *startingData = (TData*) malloc(sizeof(TData));
        startingData->val = MEMSIZE;
        startingData->isOccupied = false;
        TNode *tn = make_node((unsigned int) 0, startingData);
        insert_node(&_memlist, tn, 0);
    }
    reset_traverser(_memlist, 0);
    bool isFound = false;
    TNode *currNode = _memlist;
    TData *td = NULL;
    while (!isFound && currNode != NULL) {
        td = currNode->pdata;
        if (td->val >= size && td->isOccupied == false) {
            isFound = true;
            break;
        }
        currNode = currNode->next;
    }
    if (isFound == true) {
        // mutate node
        size_t remainingSize = td->val - size;
        // printf("Current Node - Start index:%d Length:%d\n", currNode->key, currNode->pdata->val);
        currNode->pdata->val = size;
        currNode->pdata->isOccupied = true;

        // split node and insert
        TData *nextData = (TData*) malloc(sizeof(TData));
        nextData->val = remainingSize;
        nextData->isOccupied = false;
        int nextAddress = currNode->key + size;
        TNode *nextNode = make_node((unsigned int) nextAddress, nextData);
        reset_traverser(_memlist, 0);
        insert_node(&_memlist, nextNode, 0);

        // printf("Next Address: %d\n", nextAddress);
        // printf("======================================\n");
        // printf("First Node - Start index:%d Length:%d\n", currNode->key, currNode->pdata->val);
        // printf("First node memory address: %p\n", currNode);
        // printf("======================================\n");
        // printf("Second Node - Start index:%d Length:%d\n", nextNode->key, nextNode->pdata->val);
        // printf("Second node memory address: %p\n", nextNode);
        // printf("======================================\n");
    }
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    
}

