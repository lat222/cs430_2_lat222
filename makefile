CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -c
LFLAGS = -Wall -std=c99

raycast : mainprog.o ppmformatter.o raycast.o linkedlist.o 
	$(CC) $(LFLAGS) mainprog.o ppmformatter.o raycast.o linkedlist.o -o raycast

mainprog.o : mainprog.c ppmformatter.h raycast.h linkedlist.h
	$(CC) $(CFLAGS) mainprog.c

ppmformatter.o : ppmformatter.c ppmformatter.h linkedlist.h
	$(CC) $(CFLAGS) ppmformatter.c

raycast.o: raycast.c raycast.h linkedlist.h
	$(CC) $(CFLAGS) raycast.c

linkedlist.o : linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) linkedlist.c


clean:
	\rm *.o run