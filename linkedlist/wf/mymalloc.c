#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

void print_memlist() {
    reset_traverser(_memlist, 0);
    TNode *currNode = _memlist;
    while (currNode != NULL) {
        if (currNode->pdata->isOccupied) {
            printf("Status: ALLOCATED Start index: %d Length: %d\n", currNode->key, currNode->pdata->val);
        } else {
            printf("Status: FREE Start index: %d Length: %d\n", currNode->key, currNode->pdata->val);
        }
        currNode = currNode->next;
    }
    
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
    TNode *worstNode = NULL;
    int largestFit;
    TData *td = NULL;
    while (currNode != NULL) {
        td = currNode->pdata;
        if (td->isOccupied == false && size <= td->val) {
            if (isFound == false) {
                isFound = true;
                worstNode = currNode;
                largestFit = td->val;
            }
            if (td->val > largestFit) {
                worstNode = currNode;
                largestFit = td->val;
            }
        }
        currNode = currNode->next;
    }
    if (isFound == true) {
        int startingAddress = worstNode->key;
        td = worstNode->pdata;
        // mutate node
        size_t remainingSize = td->val - size;
        // printf("Best Node - Start index: %d Length: %d\n", worstNode->key, worstNode->pdata->val);
        worstNode->pdata->val = size;
        worstNode->pdata->isOccupied = true;

        // split node and insert
        if (remainingSize != 0) {
            TData *nextData = (TData*) malloc(sizeof(TData));
            nextData->val = remainingSize;
            nextData->isOccupied = false;
            int nextAddress = worstNode->key + size;
            TNode *nextNode = make_node((unsigned int) nextAddress, nextData);
            reset_traverser(_memlist, 0);
            insert_node(&_memlist, nextNode, 0);
        }

        return &_heap[startingAddress];

        // printf("Next Address: %d\n", nextAddress);
        // printf("======================================\n");
        // printf("First Node - Start index:%d Length:%d\n", currNode->key, currNode->pdata->val);
        // printf("First node memory address: %p\n", currNode);
        // printf("======================================\n");
        // printf("Second Node - Start index:%d Length:%d\n", nextNode->key, nextNode->pdata->val);
        // printf("Second node memory address: %p\n", nextNode);
        // printf("======================================\n");
    }
    return NULL;
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long idx = get_index(ptr);
    if (idx>=0) {
        // unassign node
        TNode *currNode = find_node(_memlist, (unsigned int) idx);

        if (currNode != NULL) {
            currNode->pdata->isOccupied = false;

            // merge nodes
            if (currNode->next != NULL && currNode->next->pdata->isOccupied == false) {
                currNode->pdata->val = currNode->pdata->val + currNode->next->pdata->val;
                free(currNode->next->pdata);
                merge_node(_memlist, currNode, 1);
            }

            if (currNode->prev != NULL && currNode->prev->pdata->isOccupied == false) {
                currNode->prev->pdata->val = currNode->pdata->val + currNode->prev->pdata->val;
                free(currNode->pdata);
                merge_node(_memlist, currNode, 0);
            }
        }
    }
}

