all: memgrind.c mymalloc.h mymalloc.o
	gcc -Wall -Werror -fsanitize=address -o memgrind memgrind.c
mymalloc.o: mymalloc.c
	gcc -c mymalloc.c
