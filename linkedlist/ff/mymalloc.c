#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

// setting up first node in LinkedList
TData *startingData = (TData*) malloc(sizeof(TData));
startingData->val = MEMSIZE;
startingData->isOccupied = false;
TNode *tn = make_node((unsigned int) 0, startingData);
insert_node(&_memlist, tn, 0);


// void insertNodeToLinkedList(size_t length, bool occupied, int startingAddress) {
    // abstract out creation of node, but low priority (only for cosmetic purposes)
// }

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
    bool isFound = false;
    TNode *currNode = _memlist;
    TData *td = NULL;
    while (!isFound && currNode != NULL) {
        td = currNode->pdata;
        if (td->val >= size && td->isOccupied == false) {
            isFound = true;
            break;
        }
        currNode = succ(currNode);
    }
    if (isFound == true) {
        // delete node
        size_t remainingSize = td->val - size;
        int startAddress = currNode->key;
        free(td);
        delete_node(&_memlist, currNode);

        // split node and insert
        TData *firstData = (TData*) malloc(sizeof(TData));
        firstData->val = size;
        firstData->isOccupied = true;
        TNode *firstNode = make_node((unsigned int) startAddress, firstData);
        insert_node(&_memlist, firstNode, 0);

        TData *secondData = (TData*) malloc(sizeof(TData));
        secondData->val = remainingSize;
        secondData->isOccupied = false;
        int secondStartAddress = startAddress + size;
        TNode *secondNode = make_node((unsigned int) secondStartAddress, secondData);
        insert_node(&_memlist, secondNode, 0);
    }
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
}

