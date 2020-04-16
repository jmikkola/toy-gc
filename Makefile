gc.o: gc.c gc.h
	gcc -Wall -c gc.c
	
a.out: gc.o main.c gc.h
	gcc -Wall gc.o main.c