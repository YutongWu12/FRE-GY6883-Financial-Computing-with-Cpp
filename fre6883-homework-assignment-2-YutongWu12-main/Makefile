CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

Homework02: Homework02.o BinomialTreeModel.o Option02.o
	$(CC) $(CFLAGS) -o Homework02 Homework02.o BinomialTreeModel.o Option02.o

Homework02.o: Homework02.cpp BinomialTreeModel.h Option02.h
	$(CC) $(CFLAGS) -c Homework02.cpp

Option02.o: BinomialTreeModel.h Option02.h Option02.cpp
	$(CC) $(CFLAGS) -c Option02.cpp
	
BinomialTreeModel.o: BinomialTreeModel.h BinomialTreeModel.cpp
	$(CC) $(CFLAGS) -c BinomialTreeModel.cpp
	
clean:
	rm -rf Homework02 *.o



