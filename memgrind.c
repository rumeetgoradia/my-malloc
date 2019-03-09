#include <stdio.h>
#include "mymalloc.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>



void testcaseA()  {
	int a;
	for(a=0; a<150; a++) {
		void* ptr = malloc(1);
		free(ptr);
		
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
	int p;
	double n;
	
	char *arr[50];
	for(p=0;p<50;p++) {
		arr[p] = NULL;

		}
	while (a<50) {
		n = (double)rand() / (double)RAND_MAX;
		if(n<.5) {
			arr[a] = malloc(1);
			a++;
			continue;
		}
		else{
			int j=0;
			for(j=0; j<50; j++); {
				if(arr[j]) {
					free(arr[j]);
					arr[j]=NULL;	
					break;
				}
			else
				continue;
			}
		}
	}
	for(r=0; r<50; r++) {
		free(arr[r]);
		arr[r]=NULL;
	}
	}
	

int get(int min, int max) {
	return(random()%(max-min)+min);
}

//1 to 64 bytes allocation 

void testcaseD() {
	int a;
	int r;
	int m;
	double n;
	int mema=0;
	int num;
	char *arr[50];
	int mem[50];

	for(m=0; m<50; m++) {
		arr[m] = NULL;
		}

	while(a<50) {
		n = (double)random() / (double)RAND_MAX;
		num=get(1, 64);
		if (n<.5&&((mema+num)<=5000)){
			arr[a] = malloc(num);
			mem[a]= num;
			mema= mema+num;
			a++;
			continue;
			}	
		else{
			int j;
			for(j=0; j<50; j++) {
				if(arr[j]) {
					free(arr[j]);
					arr[j]= NULL;
					mema-=mem[j];
					break;
			}
		else
			continue;
			}
		}
		}
		//make sure that anything remaining is also freed
		for(r=0; r<50; r++) {
			free(arr[r]);
			arr[r]=NULL;
		}
	}


int main() {
	struct t1 start;
	struct t2 end;
	int a=0;
	int r=0;
	int sum;
	int result[600];
	
//A
	for(a=0; a<100; a++) {
		gettimeofday(&start, NULL);
		testcaseA();
		gettimeofday(&end, NULL);
		sum = sum+((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
		result[r]=sum;
		r++;
	

//B
		sum=0;
		gettimeofday(&start, NULL);
		testcaseB();
		gettimeofday(&end, NULL);
		sum = sum+((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
		result[r]=sum;
                r++;

//C
		sum=0;
                gettimeofday(&start, NULL);
                testcaseC();
                gettimeofday(&end, NULL);
                sum = sum+((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
                result[r]=sum;
                r++;
//D		
		sum=0;
                gettimeofday(&start, NULL);
                testcaseD();
                gettimeofday(&end, NULL);
                sum = sum+((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
                result[r]=sum;
                r++;


		}



		int i;
		int ra=0;
		for(i=0;i<100;i++) {
			ra= ra+result[i];
		}
	
		printf("For Test Case A the average time is %d milliseconds\n", ra/100);


		int rb=0;
                for(i=0;i<100;i++) {
                        rb= rb+result[i];
                }

		 printf("For Test Case B the average time is %d milliseconds\n", rb/100);


		int rc=0;
                for(i=0;i<100;i++) {
                        rc= rc+result[i];
                }

                 printf("For Test Case C the average time is %d milliseconds\n", rc/100);


		int rd=0;
                for(i=0;i<100;i++) {
                        rd= rd+result[i];
                }

                 printf("For Test Case D the average time is %d milliseconds\n", rd/100);

		return 0;
		}
