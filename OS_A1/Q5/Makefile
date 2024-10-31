CC = gcc
CFLAGS = -Wall -Wextra -pg

SRC = MaxSubarraySum.c
OBJ = $(SRC:.c=.o)

EXE1 = maxsumn
EXE2 = maxsumn2
EXE3 = maxsumn3

MAIN1 = Main_On.c
MAIN2 = Main_On2.c
MAIN3 = Main_On3.c
MAIN_OBJ1 = $(MAIN1:.c=.o)
MAIN_OBJ2 = $(MAIN2:.c=.o)
MAIN_OBJ3 = $(MAIN3:.c=.o)

all: $(EXE1) $(EXE2) $(EXE3)

$(EXE1): $(OBJ) $(MAIN_OBJ1)
	$(CC) $(CFLAGS) -o $@ $^

$(EXE2): $(OBJ) $(MAIN_OBJ2)
	$(CC) $(CFLAGS) -o $@ $^

$(EXE3): $(OBJ) $(MAIN_OBJ3)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

profiling: $(EXE1) $(EXE2) $(EXE3)
	./$(EXE1) 42 100
	mv gmon.out gmon_n_100
	gprof $(EXE1) gmon_n_100 > analysis_n_100.txt
	./$(EXE1) 42 1000
	mv gmon.out gmon_n_1000
	gprof $(EXE1) gmon_n_1000 > analysis_n_1000.txt
	./$(EXE1) 42 10000
	mv gmon.out gmon_n_10000
	gprof $(EXE1) gmon_n_10000 > analysis_n_10000.txt

	./$(EXE2) 42 100
	mv gmon.out gmon_n2_100
	gprof $(EXE2) gmon_n2_100 > analysis_n2_100.txt
	./$(EXE2) 42 1000
	mv gmon.out gmon_n2_1000
	gprof $(EXE2) gmon_n2_1000 > analysis_n2_1000.txt
	./$(EXE2) 42 10000
	mv gmon.out gmon_n2_10000
	gprof $(EXE2) gmon_n2_10000 > analysis_n2_10000.txt

	./$(EXE3) 42 100
	mv gmon.out gmon_n3_100
	gprof $(EXE3) gmon_n3_100 > analysis_n3_100.txt
	./$(EXE3) 42 1000
	mv gmon.out gmon_n3_1000
	gprof $(EXE3) gmon_n3_1000 > analysis_n3_1000.txt
	./$(EXE3) 42 10000
	mv gmon.out gmon_n3_10000
	gprof $(EXE3) gmon_n3_10000 > analysis_n3_10000.txt

clean:
	rm -f *.o $(EXE1) $(EXE2) $(EXE3)

.PHONY: all clean
