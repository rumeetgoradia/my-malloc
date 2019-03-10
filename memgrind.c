#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

void testcaseA()  {
	int a;
	for(a = 0; a < 150; a++) {
		void* ptr = malloc(1);
//		printf("malloced\n");
		free(ptr);
//		printf("freed\n");
	}
}

void testcaseB()  {
	int a;
	char* arr[150];
	for (a=0; a<150; a++) {
		arr[a]=malloc(1);
	}
	for(a=0;a<150;a++) {
		free(arr[a]);
		arr[a]=NULL;
	}
}

void testcaseC() {
	int a=0;
	int r=0;
	int p=0;
	int n=0;
	srand(time(0));
	char *arr[50];
	for (p = 0; p < 50; ++p) {
		arr[p] = NULL;
	}
	while (a < 50) {
		n = (int)rand() % 2;
		if(n == 0) {
			int i = 0;
			for (i = 0; i <= a; ++i) {
				if (!arr[i]) {
					arr[i] = malloc(1);
					++a;
					break;
				}
			}
		}
		else {
			int j = 0;
			if (a != 0) {
				for(j=0; j < 50; ++j) {
					if(arr[j]) {
						free(arr[j]);
						--a;
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
	

int get(int min, int max) {
	srand(time(0));
	return(random()%(max-min)+min);
}

//1 to 64 bytes allocation 

void testcaseD() {
	int a = 0;
	int r = 0;
	int m = 0;
	int n = 0;
	int mema = 0;
	int size = 0;
	char *arr[50];
	int mem[50];
	srand(time(0));
	for(m = 0; m < 50; ++m) {
		arr[m] = NULL;
	}
	while(a < 50) {
		n = (int)rand() % 2;
		size = ((int)rand() % 64) + 1;
//		printf("size: %d\n", size);
		if (n == 0 && ((mema + size + 3) <= 4093)){
			int i = 0;
			for (i = 0; i <= a; ++i) {
				if (!arr[i]) {
					arr[i] = malloc(size);
					mem[i] = size + 3;
					mema += size + 3;
					++a;
					break;
				}
			}	
		} else if (n == 1 || (mema + size + 3 > 4093 && a < 50)) {
			if (a != 0) {
				int j = 0;
				for(j = 0; j < 50; ++j) {
					if(arr[j]) {
						free(arr[j]);
						arr[j] = NULL;
						mema -= mem[j];
						--a;
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


int main() {
	struct timeval start;
	struct timeval end;
	int a = 0;
	int r = 0;
	int sum = 0;
	int result[600];
	
//A
	for(a = 0; a < 100; ++a) { 
		sum = 0;
		gettimeofday(&start, NULL);
		testcaseA();
		gettimeofday(&end, NULL);
		sum = sum + ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
		result[r] = sum;
		++r;
	

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
                result[r]= sum;
                ++r;

	}
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
	return EXIT_SUCCESS;
}
