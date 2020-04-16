gc.o: gc.c gc.h
	gcc -c gc.c
	
a.out: gc.o main.c gc.h
	gcc gc.o main.c