CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

TARGET = arboles_grafos
SRCS = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRCS) include/BST.hpp include/RedBlackTree.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean

