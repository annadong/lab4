/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */

typedef struct node {
	int size;
	int isFree;
	struct node* next;
	struct node* prev;
}node_t;

/* global variables */
node_t* bfHead;

/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	if (size < sizeof(node_t) + 4) {
		return -1;
	}


	bfHead = malloc(size);
	bfHead->size = size - sizeof(node_t);
	bfHead->isFree = 1;
	bfHead->next = NULL;
	bfHead->prev = NULL;
	
	printf("initialize size: %lu\n",(unsigned long) bfHead->size);
	printf("size of node_t: %lu\n", (unsigned long) sizeof(node_t));
	printf("size inputted: %lu\n", (unsigned long) size);	
	return 0;

}

int worst_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	// To be completed by students
	size_t thisSize = size;
	node_t *cur = bfHead;
	
	size_t minSize = 100000;
	node_t* smallestNode = NULL;

	thisSize += (4-thisSize%4)%4;

	while (cur!= NULL) {
		if (cur->size >= thisSize && cur->size < minSize && cur->isFree == 1) {
			minSize = cur->size;
			smallestNode = cur;
		}
		cur = cur->next;
	}

	if (smallestNode == NULL) {
		return NULL;
	}
	printf("size of smallest node: %lu\n", (unsigned long) smallestNode->size);

	if (thisSize == smallestNode->size) {
		smallestNode->isFree = 0;
	} else {
		size_t remainingSize = smallestNode->size - thisSize;

		if (remainingSize < sizeof(node_t) + 4) {
			smallestNode->isFree = 0;
		}else {
			node_t* newNode = (node_t *)((size_t) smallestNode + thisSize +sizeof(node_t));
			newNode->size = smallestNode->size - thisSize - sizeof(node_t);
			newNode->prev = smallestNode;
			newNode->next = smallestNode->next;
	
			node_t* next = smallestNode->next;
		
			smallestNode->next = newNode;
			if(next != NULL) {
				next->prev = newNode;
			}
			newNode->isFree = 1;
			smallestNode->isFree = 0;
			
			smallestNode->size = thisSize;
		}
	}

	return (node_t *)((size_t)smallestNode+sizeof(node_t));
}


void *worst_fit_alloc(size_t size)
{
	// To be completed by students
	return NULL;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{

	// To be completed by students
	if (!ptr) {
		return;
	}

	node_t* addressToFind = (node_t*) ((size_t)(ptr)- sizeof(node_t));

	node_t* nodeToDealloc = NULL;
	node_t* curNode = bfHead;

	while (curNode != NULL) {
		if (curNode == addressToFind) {
			nodeToDealloc = curNode;
			break;
		}
		curNode = curNode->next;
	}

	if (!nodeToDealloc) {
		return;
	}

	nodeToDealloc->isFree = 1;

	size_t newSize = nodeToDealloc->size;
	
	//if the next node is free, merge
	if(nodeToDealloc-> next != NULL) {
		if (nodeToDealloc->next->isFree == 1) {
			newSize += nodeToDealloc->next->size + sizeof(node_t);
			node_t* newNext = nodeToDealloc->next->next;
			nodeToDealloc->next = newNext;
			if(newNext != NULL) {
				newNext->prev = nodeToDealloc;
			}
			nodeToDealloc->size = newSize;
		}
	}

	//if the previous node is free, merge
	if(nodeToDealloc->prev != NULL) {
		if (nodeToDealloc->prev->isFree == 1) {
			newSize += nodeToDealloc->prev->size + sizeof(node_t);
			node_t* prev = nodeToDealloc->prev;
			node_t* next = nodeToDealloc->next;
			prev->next = next;
			if(next != NULL) {
				next->prev = prev;
			}
			prev->size = newSize;
			prev->isFree = 1;
		}
	}


	return;
}

void worst_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students
	node_t* cur = bfHead;
	int count = 0;
	while (cur != NULL) {
		printf("cur size: %lu\n", (unsigned long) cur->size);
		if (cur->isFree == 1 && cur->size < size ) {
			count += 1;
		}
		cur = cur->next;
	}
	return count;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}

