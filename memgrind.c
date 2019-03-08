#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>



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
	for (a=0; a<150; a==) {
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
	
	char *arr[150];
	for(p=0;p<150;p++) {
		arr[p] = NULL;

		}
	while (a<150) {
		n = (double)rand() / (double)RAND_MAX;
		if





void testcaseD() {




