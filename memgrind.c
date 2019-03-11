/* Aditi Singh and Rumeet Goradia
 * as2811, rug5
 * CS214, Section 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

/* testcaseA() --> malloc() 1 byte and immediately free it, 150 times */
void testcaseA()  {
	int a;
	for(a = 0; a < 150; ++a) {
		void* ptr = malloc(1);
		free(ptr);
	}
}

/* testcaseB() --> malloc() 1 byte pointer 50 times in a row, then free() all 50
 * 	1. repeat entire process 3 times to get a total of 150 malloc() calls and free() calls
 */ 	
void testcaseB()  {
	int i = 0;
	int j = 0;
	char* arr[50];
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 50; ++j) {
			arr[j] = malloc(1);
		}
		for (j = 0; j < 50; ++j) {
			free(arr[j]);
		}
	}
}


/* testcaseC() --> randomly malloc() and free() 1 byte pointers
 * 	1. use random number generator to decide whether to malloc() or free()
 * 	2. keep repeating until a total of 50 pointers are allocated at the same time
 * 	3. free all pointers
 */
void testcaseC() {
	int count=0;
	int r=0;
	int p=0;
	int n=0;
	//seed random number generator
	srand(time(0));
	char *arr[50];
	for (p = 0; p < 50; ++p) {
		//initialize array
		arr[p] = NULL;
	}
	while (count < 50) {
		//random number generator
		n = (int)rand() % 2;
		if(n == 0) {
			int i = 0;
			//choose first available index
			for (i = 0; i <= count; ++i) {
				if (!arr[i]) {
					arr[i] = malloc(1);
					++count;
					break;
				}
			}
		}
		else {
			int j = 0;
			if (count != 0) {
				for(j=0; j < 50; ++j) {
					if(arr[j]) {
						free(arr[j]);
						--count;
						arr[j] = NULL;
						break;
					}
				}
			}
		}
	}
	//free all
	for(r=0; r<50; ++r) {
		free(arr[r]);
		arr[r]=NULL;
	}
}
	
/* testcaseD() --> randomly malloc() or free() a randomly sized pointer
 *	1. use same basic setup as testcaseC()
 *	2. use memAll to keep track of available memory, starting with 4091 since first five bytes are metadata and magic number
 *	3. for simplicity, always free() last added pointer 
 *	4. repeat until 50 pointers are allocated at the same time, and then free all of them
 */
void testcaseD() {
	int count = 0;
	int r = 0;
	int m = 0;
	int n = 0;
	int memAll = 5;
	int size = 0;
	char *arr[50];
	//keep track of individual pointer sizes
	int mem[50];
	srand(time(0));
	for(m = 0; m < 50; ++m) {
		arr[m] = NULL;
		mem[m] = 0;
	}
	int last_inserted = n;
	while(count < 50) {
		n = (int)rand() % 2;
		size = ((int)rand() % 64) + 1;
		if (n == 0 && memAll + size < 4091) {
			int i = 0;
			for (i = 0; i <= count; ++i) {
				if (!arr[i]) {
					arr[i] = malloc(size);
					mem[i] = size;
					memAll += size + 3;						
					++count;
					last_inserted = i;
					break;
				}
			}	
		} else if (n == 1 || (memAll + size >= 4091)) {
			if (count != 0) {
				//start freeing from the end
				free(arr[last_inserted]);
				arr[last_inserted] = NULL;
				memAll -= mem[last_inserted] + 3;
				mem[last_inserted] = 0;
				--count;
				--last_inserted;
			}
		}
	}
	//make sure that anything remaining is also freed
	for(r = 0; r < 50; ++r) {
		free(arr[r]);
	}
}

/* testcaseE() --> allocate pointers until capacity of myblock is full, then free() and reallocate() every other pointer
 * 	1. 75 50-byte pointers is near max capacity of myblock
 */
void testcaseE() {
	int mind=0;
	int find=0;
	int *ptrs[75];
//malloc until capacity is reached
	for (mind = 0; mind < 75; ++mind) {
		ptrs[mind] = malloc(50);
	}
//free every other block
	for(find = 0; find < mind; find += 2) {
		free(ptrs[find]);
		ptrs[find] = NULL;
	}
//remalloc
	for(mind=0; mind < 75; mind += 2) {
		ptrs[mind] = malloc(50);
	}
	for (find = 0; find < 75; ++find) {
		free(ptrs[find]);
		ptrs[find] = NULL;
	}

}

/* testcaseF() --> demonstrates the effectiveness of the library's coalescing features 
 * 	1. two pointers of the same size are malloced and freed repeatedly at increasing sizes
 * 	2. then the same happens, but with decreasing sizes
 */ 
void testcaseF() {
	int size = 1;
	char *ptrA, *ptrB;
	int i = 0;
	for (i = 0; i < 3; ++i) {
		for (size = 1; size < 4091/2; size += 50) {
			ptrA = malloc(size);
			ptrB = malloc(size);
			if (ptrA) {
				free(ptrA);
			}
			if (ptrB) {
				free(ptrB);
			}
		}
		for (; size > 0; size -= 50) {
			ptrA = malloc(size);
			ptrB = malloc(size);
			if (ptrA) {
				free(ptrA);
			}
			if (ptrB) {
				free(ptrB);
			}
		}
	}
}

int main() {
	struct timeval start;
	struct timeval end;
	int a = 0;
	int r = 0;
	int sum = 0;
	int result[600];
	int i=0;
	for(a = 0; a < 100; ++a) {
//results for A will be stored in result[0], result[6], result[12], etc.
//A
		sum = 0;
		gettimeofday(&start, NULL);
		testcaseA();
		gettimeofday(&end, NULL);
		sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		result[r] = sum;
		++r;
//results for B will be stored in result[1], result[7], etc.
//B	
		sum = 0;
		gettimeofday(&start, NULL);
		testcaseB();
		gettimeofday(&end, NULL);
		sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		result[r]=sum;
                ++r;
//C
		sum = 0;
                gettimeofday(&start, NULL);
                testcaseC();
                gettimeofday(&end, NULL);
                sum = sum + ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
                result[r] = sum;
                ++r;

//D	
		sum = 0;
                gettimeofday(&start, NULL);
                testcaseD();
                gettimeofday(&end, NULL);
                sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
                result[r] = sum;
                ++r;


//E
		sum = 0;
		gettimeofday(&start, NULL);
		testcaseE();
		gettimeofday(&end, NULL);
		sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		result[r] = sum;
		++r;

//F
		sum = 0;
		gettimeofday(&start, NULL);
		testcaseF();
		gettimeofday(&end, NULL);
		sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		result[r] = sum;
		++r;
	}
	
	int ra=0;
	for(i = 0; i < 600; i += 6) { 
		ra= ra+result[i];
	}
	printf("For Test Case A, the average time is %d milliseconds.\n", ra/100);
	
	int rb=0;
        for(i = 1; i < 600; i += 6) {
		rb= rb+result[i];
	}
	printf("For Test Case B, the average time is %d milliseconds.\n", rb/100);

	int rc=0;
	for(i = 2; i < 600; i += 6) {
		rc= rc+result[i];
	}
	printf("For Test Case C, the average time is %d milliseconds.\n", rc/100);

	int rd=0;
	for(i = 3; i < 600; i += 6) {
		rd= rd+result[i];
	}
	printf("For Test Case D, the average time is %d milliseconds.\n", rd/100);
	
	int re=0;
	for(i = 4; i < 600; i += 6) {
		re= re+result[i];
	}
	printf("For Test Case E, the average time is %d milliseconds.\n", re/100);
	
	int rf=0;
	for (i = 5; i < 600; i += 6) {
		rf = rf+result[i];
	}
	printf("For Test Case F, the average time is %d milliseconds.\n", rf/100);

	return EXIT_SUCCESS;
}


