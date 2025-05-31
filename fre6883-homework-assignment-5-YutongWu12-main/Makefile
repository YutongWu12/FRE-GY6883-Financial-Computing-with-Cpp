CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

# Define object files
OBJS = OptionPricer05.o BinomialTreeModel02.o Option05.o DoubDigitOpt.o StrangleOpt.o ButterflyOpt.o

# Define headers
HEADERS = BinomialTreeModel02.h Option05.h DoubDigitOpt.h StrangleOpt.h ButterflyOpt.h

# Target executable
OptionPricer05: $(OBJS)
	$(CC) $(CFLAGS) -o OptionPricer05 $(OBJS)

# Compile individual object files
OptionPricer05.o: OptionPricer05.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c OptionPricer05.cpp

Option05.o: Option05.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c Option05.cpp

DoubDigitOpt.o: DoubDigitOpt.cpp DoubDigitOpt.h Option05.h
	$(CC) $(CFLAGS) -c DoubDigitOpt.cpp

StrangleOpt.o: StrangleOpt.cpp StrangleOpt.h Option05.h
	$(CC) $(CFLAGS) -c StrangleOpt.cpp

ButterflyOpt.o: ButterflyOpt.cpp ButterflyOpt.h Option05.h
	$(CC) $(CFLAGS) -c ButterflyOpt.cpp

BinomialTreeModel02.o: BinomialTreeModel02.cpp BinomialTreeModel02.h
	$(CC) $(CFLAGS) -c BinomialTreeModel02.cpp

# Clean target
.PHONY: clean
clean:
	rm -rf OptionPricer05 $(OBJS)
