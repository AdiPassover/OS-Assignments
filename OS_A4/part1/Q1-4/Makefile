CXX = g++
CXXFLAGS = -Wall -std=c++11 -g

TARGET = euler
SOURCES = Graph.cpp Main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean clean_all coverage valgrind_memcheck valgrind_callgraph profiling

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

Graph.o: Graph.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c Graph.cpp

Main.o: Main.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) -c Main.cpp

clean:
	rm -f *.o $(TARGET) gmon.out callgrind.out.* cachegrind.out.* coverage.info -r *.gcda *.gcno

clean_all:
	make clean
	rm -f *.gcov

# coverage
CXXFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -lgcov --coverage
coverage: $(TARGET)
	./$(TARGET) -n 100 -m 200 -s 42 > /dev/null
	./$(TARGET) -n 100 -m 200 -s 41 > /dev/null
	./$(TARGET) -n 1000 -m 4000 -s 40 > /dev/null
	./$(TARGET) -n 99 -m 498501 -s 39 > /dev/null || true
	./$(TARGET) -n 99 -m 4851 -s 39 > /dev/null
	./$(TARGET) -n 99 -m 4851 -s 39 -h error_on_purpose > /dev/null || true
	./$(TARGET) -n 99 -m 4851 -h error_on_purpose > /dev/null || true

	gcov Main.cpp > reports/coverage_report.txt 2>&1
	gcov Graph.cpp >> reports/coverage_report.txt 2>&1
	rm -f *.h.gcov *.tcc.gcov new.gcov *.gcda *.gcno

# valgrind
valgrind_memcheck: $(TARGET)
	valgrind --tool=memcheck -v --leak-check=full --show-leak-kinds=all --log-file=reports/valgrind_memcheck.txt ./$(TARGET) -n 99 -m 4851 -s 39 > /dev/null

valgrind_callgraph: $(TARGET)
	chmod 0700 /run/user/1000/
	export $(dbus-launch)
	valgrind --tool=callgrind ./$(TARGET) -n 99 -m 4851 -s 39 > /dev/null
	kcachegrind callgrind.out.*

# profiling
profiling: CXXFLAGS += -pg
profiling: $(TARGET)
	./$(TARGET) -n 499 -m 124251 -s 42 > /dev/null
	gprof $(TARGET) gmon.out > reports/profiling_report.txt
	rm -f gmon.out