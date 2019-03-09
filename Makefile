all: memgrind.c mymalloc.o
	gcc -fsanitize=address -o memgrind memgrind.c mymalloc.o
mymalloc.o: mymalloc.c
	gcc -c mymalloc.c
clean:
	rm -f memgrind
	rm -f mymalloc.o
