CC = gcc
CFLAGS = -Wall -Wextra -g

all: main

main: main.o table.o comandos.o
	$(CC) $(CFLAGS) -o main main.o table.o comandos.o -lcsv -ldl -rdynamic

main.o: main.c table.h comandos.h
	$(CC) $(CFLAGS) -c main.c

table.o: table.c table.h
	$(CC) $(CFLAGS) -c table.c

comandos.o: comandos.c comandos.h table.h	# dependency from comandos to table
	$(CC) $(CFLAGS) -c comandos.c -ldl

clean:
	rm -f *.o main