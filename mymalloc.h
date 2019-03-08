#include<stdlib.h>

#define malloc( x ) mymalloc ( x, __FILE__, __LINE__ )
#define free( x ) myfree ( x, __FILE__, __LINE__)

void* mymalloc(size_t req, char* file, size_t line);
void myfree(void* addr, char* file, size_t line);

static unsigned char myblock[4096];

/*typedef struct metadata {
*	size_t in_use;
*	size_t size;
*	struct metadata * next;
} metadata; */
