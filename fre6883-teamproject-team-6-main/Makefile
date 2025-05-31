# by Hao Weng
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -pthread

# Executable
TARGET = main

# Source files
SRCS = main.cpp \
       CAARProcessor.cpp \
       EodMarketDatam.cpp \
       MarketCalendar.cpp \
       Stock.cpp \
       StockGroupSplitter.cpp \
       Plot.cpp \
       Result.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Include path
INCLUDES = -I.

# Final target with curl linked
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lcurl

# Compile rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule
.PHONY: clean
clean:
	rm -f $(TARGET) *.o
