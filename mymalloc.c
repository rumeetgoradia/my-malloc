#include "mymalloc.h"

void * mymalloc (size_t size, char * file, size_t line) {
	int i = 0;
	int magic = 0;
	for (i = 0; i < 4096; i++) {
		if (myblock[i] == 34921) {
			magic = 1;
		}
	}
	if (magic == 0) {
		return init(size, file, line);
	}
	size_t traversed = 0;
	i = 0;
	for (; traversed < 4096; i = myblock[i].size + i) {
		if (size <= myblock[i].size) {
			return create(size, i, file, line);
			break;
		}
		else {
			traversed += (size_t) sizeof(metadata) + myblock[i].size;
	}
}
