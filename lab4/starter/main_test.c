/**Lw
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"



int main(int argc, char *argv[])
{
	int num = 0;
	int algo = 0; // default algorithm to test is best fit  
	void *p, *q;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <0/1>. 0 for best fit and 1 for worst fit \n", argv[0]);
		exit (1);
	} else if (!strcmp(argv[1], "1") || !strcmp(argv[1], "0")) {
		algo = atoi(argv[1]);
	} else {
		fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
		exit(1);
	}
	
	if ( algo == 0 ) {
		/*
		best_fit_memory_init(1024);	// initizae 1KB, best fit

		p = best_fit_alloc(8);		// allocate 8B
		printf("best fit: p=%p\n", p);
		if ( p != NULL ) {
			best_fit_dealloc(p);	
		}
		num = best_fit_count_extfrag(4);
		*/
		//memoryIsAllocated();
		//reuseDeallocMem();
		//noBestFit();
		deallocNonExistingBlock();
	} else if ( algo == 1 ) {

		worst_fit_memory_init(1024);	// initizae 1KB, worst fit

		q = worst_fit_alloc(8);		// allocate 8B
		printf("worst fit: q=%p\n", q);
		if ( q != NULL ) {
			worst_fit_dealloc(q);	
		}
		num = worst_fit_count_extfrag(4);
	} else {
		fprintf(stderr, "Should not reach here!\n");
		exit(1);
	}

	//printf("num = %d\n", num);

	return 0;
}

void invalidInitSizeTestBF() {
	int initVal;

	initVal = best_fit_memory_init(3);

	if (initVal == -1) {
		printf("invalid size\n");
	}
	
}

void memoryIsAllocated() {
	int initVal, num;
	void* p1,*p2;
	
	initVal = best_fit_memory_init(1024);

	if(initVal == 0) {
		printf("initialized memory\n");
		p1 = best_fit_alloc(8);
		if (p1 != NULL) {
			printf("allocated memory for best fit: p1=%p\n", p1);
			num = best_fit_count_extfrag(1001);

			printf("number of free blocks less than 1001: %d\n", num);
			
			p2 = best_fit_alloc(6);
			
			if (p2 != NULL) {
				printf("allocated memory for best fit: p2=%p\n", p2);
				num = best_fit_count_extfrag(1001);
				printf("number of free blocks less than 1001: %d\n", num);
				
				best_fit_dealloc(p1);
				best_fit_dealloc(p2);

				num = best_fit_count_extfrag(1001);
				printf("number of free blocks less than 1001: %d\n", num);
			}else {
				printf("failed to allocate memory for best fit\n");
			}	
					
		}else {
			printf("failed to allocate memory for best fit\n");
		}
		
	}else {
		printf("failed to initialize memory");
	}
}

void reuseDeallocMem() {
	void *p1, *p2, *p3;
	int num;

	best_fit_memory_init(1024);

	p1 = best_fit_alloc(8);
	p2 = best_fit_alloc(8);
	printf("address: %p\n", p2);
	p3 = best_fit_alloc(8);
	best_fit_dealloc(p2);

	num = best_fit_count_extfrag(10);
	printf("number of free blocks less than 10:%d\n", num);
	
	p2 = best_fit_alloc(8);
	printf("address: %p\n", p2);
	
	num = best_fit_count_extfrag(10);
	printf("number of free blocks less than 10:%d\n", num);
}

void noBestFit() {
	void *p1, *p2;
	
	best_fit_memory_init(100);

	p1 = best_fit_alloc(50);

	if (p1 != NULL) {
		p2 = best_fit_alloc(50);
		if (p2 == NULL) {
			printf("unable to allocate memory because no more space\n");
		} else {
			printf("test failed");
		}
	} else {
		printf("failed to allocate memory");
	}
}

void deallocNonExistingBlock() {
	void *p1, *p2 ;
	int num;
	
	best_fit_memory_init(100);
	
	p1 = best_fit_alloc(8);
	p2 = best_fit_alloc(8);
	
	num = best_fit_count_extfrag(11);
	printf("number of free blocks less than 11: %d\n", num);

	best_fit_dealloc(p1);
	num = best_fit_count_extfrag(11);
	printf("number of free blocks less than 11 after deallocating a size 10 block: %d\n", num);

	best_fit_dealloc(p1);
	num = best_fit_count_extfrag(11);
	printf("number of free blocks less than 11 after deallocating a non existing  block: %d\n", num);

}

