CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

TARGET = main
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

run: $(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run