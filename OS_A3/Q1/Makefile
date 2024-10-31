CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g # -pg

MAIN = Main.cpp
TEST = Test.cpp
SRCS = Point.cpp ConvexHull.cpp GrahamScan.cpp MonotoneChain.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = convex
TEST_TARGET = test

#run: $(TARGET)
#	./$(TARGET)

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJS) $(MAIN:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(OBJS) $(TEST:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)

.PHONY: all clean