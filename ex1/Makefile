CC = gcc
CFLAGS = -Wall -Wextra -g

all: ex1_filter ex1_loadAndSave

ex1_filter: ex1_filter.c
	$(CC) $(CFLAGS) -o ex1_filter ex1_filter.c

ex1_loadAndSave: ex1_loadAndSave.c
	$(CC) $(CFLAGS) -o ex1_loadAndSave ex1_loadAndSave.c

clean:
	rm -f ex1_filter ex1_loadAndSave
