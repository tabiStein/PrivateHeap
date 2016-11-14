/*
 * mallok.h
 * Tabitha Stein
 * TCSS 333 B - Spring 2015
 * Assignment 6
 */

#ifndef MALLOK_H_
#define MALLOK_H_

void create_pool(int size);
void *my_malloc(int size);
void my_free(void *block);

#endif /* MALLOK_H_ */
