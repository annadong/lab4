/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 */
// * @date: 2015-11-20

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */

//linked list structure to store memory blocks
typedef struct node {
	int size;
	int isFree; // mark as 1 if it is free, 0 if it is in use
	struct node* next;
	struct node* prev;
}node_t;

/* global variables */
void* bfHead;
void* wfHead;
/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.

	// do not initialize memory unless the size given is greater than 4 byte
	//plus the size of a linked list node
	if (size < sizeof(node_t) + 4) {
		return -1;
	}

	//allocate the size specified and assign the address to the best fit pointer
	bfHead = malloc(size);

	//initialize the first block in the linked list
	node_t* thisNode = (node_t *) bfHead;
	thisNode->size = size - sizeof(node_t);
	thisNode->isFree = 1;
	thisNode->next = NULL;
	thisNode->prev = NULL;

	return 0;

}

int worst_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.

	// do not initialize memory unless the size given is greater than 4 byte
	//plus the size of a linked list node
	if (size < sizeof(node_t) + 4) {
		return -1;
	}

	//allocate the size specified and assign the address to the worst fit pointer
	wfHead = malloc(size);

	//initialize the first block in the linked list
	node_t* thisNode = (node_t *) wfHead;
	thisNode->size = size - sizeof(node_t);
	thisNode->isFree = 1;
	thisNode->next = NULL;
	thisNode->prev = NULL;

	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	// To be completed by students
	size_t thisSize = size;
	node_t *cur = (node_t *) bfHead;
	

	//set the minimum size to be really big so that it can be changed to a smaller size after comparison
	size_t minSize = 100000;
	node_t* smallestNode = NULL;

	//math logic to round the size requested to the nearest multiple 4 to reduce very small fragmentation
	thisSize += (4-thisSize%4)%4;

	//go through the list and find the smallest block available that contains enough memory for the requested memory size
	while (cur!= NULL) {
		if (cur->size >= thisSize && cur->size < minSize && cur->isFree == 1) {
			minSize = cur->size;
			smallestNode = cur;
		}
		cur = cur->next;
	}

	//if we did not find a memory block that can be used, return null
	if (smallestNode == NULL) {
		return NULL;
	}

	//if the memory size found is exactly the size you want then set this memory block to be in use
	if (thisSize == smallestNode->size) {
		smallestNode->isFree = 0;
	} else {
		//find the remaining size of free memory that is not in use
		size_t remainingSize = smallestNode->size - thisSize;

		// check to see if the remaining size is too small to be of use, (less than 4 byte + size of a node structure)
		//if it is too small, just use the block
		if (remainingSize < sizeof(node_t) + 4) {
			smallestNode->isFree = 0;
		}else {
			//if it is not too small, then we want to partition the block into two blocks
			// one block being the size of the requested size and this is the block that we are going to return
			//the other block is a new block with the remaining free memory space, and add this block to directly precede the available block found
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


	size_t thisSize = size;
	node_t *cur = (node_t *) wfHead;
	
	size_t maxSize = 0;
	node_t* biggestNode = NULL;

	thisSize += (4-thisSize%4)%4;

	//unlike best fit, instead of finding the smallest memory block, checks for the biggest block of memory available
	//also check if the biggest block is big enough to satisify the requested size 
	while (cur!= NULL) {
		if (cur->size >= thisSize && cur->size > maxSize && cur->isFree == 1) {
			maxSize = cur->size;
			biggestNode = cur;
		}
		cur = cur->next;
	}

	if (biggestNode == NULL) {
		return NULL;
	}
	printf("size of biggestNode node: %lu\n", (unsigned long) biggestNode->size);

	if (thisSize == biggestNode->size) {
		biggestNode->isFree = 0;
	} else {
		size_t remainingSize = biggestNode->size - thisSize;

		if (remainingSize < sizeof(node_t) + 4) {
			biggestNode->isFree = 0;
		}else {
			node_t* newNode = (node_t *)((size_t) biggestNode + thisSize +sizeof(node_t));
			newNode->size = biggestNode->size - thisSize - sizeof(node_t);
			newNode->prev = biggestNode;
			newNode->next = biggestNode->next;
	
			node_t* next = biggestNode->next;
		
			biggestNode->next = newNode;
			if(next != NULL) {
				next->prev = newNode;
			}
			newNode->isFree = 1;
			 biggestNode->isFree = 0;
			
			biggestNode->size = thisSize;
		}
	}

	return (node_t *)((size_t)biggestNode+sizeof(node_t));
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{

	// To be completed by students

	//error check
	if (!ptr) {
		return;
	}

	node_t* addressToFind = (node_t*) ((size_t)(ptr)- sizeof(node_t));

	node_t* nodeToDealloc = NULL;
	node_t* curNode = (node_t *) bfHead;

	//try to find the memory block in the linked list
	while (curNode != NULL) {
		if (curNode == addressToFind) {
			nodeToDealloc = curNode;
			break;
		}
		curNode = curNode->next;
	}

	// if no block is found, return
	if (!nodeToDealloc) {
		return;
	}


	//if a block is found, free up that block
	nodeToDealloc->isFree = 1;

	size_t newSize = nodeToDealloc->size;
	
	//coalscenece:

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
	if (!ptr) {
		return;
	}

	node_t* addressToFind = (node_t*) ((size_t)(ptr)- sizeof(node_t));

	node_t* nodeToDealloc = NULL;
	node_t* curNode = (node_t *) wfHead;

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

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students

	//intereate through the linked list and have a count variable that increments each time a free memory block
	//less than the specified size is found
	node_t* cur = (node_t *) bfHead;
	int count = 0;
	while (cur != NULL) {
		printf("block size: %lu, block is free:  %d\n", (unsigned long) cur->size, cur->isFree);
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
	node_t* cur = (node_t *) wfHead;
	int count = 0;
	while (cur != NULL) {
		printf("block size: %lu, block is free  %d\n ", (unsigned long) cur->size, cur->isFree);
		if (cur->isFree == 1 && cur->size < size ) {
			count += 1;
		}
		cur = cur->next;
	}
	return count; 
	
}


