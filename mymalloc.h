#include<stdlib.h>

#define malloc( x ) mymalloc ( x, __FILE__, __LINE__ )
#define free( x ) myfree ( x, __FILE__, __LINE__)

void* mymalloc(size_t size, char* file, size_t line);
void myfree(void* input, char* file, size_t line);

static unsigned char myblock[4096];
