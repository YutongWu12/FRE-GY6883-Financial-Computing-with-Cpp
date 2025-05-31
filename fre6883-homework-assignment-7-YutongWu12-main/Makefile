# Makefile

CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Object files
OBJS = BinomialTreeModel02.o Option08.o OptionPricer08.o

# Target
TARGET = OptionPricer08

# Default build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile each .cpp file into .o
BinomialTreeModel02.o: BinomialTreeModel02.cpp BinomialTreeModel02.h BinLattice02.h BSModel.h
	$(CXX) $(CXXFLAGS) -c BinomialTreeModel02.cpp

Option08.o: Option08.cpp Option08.h
	$(CXX) $(CXXFLAGS) -c Option08.cpp

OptionPricer08.o: OptionPricer08.cpp BinomialTreeModel02.h Option08.h
	$(CXX) $(CXXFLAGS) -c OptionPricer08.cpp

# Clean command
clean:
	rm -f *.o $(TARGET)
