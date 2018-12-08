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
	invalidInitSizeTestBF();
	memoryIsAllocated();
	reuseDeallocMem();
	noBestFit();
	deallocNonExistingBlock();
	test1_bf();
	test1_wf()
	test2_bf();
	test2_wf();
	bestFitVsWorstFitTest1();
	bestFitVsWorstFitTest2();

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

/*
	TEST 1: 

	Tests if the system can handle an invalid memory block size. The size 
	must be large enough to fit at least one control block. 
*/
void test1_bf() {
	int initVal;

	initVal = best_fit_memory_init(3);

	if (initVal == -1) {
		printf("PASSED Test1 - Best Fit\n");
	} else {
		printf("FAILED Test1 - Best Fit");
	}
}

void test1_wf() {
	int initVal;

	initVal = worst_fit_memory_init(3);

	if (initVal == -1) {
		printf("PASSED Test1 - Worst Fit\n");
	} else {
		printf("FAILED Test1 - Worst Fit");
	}
}

void test2_bf() {
	int initVal, num;
	void* p1,*p2;
	
	initVal = best_fit_memory_init(1024);

	if(initVal == 0) {
		p1 = best_fit_alloc(8);
		if (p1 != NULL) {
			num = best_fit_count_extfrag(1001);			
			p2 = best_fit_alloc(6);
			
			if (p2 != NULL) {
				num = best_fit_count_extfrag(1001);
				
				best_fit_dealloc(p1);
				best_fit_dealloc(p2);

				num = best_fit_count_extfrag(1001);
				printf("PASSED Test2 - Best Fit\n");
			}else {
				printf("FAILED Test2 - Best Fit - failed to initialize memory - failed to allocate memory for best fit\n");
			}	
					
		}else {
			printf("FAILED Test2 - Best Fit - failed to initialize memory - failed to allocate memory for best fit");
		}
		
	}else {
		printf("FAILED Test2 - Best Fit - failed to initialize memory");
	}
}

void test2_wf() {
	int initVal, num;
	void* p1,*p2;
	
	initVal = best_fit_memory_init(1024);

	if(initVal == 0) {
		p1 = best_fit_alloc(8);
		if (p1 != NULL) {
			num = best_fit_count_extfrag(1001);
			p2 = best_fit_alloc(6);
			
			if (p2 != NULL) {
				num = best_fit_count_extfrag(1001);
				
				best_fit_dealloc(p1);
				best_fit_dealloc(p2);

				num = best_fit_count_extfrag(1001);
				printf("PASSED Test2 - Worst Fit\n");
			}else {
				printf("FAILED Test2 - Worst Fit - failed to initialize memory - failed to allocate memory for best fit\n");
			}	
					
		}else {
			printf("FAILED Test2 - Worst Fit - failed to initialize memory - failed to allocate memory for best fit");
		}
		
	}else {
		printf("FAILED Test2 - Worst Fit - failed to initialize memory");
	}
}

void bestFitVsWorstFitTest1() {
    void *p1, *p2;
    void *pBf[10], *pWf[210];

    best_fit_memory_init(2000);
    worst_fit_memory_init(2000);
    
    p1 = best_fit_alloc(950);
    p2 = worst_fit_alloc(950);
    
    best_fit_alloc(10);
    worst_fit_alloc(10);

    best_fit_dealloc(p1);
    worst_fit_dealloc(p2);
    
    worst_fit_count_extfrag(100);
    
    for (int i = 0; i < 10; i ++ ){
        pBf[i] = best_fit_alloc(50);
        pWf[i] = worst_fit_alloc(50);
    } 

    int num;

    num = best_fit_count_extfrag(600);
    printf("best fit fragments smaller than 600  bytes:%d\n", num);
    num = worst_fit_count_extfrag(600);
    printf("worst fit fragments smaller than 600 bytes:%d\n", num);    
        
}
void bestFitVsWorstFitTest2() {
    void *pBf[10], *pWf[10];

    best_fit_memory_init(2000);
    worst_fit_memory_init(2000);

    for (int i = 0; i < 10; i ++ ) {
        pBf[i] = best_fit_alloc(50);
        pWf[i] = worst_fit_alloc(50); 
    }

    for (int i = 0; i < 10; i += 2) {
        best_fit_dealloc(pBf[i]);
        worst_fit_dealloc(pWf[i]); 
    }
    int num;

    num = best_fit_count_extfrag(10);
    printf("\n");
    num = worst_fit_count_extfrag(10);


    for (int i = 0; i < 5; i ++ ) {
        best_fit_alloc(20);
        worst_fit_alloc(20);
    }
    printf("\nbest fit test:\n\n");
    num = best_fit_count_extfrag(50);
    printf("\nbest fit fragments smaller than 50 bytes: %d\n", num);
    printf("\nworst fit test:\n\n");
    num = worst_fit_count_extfrag(50);
    printf("\nworst fit fragments smaller than 50 bytes: %d\n", num);
}



