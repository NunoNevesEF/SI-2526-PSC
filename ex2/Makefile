CC = gcc
CFLAGS = -Wall -Wextra -g

all: ex2_loadAndSave ex2_filter

ex2_loadAndSave: ex2_loadAndSave.o table.o
	$(CC) $(CFLAGS) -o ex2_loadAndSave ex2_loadAndSave.o table.o -lcsv

ex2_filter: ex2_filter.o table.o
	$(CC) $(CFLAGS) -o ex2_filter ex2_filter.o table.o -lcsv

ex2_loadAndSave.o: ex2_loadAndSave.c table.h
	$(CC) $(CFLAGS) -c ex2_loadAndSave.c

ex2_filter.o: ex2_filter.c table.h
	$(CC) $(CFLAGS) -c ex2_filter.c

table.o: table.c table.h
	$(CC) $(CFLAGS) -c table.c

clean:
	rm -f *.o ex2_loadAndSave ex2_filter
