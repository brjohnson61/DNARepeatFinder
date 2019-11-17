CC=gcc
OBJ=HashTableLinkedListCollision.o RepeatedDNASequence.o
SRC=HashTableLinkedListCollision.c RepeatedDNASequence.c
CFLAGS=-g -I -Wall
EXEC=sequencer

all: HashTableLinkedListCollision.o RepeatedDNASequence.o
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS)

.PHONY: clean
.PHONY: run

clean:
	rm sequencer *.o
run:
	./sequencer