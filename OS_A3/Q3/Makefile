CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g # -pg

MAIN = Main.cpp
SRCS = Point.cpp ConvexHull.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = convex

all: $(TARGET)

$(TARGET): $(OBJS) $(MAIN:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(OBJS) $(TEST:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)

.PHONY: all clean