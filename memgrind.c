#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"



//this test case mallocs 1 byte and then frees it immediately 
//this is done 150 times

void testcaseA()  {
	int a;
	for(a = 0; a < 150; ++a) {
		void* ptr = malloc(1);
		free(ptr);
	}
}


//this test case mallocs 1 byte and stores the pointer within an array
//this is done 150 times 
//once 50 byte chunks are malloced then the 50 one byte pointers one by one
void testcaseB()  {
	int i = 0;
	int j = 0;
	char* arr[50];
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 50; j++) {
			arr[j] = malloc(1);
		}
		for (j = 0; j < 50; j++) {
			free(arr[j]);
		}
	}
}
/*this test case randomly chooses between a 1 byte malloc and a 1 byte pointer (this is done 50 times)
The variable count keeps track of the amount of times and malloc and free are performed
*/

void testcaseC() {
	int count=0;
	int r=0;
	int p=0;
	int n=0;
	srand(time(0));
	char *arr[50];
	for (p = 0; p < 50; ++p) {
		arr[p] = NULL;
	}
	while (count < 50) {
		n = (int)rand() % 2;
		if(n == 0) {
			int i = 0;
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
	for(r=0; r<50; ++r) {
		free(arr[r]);
		arr[r]=NULL;
	}
}
	

/*this test case chooses between a randomly sized malloc or free pointer that is between 1 and 64 bytes
 * the amount of memory allocated is kept track through the variable memAll
 *the amount of times malloc is performed is kept track of through the variable count so that the total doesn't exceed memory capacity and is only done a total of 50 times 
 *Once malloc is completed all the pointers are freed
 */

void testcaseD() {
	int count = 0;
	int r = 0;
	int m = 0;
	int n = 0;
	int memAll = 0;
	int size = 0;
	char *arr[50];
	int mem[50];
	srand(time(0));
	for(m = 0; m < 50; ++m) {
		arr[m] = NULL;
	}
	while(count < 50) {
		n = (int)rand() % 2;
		size = ((int)rand() % 64) + 1;
//		printf("size: %d\n", size);
		if (n == 0 && ((memAll + size + 3) <= 4091)){
			int i = 0;
			for (i = 0; i <= count; ++i) {
				if (!arr[i]) {
					arr[i] = malloc(size);
					mem[i] = size + 3;
					memAll += size + 3;
					++count;
					break;
				}
			}	
		} else if (n == 1 || (memAll + size + 3 > 4091 && count < 50)) {
			if (count != 0) {
				int j = 0;
				for(j = 0; j < 50; ++j) {
					if(arr[j]) {
						free(arr[j]);
						arr[j] = NULL;
						memAll -= mem[j];
						--count;
						break;
					}
				}
			}
		}
	}
		//make sure that anything remaining is also freed
	for(r = 0; r < 50; ++r) {
		free(arr[r]);
		arr[r] = NULL;
	}
}

/*this test case first performs malloc until capacity is reached and then it frees every other block and eventually remallocs
 * the variable find keeps track of the indice of free 
 * the variable mind keep track of the indice of malloc  
*/

void testcaseE() {
	int block=1;
	int mind=0;
	int find=0;
	int *ptrs[1000];

//malloc until capacity is reached

	while ((ptrs[mind] = malloc(50)) != NULL) {
		block++;
		mind++;
		}
//free every other block
	for(find=0; find<mind; find= find+2) {
		free(ptrs[find]);
		ptrs[find] = NULL;
	}
//remalloc
	for(mind=0; mind<block-1; find = find+1) {
		free(ptrs[find]);
		ptrs[find]= NULL;
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
	
//A
	for(a = 0; a < 100; ++a) { 
		sum = 0;
		gettimeofday(&start, NULL);
		testcaseA();
		gettimeofday(&end, NULL);
		sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		result[r] = sum;
		++r;

        	int ra=0;
       		 for(i = 0; i < 100; i += 6) {
                ra= ra+result[i];
        }

        	printf("For Test Case A the average time is %d milliseconds\n", ra/100);

//B	
		sum = 0;
		gettimeofday(&start, NULL);
		testcaseB();
		gettimeofday(&end, NULL);
		sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		result[r]=sum;
                ++r;
	 int rb=0;
        for(i = 1; i < 100; i += 6) {
                rb= rb+result[i];
        }
        printf("For Test Case B the average time is %d milliseconds\n", rb/100);




//C
		sum = 0;
                gettimeofday(&start, NULL);
                testcaseC();
                gettimeofday(&end, NULL);
                sum = sum + ((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
                result[r] = sum;
                ++r;

	        int rc=0;
        for(i = 2;i < 100; i += 6) {
                rc= rc+result[i];
        }
        printf("For Test Case C the average time is %d milliseconds\n", rc/100);




//D	
		sum = 0;
                gettimeofday(&start, NULL);
                testcaseD();
                gettimeofday(&end, NULL);
                sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
                result[r]= sum;
                ++r;

		 int rd=0;
	        for(i = 3;i < 100; i += 6) {
                rd= rd+result[i];
        	}
        	printf("For Test Case D the average time is %d milliseconds\n", rd/100);

		}


//E
		sum = 0;
                gettimeofday(&start, NULL);
                testcaseE();
                gettimeofday(&end, NULL);
                sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
                result[r]= sum;
                ++r;

                 int re=0;
                for(i = 3;i < 100; i += 6) {
                re= re+result[i];
                }
                printf("For Test Case E the average time is %d milliseconds\n", re/100);

                

		return EXIT_SUCCESS;
}


/*
	int i = 0;
	
	int ra=0;
	for(i = 0; i < 100; i += 6) { 
		ra= ra+result[i];
	}
	
	printf("For Test Case A the average time is %d milliseconds\n", ra/100);
	

	int rb=0;
        for(i = 1; i < 100; i += 6) {
		rb= rb+result[i];
	}
	printf("For Test Case B the average time is %d milliseconds\n", rb/100);


	int rc=0;
	for(i = 2;i < 100; i += 6) {
		rc= rc+result[i];
	}
	printf("For Test Case C the average time is %d milliseconds\n", rc/100);

	int rd=0;
	for(i = 3;i < 100; i += 6) {
		rd= rd+result[i];
	}
	printf("For Test Case D the average time is %d milliseconds\n", rd/100);
	
	int re=0;
	for(i = 4; i < 100; i += 6) {
		re= re+result[i];
	}
	printf("For Test Case E the average time is %d milliseconds\n", re/100);
	return EXIT_SUCCESS;
}

*/
