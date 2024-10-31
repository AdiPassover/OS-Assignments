CXX = g++
CXXFLAGS = -Wall -g -fprofile-arcs -ftest-coverage

TARGET = dijkstra
SRCS = Main.cpp Graph.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

coverage: $(TARGET)
	./$(TARGET) < ./tests/goodInput1.txt > /dev/null || true;
	./$(TARGET) < ./tests/goodInput2.txt > /dev/null || true;
	./$(TARGET) < ./tests/negativeSize.txt > /dev/null || true;
	./$(TARGET) < ./tests/negativeWeights.txt > /dev/null || true;
	./$(TARGET) < ./tests/notEnoughEdges.txt > /dev/null || true;
	./$(TARGET) < ./tests/notNumbers.txt > /dev/null || true;
	./$(TARGET) < ./tests/selfEdges.txt > /dev/null || true;
	./$(TARGET) < ./tests/tooManyEdges.txt > /dev/null || true;
	./$(TARGET) < ./tests/wrongStartingVertex.txt > /dev/null || true;

	gcov Main.cpp > coverage_report.txt 2>&1
	gcov Graph.cpp >> coverage_report.txt 2>&1

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.gcda *.gcno *.gcov coverage_report.txt
