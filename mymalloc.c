/* Aditi Singh and Rumeet Goradia
 * as2811, rug5
 * CS214, Section 4
 */ 

#include<stdio.h>
#include<stdlib.h>
#include "mymalloc.h"

/*MAGIC NUMBER: 34927*/

/* calc() --> returns the sum of two passed bytes, assumes second is more significant byte */
size_t calc (unsigned char first, unsigned char second) {
	return (second << 8) + first;	 
}

/* split() --> create new metadata block after the current one
 *	1. find the difference between available bytes and sum of metadata and allocated bytes
 *	2. set new block to free metadata with remaining size if still space in array for metadata
 */
void split (int index, size_t size, size_t available) {
	size_t remaining_size = available - (3 + size);
	if (index + size < 4093) {
		myblock[index + size + 1] = (unsigned char) remaining_size;
		remaining_size = remaining_size >> 8;
		myblock[index + size + 2] = (unsigned char) remaining_size;
		myblock[index + size + 3] = 'f';
	} 
}
/* coalesce() --> combine two free chunks
 *	1. find sum of current pointer's size, next pointer's size, and size of metadata
 *	2. set current pointer's size to this sum
 */ 
void coalesce (char * ptr, size_t curr_size, size_t next_size) {
	size_t new_size = curr_size + next_size + 3;
	*(ptr - 3) = (unsigned char) new_size;
	new_size = new_size >> 8;
	*(ptr - 2) = (unsigned char) new_size;
}
/* init() --> first malloc call, return first pointer
 * 	1. ensure there is enough space for necessary metadata
 * 	2. set magic number so that init isn't called again
 * 	3. set current block's size and change to in-use
 * 	4. call split to form new metadata block
 * 	5. return pointer to first index after metadata
 */
void * init (size_t size, char * file, size_t line) {
	if (size > 4091) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nNot enough memory. Please try again with a smaller size.\n", file, (unsigned long int) line);
		return NULL;
	} 
	myblock[0] = (unsigned char) 34927;
	size_t magic2 = 34927 >> 8;
	myblock[1] = (unsigned char) magic2;
	myblock[2] = (unsigned char) size;
	size_t size2 = size >> 8;
	myblock[3] = (unsigned char) size2;
	myblock[4] = 't';
	split(4, size, 4091);
	return (void *) (myblock + 5);
}

/* create() --> return new pointer
 *	1. set pointer to in-use
 *	2. only proceed to change metadata if there is enough space to create new metadata
 *	3. set size in metadata
 *	4. call split to form metadata block
 *	5. return pointer to first index after metadata
 */ 
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

/* mymalloc() --> dynamically allocate pointers using 3-byte metadata
 *	1. ensure size is valid
 *	2. check for magic number; if none, return init()'s return pointer
 *	3. traverse through current list of pointers, incrementing by current pointer's size each time
 *	4. if pointer is free and has enough space, return create()'s return pointer
 *	5. check if possible to coalesce; if possible, call coalesce() and return create()'s return pointer
 *	6. if reached end of loop, either no free space or just not enough space for this pointer, return NULL
 */ 
void * mymalloc (size_t size, char * file, size_t line) {
	if (size <= 0) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid size for dynamic allocation. Please try again.\n", file, (unsigned long int) line);
	} 
	if (calc((unsigned char) myblock[0], (unsigned char) myblock[1]) != 34927) {
		return init(size, file, line);
	}
	int i = 5;
	size_t calc_size = calc((unsigned char) myblock[2], (unsigned char) myblock[3]);
	size_t old_size = calc_size;
	//establish variable to keep error messages specific --> no free space at all, or just no space
	int any_free = 0;
	for (i = 5; i < 4096; i += old_size + 3) {
		old_size = calc_size;
		if (myblock[i - 1] == 'f' && calc_size != 0) {
			any_free = 1;
		}
		if (calc_size >= size && myblock[i - 1] == 'f') {
			return create(i, size, calc_size);
		}
		if (i + calc_size < 4095) {
			int next_size = calc((unsigned char) myblock[i + calc_size], (unsigned char) myblock[i + calc_size + 1]);
			int coalesce_index = i;
			int sum = calc_size;
			int coalesce_check = 0;
			while ((coalesce_index + calc_size + 2 < 4096) && (myblock[coalesce_index - 1] == 'f') && (myblock[coalesce_index + calc_size + 2] == 'f')) {
			//add the sizes of all subsequent free blocks, not just the first one
				coalesce_check = 1;
				sum += next_size + 3;
				coalesce_index += calc_size + 3;
				calc_size = calc((unsigned char) myblock[coalesce_index - 3], (unsigned char)myblock[coalesce_index - 2]);
				if (coalesce_index + calc_size < 4095) {
					next_size = calc((unsigned char) myblock[coalesce_index + calc_size], (unsigned char) myblock[coalesce_index + calc_size + 1]);
				}
			}
			if (coalesce_check) {
				coalesce((char *)(myblock + i), old_size, sum);
				calc_size = calc((unsigned char)myblock[i - 3], (unsigned char)myblock[i - 2]);
				if (calc_size >= size) {
					return create(i, size, calc_size);
				}
			}
			calc_size = calc((unsigned char) myblock[i + calc_size], (unsigned char) myblock[i + calc_size + 1]);
		}
	}
	if (any_free == 1) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nNo space large enough to contain this pointer. Please try again with a smaller size.\n", file, (unsigned long int) line);
	} else {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nNo free space. Free some pointers to create space for this allocation.\n", file, (unsigned long int) line);
	}
	return NULL;
}
/* myfree() --> sets current pointer to not in use
 *	1. run many tests to ensure passed-in pointer is valid
 *	2. if all tests passed, set pointer's in-use byte to false in its metadata
 *	3. check if possible to coalesce, and do so if yes
 */ 
void myfree (void * input, char * file, size_t line) {
	char * ptr = (char *) input;
	if (ptr < (char *) myblock || ptr >= (char *) myblock + 4096 || (*(ptr - 1) != 't' && *(ptr - 1) != 'f')) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid use of free(). This pointer was not dynamically allocated through malloc().\n", file, (unsigned long int) line);
		return;
	} if (ptr == NULL) {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid use of free(). This pointer is NULL.\n", file, (unsigned long int) line);
		return;	
	} if (*(ptr - 1) == 'f') {
		fprintf(stderr, "Error in file \"%s\" at line #%lu.\nInvalid use of free(). This pointer was recently freed without being dynamically allocated again.\n", file, (unsigned long int) line);
		return;
	} if (*(ptr-1) == 't') {
		*(ptr-1) = 'f';
		size_t size = calc((unsigned char) *(ptr-3), (unsigned char) *(ptr-2));
		//check if possible to coalesce
		if ((ptr + size + 2) < (char *)(myblock + 4096) && (ptr + size + 2) > (char *)(myblock) && *(ptr + size + 2) == 'f') {
			size_t next_size = calc((unsigned char) *(ptr + size), (unsigned char) *(ptr + size + 1));
			coalesce(ptr, size, next_size);
		}
	}
}
