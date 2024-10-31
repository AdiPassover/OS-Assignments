CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g #-fprofile-arcs -ftest-coverage ## comment fprofile and ftest when running valgrind
LDFLAGS = -lgcov --coverage

DS_SRC = data_structures/fibonacci_heap.cpp data_structures/union_find.cpp
MST_SRC = graph/mst_algo/kruskal.cpp graph/mst_algo/prim.cpp
GRAPH_SRC = graph/graph.cpp graph/tree.cpp
SERVER_SRC = servers/server.cpp servers/client_commands.cpp
LF_SRC = servers/leader_follower/lf_handler.cpp servers/leader_follower/lf_server.cpp
PIPE_SRC = servers/pipeline/pipeline_server.cpp # servers/pipeline/pipeline_handler.cpp
MAIN_OBJ = main.o
SRC = $(DS_SRC) $(MST_SRC) $(GRAPH_SRC) $(SERVER_SRC) $(LF_SRC) $(PIPE_SRC)
OBJ = $(SRC:.cpp=.o)

TEST_SRC = tests/test_main.cpp tests/test_data_structures.cpp tests/test_mst_algos.cpp tests/test_handlers.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

TARGET = server
TEST_TARGET = test_exe
TEST_RUN = test

ALL_OBJ = $(MAIN_OBJ) $(OBJ) $(TEST_OBJ)

.PHONY: run $(TEST_RUN) all clean coverage clean_coverage clean_all memcheck helgrind callgrind

run: $(TARGET)
	./$(TARGET)

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(MAIN_OBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_RUN): $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(ALL_OBJ) $(TARGET) $(TEST_TARGET)

coverage: all
	./coverage.sh -l
	./coverage.sh -p
	./$(TEST_TARGET)
	./$(TARGET) -error_on_purpose > /dev/null || true
	./$(TARGET) -l -p > /dev/null || true
	echo "q" | ./$(TARGET) > /dev/null || true
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*include/c++/*' '*doctest.h' --output-file filtered_coverage.info
	genhtml filtered_coverage.info --output-directory coverage_report

memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --log-file=memcheck.txt ./$(TARGET)

helgrind: $(TARGET)
	valgrind -v --tool=helgrind --log-file=valgrind_reports/pipe_helgrind.txt ./coverage.sh -p
	valgrind -v --tool=helgrind --log-file=valgrind_reports/lf_helgrind.txt ./coverage.sh -l

#cachegrind: $(TARGET)
#	valgrind -v --tool=cachegrind --log-file=valgrind_reports/pipe_cachegrind.txt ./coverage.sh -p
#	valgrind -v --tool=cachegrind --log-file=valgrind_reports/lf_cachegrind.txt ./coverage.sh -l

callgrind: $(TARGET)  ## to display: kcachegrind <file_name>
	chmod 0700 /run/user/1000/
	export $(dbus-launch)
	valgrind --tool=callgrind ./$(TARGET)

clean_coverage:
	rm -rf $(ALL_OBJ:.o=.gcda) $(ALL_OBJ:.o=.gcno) filtered_coverage.info coverage.info

clean_all: clean clean_coverage