/*
 * mallok_test.c
 * Tabitha Stein
 * TCSS 333 B - Spring 2015
 * A test file for mallok.c.
 */

#include 	"mallok.h"
#include	<stdlib.h>
#include	<stdio.h>
#define		true	1
#define		false	0

int count_num_requests();
void repeat_request_free();
void try_succeed_fail();
void a_and_b();
void request_mult_sizes();

int main(void) {
	printf("Num times 10 byte blocks can be allocated in 1000 byte pool before disallowed: %d\n\n", count_num_requests());
	repeat_request_free();
	try_succeed_fail();
	a_and_b();
	request_mult_sizes();
	return 0;
}

/*Counts the number of times 10 byte blocks of memory can be
 *allocated with my_malloc in a 1000 byte pool. Returns this
 *number.*/
int count_num_requests() {
	create_pool(1000);
	printf("Attempting to allocate 100 10 byte blocks to 1000 byte pool:\n");
	void *curr;
	int count = 0;
	do {
		curr = my_malloc(10);
		if(curr) {
			printf("Block #%d successfully allocated\n", count);
			count++;
		}

	} while (curr != NULL);
	return count;
}

/*Asks for a 1000 byte pool of memory and allocates 5 blocks of 200
 *bytes to it, frees them, reallocates them, etc. 7 times.*/
void repeat_request_free() {
	create_pool(1000);

	int i;
	for (i = 0; i < 7; i++) {
		int j;
		char *curr[5];
		printf("Attempt #%d to allocate and free 5 200 blocks to 1000 block pool:\n", i);
		//Allocate them all
		for (j = 0; j < 5; j++) {
			curr[j] = my_malloc(200);
			if (curr[j] == NULL) {
				printf("\tMemory could not be allocated on try #%d\n", j);
			} else {
				printf("\tBlock #%d successfully allocated\n", j);
			}
		}

		//Free them all now
		for (j = 0; j < 5; j++) {
			my_free(curr[j]);
		}

	}
}


void try_succeed_fail() {
	printf("\nAttempting to allocated 1000 byte pool and my_malloc 5, 200 byte blocks\n");
	create_pool(1000);
	int j;
	char *curr[5];
	//Allocate them all
	for (j = 0; j < 5; j++) {
		curr[j] = my_malloc(200);
	}
	char *testPtr;
	printf("Freeing Middle block\n");
	my_free(curr[2]);

	printf("Requesting 210 byte block\n");
	testPtr = my_malloc(210);
	if (testPtr) {
		printf("Mem allocation successful!\n");
	}

	printf("Requesting 150 byte block\n");
	testPtr = my_malloc(150);
	if (testPtr) {
		printf("Mem allocation successful!\n");
	}

	printf("Requesting 60 byte block\n");
	testPtr = my_malloc(60);
	if (testPtr) {
		printf("Mem allocation successful!\n");
	}

	printf("Requesting 50 byte block\n");
	testPtr = my_malloc(50);
	if (testPtr) {
		printf("Mem allocation successful!\n");
	}

	printf("\n");
}

void a_and_b() {
	create_pool(1000);
	int j;
	char *curr[5];
	//Allocate them all
	for (j = 0; j < 5; j++) {
		curr[j] = my_malloc(200);

		//Fill with As or Bs based on parity
		char filler;
		if (j % 2) { //if odd
			filler = 'B';
		} else {
			filler = 'A';
		}
		int i;
		for (i = 0; i < 200; i++) {
			*(curr[j] + i) = filler;
		}
	}

	//Confirm As and Bs weren't overwritten
	for (j = 0; j < 5; j++) {
		//Fill with As or Bs based on parity
		char filler;
		if (j % 2) { //if odd
			filler = 'B';
		} else {
			filler = 'A';
		}
		int correctFiller = false;
		int i;
		for (i = 0; i < 200; i++) {
			correctFiller = (*(curr[j] + i) == filler);
		}
		if (correctFiller) {
			printf("For block %d, all mem locations contained %c\n", j, filler);
		}
	}

	printf("\n");
}


void request_mult_sizes() {
	int pool_size = 1000;
	int block_sizes[] = {1000, 250, 100};

	create_pool(pool_size);
	int i;

	//Fill pool with blocks of each different size
	for (i = 0; i < 3; i++) {
		int j;

		//Determine number to make
		int num_to_make = 1000 / block_sizes[i];
		char *curr[num_to_make];

		//Allocate them all
		for (j = 0; j < num_to_make; j++) {
			curr[j] = my_malloc(block_sizes[i]);
			if (curr[j] == NULL) {
				printf("Memory could not be allocated for size %d on try #%d\n", block_sizes[i], j);
			} else {
				printf("Memory successfully allocated for size %d on try #%d\n", block_sizes[i], j);
			}
		}

		//Free them all now
		for (j = 0; j < num_to_make; j++) {
			my_free(curr[j]);
		}

		printf("\n");
	}
}
