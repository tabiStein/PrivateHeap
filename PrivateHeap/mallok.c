/*
 * mallok.c
 * Tabitha Stein
 * TCSS 333 B - Spring 2015
 * Assignment 6
 * An abstract object that creates a pool of memory of the given size,
 * which can be used to create allocated blocks of memory on the heap
 * within this pool.
 */

#include 	"mallok.h"
#include 	<stdlib.h>
#include	<stdio.h>
#define 	true	1
#define		false 	0

struct node {
	char *blockStartPtr;
	int blockSize;
	int isFree;
	struct node *nextNodePtr;
};

typedef struct node Node;

static Node *headPtr;
static int poolSize = 0;

void free_heap();

/*Attempts to allocate a private heap of the given size, printing
 *a failure message if it does not succeed.*/
void create_pool(int size) {
	free_heap(); //Can only have one private heap at a time:
				 //free in case another is in use

	headPtr = malloc(sizeof(Node));
	headPtr->blockStartPtr = malloc(size);

	if (headPtr == NULL) {
		printf("Private heap could not be made.");
		free(headPtr);
		return;
	}

	poolSize = size;
	headPtr->blockSize = size;
	headPtr->isFree = true;
	headPtr->nextNodePtr = NULL;
}

/*Attempts to allocate the specified number of bytes in the private
 *heap, returning a pointer to it upon success. Upon failure, a message
 *is printed and NULL is returned.*/
void *my_malloc(int size) {
	Node *curr = headPtr;
	while (curr != NULL) {
		//had forgotten to check if curr was free, causing an infinite loop in main
		if (size <= curr->blockSize && curr->isFree) {
			//big enough chunk found
			int remainderSize = abs(curr->blockSize - size);
			if (0 < remainderSize) {
				//there will be a remainder block
				Node *remainderPtr = malloc(sizeof(Node));
				remainderPtr->blockStartPtr = (curr->blockStartPtr + size);
				remainderPtr->blockSize = remainderSize;
				remainderPtr->isFree = true;
				remainderPtr->nextNodePtr = curr->nextNodePtr;
				//update allocated block
				curr->nextNodePtr = remainderPtr;
				curr->blockSize = size;
			}
			curr->isFree = false;
			return (void *)(curr->blockStartPtr);
		}
		curr = curr->nextNodePtr;
	}
	//Failure: a large enough segment couldn't be found
	printf("Memory could not be allocated.\n");
	return NULL;

}

/*Attempts to free the block of memory pointed to by the given pointer. */
void my_free(void *block) {
	if (headPtr->blockStartPtr + poolSize < (char *) block) {
		printf("Memory is not on private heap.");
		return;
	}

	//Search for block
	Node *prev = NULL;
	Node *curr = headPtr;

	while (curr != NULL) {
		if (curr->blockStartPtr == ((char *)block)) {
			break;
		}
		//Confirms we haven't passed the block
		if (((char *)block) < curr->blockStartPtr + curr->blockSize) {
			printf("Memory is not on private heap.");
			return;
		}
		prev = curr;
		curr = curr->nextNodePtr;
	}

	curr->isFree = true;

	/*Block is not at the end, there is more than one block, or the following block is free*/
	if(curr->nextNodePtr != NULL && (curr->nextNodePtr->isFree)) {
		//Merge with following block
		int nextSize = curr->nextNodePtr->blockSize;
		Node *newNext = curr->nextNodePtr->nextNodePtr;
		free(curr->nextNodePtr);
		curr->nextNodePtr = newNext;
		curr->blockSize += nextSize;
	}

	/*The previous block is free*/
	if (prev != NULL && prev->isFree) {
		//merge prev and curr into one free block
		prev->blockSize += curr->blockSize;
		prev->nextNodePtr = curr->nextNodePtr;
		free(curr); //Node no longer needed
	}

}

/*Frees all nodes and frees the current private heap from the real heap.
 *Confirms that a private heap exists before this attempt.*/
void free_heap() {
	poolSize = 0;
	if (headPtr != NULL) {
		free(headPtr->blockStartPtr);
		Node *next = headPtr->nextNodePtr;
		free(headPtr);
		while (next != NULL) {
			Node *curr = next;
			next = curr->nextNodePtr;
			free(curr);
		}
	}
}
