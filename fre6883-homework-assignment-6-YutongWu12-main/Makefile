CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

HW06: Homework06.o BinomialTreeModel02.o Option08.o
	$(CC) $(CFLAGS) -o Homework06 Homework06.o BinomialTreeModel02.o Option08.o

Homework06.o: Homework06.cpp BinomialTreeModel02.h Option08.h
	$(CC) $(CFLAGS) -c Homework06.cpp

Option08.o: BinomialTreeModel02.h BinLattice02.h Option08.h Option08.cpp
	$(CC) $(CFLAGS) -c Option08.cpp
	
BinomialTreeModel02.o: BinomialTreeModel02.h BinomialTreeModel02.cpp
	$(CC) $(CFLAGS) -c BinomialTreeModel02.cpp
	
clean:
	rm -rf HW06 *.o