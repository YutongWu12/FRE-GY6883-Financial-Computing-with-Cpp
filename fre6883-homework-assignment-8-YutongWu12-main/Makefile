# 编译器
CXX = g++
# 编译选项
CXXFLAGS = -std=c++11 -Wall -g

# 最终程序名
TARGET = main02

# 所有的源文件和头文件
SRCS = main02.cpp Function02.cpp NonlinearSolver02.cpp
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# 编译每个 cpp 文件成 o 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理命令
clean:
	rm -f $(OBJS) $(TARGET)
