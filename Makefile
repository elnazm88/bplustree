CC = g++

all: bptree
		./bptree
hashtable: bptree.o
		g++ -Wall -o bptree bptree.o

clean:
	rm -f *.o *~ *.dat
