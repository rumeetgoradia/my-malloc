#include<stdio.h>
#include<stdlib.h>
#include "mymalloc.h"

/*BASE MAGIC NUMBER: 34922*/

size_t calc (unsigned char first, unsigned char second) {
	return (second << 8) + first;	 
}

void update (int index, size_t difference, int factor) {
	size_t size = calc(myblock[index-2], [index-1]);
	int i = index + calc_size;
	for (i = index + calc_size; i < 4096; i += calc_size) { 
		if (myblock[i + 5] == 'f' || myblock[i + 5] == 't') {
			size_t magic = calc(myblock[i + 1], myblock[i + 2]);
			magic += (difference * factor);
			myblock[i + 1] = magic;
			magic = magic >> 8;
			myblock[i + 2] = magic;
		} else {
			return;
		}
	}
}

void split (int index, size_t size, size_t available, size_t prev_magic) {
	size_t adjusted_magic = prev_magic + 5 + size;
	size_t remaining_size = available - (adjusted_magic - prev_magic);
	if (index + size < 4091) {
		update_magic(index, adjusted_magic - prev_magic, 1);
		myblock[index + size + 1] = (unsigned char) adjusted_magic;
		adjusted_magic = adjusted_magic >> 8;
		myblock[index + size + 2] = (unsigned char) adjusted_magic;
		myblock[index + size + 3] = (unsigned char) remaining_size;
		remaining_size = remaining_size >> 8;
		myblock[index + size + 4] = (unsigned char) remaining_size;
		myblock[index + size + 5] = 'f';
	} 
}

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
	split(4, size, 4091, 34927); //pass in index of in_use
	return (void *) (myblock + 5);
}

void * create(int index, size_t size, size_t available, size_t magic) {
	myblock[index - 1] = 't';
	if (available - size >= 5) {
		myblock[index - 3] = (unsigned char) size;
		size_t size2 = size >> 8;
		myblock[index - 2] = (unsigned char) size2;
		split(index - 1, size, available, magic);
	}
	return (void *) (myblock + i);
		
}

void * mymalloc (size_t size, char * file, size_t line) {
	if (calc(myblock[0], myblock[1]) != 34927) {
		return init(size, file, line);
	}
	int i = 5;
	size_t calc_size = calc(myblock[2], myblock[3]);
	size_t magic_num = calc(myblock[0], myblock[1]);
	for (i = 5; i < 4096; i += calc_size + 5) {
		if (calc_size >= size && myblock[i - 1] == 'f') {
			return create(i, size, calc_size, magic_num);
		}
		int next_size = calc(myblock[i + calc_size + 2], myblock[i + calc_size + 3]);
		if (myblock[i - 1] == 'f' && myblock[i + calc_size + 4] == 'f' && calc_size + next_size >= size) {
			coalesce(i, calc_size, next-size);
			return create(i, size, calc_size, magic_num);
		}
		magic_num = calc(myblock[i + calc_size], myblock[i + calc_size + 1];
		calc_size = next_size;
				
	}
	fprintf(stderr, "Error in file \"%s\" at line #%lu.\nNot enough memory. Please try again with a smaller size.\n", file, (unsigned long int) line);
	return NULL;
}
