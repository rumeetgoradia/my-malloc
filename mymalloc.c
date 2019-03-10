#include<stdio.h>
#include<stdlib.h>
#include "mymalloc.h"

/*BASE MAGIC NUMBER: 34927*/

size_t calc (unsigned char first, unsigned char second) {
	return (second << 8) + first;	 
}
/*
void update_magic (int index, size_t difference) {
	size_t size = calc(myblock[index-2], [index-1]);
	int i = index + calc_size;
	for (i = index + calc_size; i < 4096; i += calc_size) { 
		if (myblock[i + 5] == 'f' || myblock[i + 5] == 't') {
			size_t magic = calc(myblock[i + 1], myblock[i + 2]);
			magic += difference;
			myblock[i + 1] = magic;
			magic = magic >> 8;
			myblock[i + 2] = magic;
		} else {
			return;
		}
	}
} */

void split (int index, size_t size, size_t available) {
//	size_t adjusted_magic = prev_magic + 5 + size;
	size_t remaining_size = available - (3 + size);
//	printf("SPLIT: %lu\n", remaining_size);
	if (index + size < 4091) {
//		update_magic(index, adjusted_magic - prev_magic, 1);
//		myblock[index + size + 1] = (unsigned char) adjusted_magic;
//		adjusted_magic = adjusted_magic >> 8;
//		myblock[index + size + 2] = (unsigned char) adjusted_magic;
		myblock[index + size + 1] = (unsigned char) remaining_size;
		remaining_size = remaining_size >> 8;
		myblock[index + size + 2] = (unsigned char) remaining_size;
		myblock[index + size + 3] = 'f';
	} 
}

void coalesce (char * ptr, size_t curr_size, size_t next_size) {
	size_t new_size = curr_size + next_size + 3;
	*(ptr - 3) = (unsigned char) new_size;
	new_size = new_size >> 8;
	*(ptr - 2) = (unsigned char) new_size;
}

void * init (size_t size, char * file, size_t line) {
	if (size > 4093) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nNot enough memory. Please try again with a smaller size.\n", file, (unsigned long int) line);
		return NULL;
	} 
//	printf("SIZE: %lu\n", size);
	myblock[0] = (unsigned char) 34927;
	size_t magic2 = 34927 >> 8;
	myblock[1] = (unsigned char) magic2;
	myblock[2] = (unsigned char) size;
	size_t size2 = size >> 8;
	myblock[3] = (unsigned char) size2;
	myblock[4] = 't';
//	printf("first two size bytes: %X %X\n", myblock[2], myblock[3]);
	split(4, size, 4091); //pass in index of in_use
	return (void *) (myblock + 5);
}

void * create (int index, size_t size, size_t available) {
	myblock[index - 1] = 't';
	if (available - size >= 3) {
		myblock[index - 3] = (unsigned char) size;
		size_t size2 = size >> 8;
		myblock[index - 2] = (unsigned char) size2;
		split(index - 1, size, available);
	}
	return (void *) (myblock + index);	
}

void * mymalloc (size_t size, char * file, size_t line) {
	if (size <= 0) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid size for dynamic allocation. Please try again.\n", file, (unsigned long int) line);
	} 
	if (calc((unsigned char) myblock[0], (unsigned char) myblock[1]) != 34927) {
		return init(size, file, line);
	}
	int i = 5;
//	printf("first two size bytes: %X %X\n", myblock[2], myblock[3]);
	size_t calc_size = calc((unsigned char) myblock[2], (unsigned char) myblock[3]);
	size_t old_size = calc_size;
//	printf("SIZE 0: %lu\n", calc_size);
//	size_t magic_num = calc((unsigned char) myblock[0], (unsigned char) myblock[1]);
	for (i = 5; i < 4096; i += old_size + 3) {
		old_size = calc_size;
//		printf("flag: %c\n", myblock[i - 1]);
		if (calc_size >= size && myblock[i - 1] == 'f') {
			return create(i, size, calc_size);
		}
//		printf("got past first if\n");
		int next_size = calc((unsigned char) myblock[i + calc_size], (unsigned char) myblock[i + calc_size + 1]);
//		printf("got past next_size\n");
		if (myblock[i - 1] == 'f' && myblock[i + calc_size + 2] == 'f' && calc_size + next_size + 3 >= size) {
			coalesce((char *)(myblock + i), calc_size, next_size);
			return create(i, size, calc_size + next_size + 3);
		}
//		printf("got past potential coalesce\n");
//		magic_num = calc((unsigned char) myblock[i + calc_size], (unsigned char) myblock[i + calc_size + 1];
		calc_size = next_size;			
//		printf("SIZE %d: %lu\n", i, calc_size);
	}
	fprintf(stderr, "Error in file \"%s\" at line #%lu.\nNot enough memory. Please try again with a smaller size.\n", file, (unsigned long int) line);
	return NULL;
}

void myfree (void * ptr, char * file, size_t line) {
//	printf("ptr: %x\n", (char *) ptr);
	if ((char* )ptr < (char *) myblock || (char *) ptr >= (char *) (myblock + 4096) || ((*((char *)(ptr - 1)) != 't' && *((char*)(ptr - 1)) != 'f'))) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid use of free(). This pointer was not dynamically allocated through malloc().\n", file, (unsigned long int) line);
		return;
	} if ((char *) ptr == NULL) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid use of free(). This pointer is NULL.\n", file, (unsigned long int) line);
		return;	
	} if (*((char *) ptr - 1) == 'f') {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid use of free(). This pointer was recently freed without being dynamically allocated again.\n", file, (unsigned long int) line);
		return;
	} if (*((char *)(ptr-1)) == 't') {
		*((char *) (ptr-1)) = 'f';
		size_t size = calc((unsigned char) *((char *)(ptr-3)), (unsigned char) *((char *)(ptr-2)));
		if (*((char *)(ptr + size + 2)) == 'f') {
			size_t next_size = calc((unsigned char) *((char *)(ptr + size)), (unsigned char) *((char *)(ptr + size + 1)));
			coalesce((char *)ptr, size, next_size);
		}
//		printf("setting add %x to NULL\n", (char *) ptr);
	}
}
