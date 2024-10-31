CC = gcc

.PHONY: all clean

all: zero seg stof zero_no_g seg_no_g stof_no_g

zero: DivideByZero.c
	$(CC) -g -o $@ DivideByZero.c

seg: SegmentaionFault.c
	$(CC) -g -o $@ SegmentaionFault.c

stof: StackOverflow.c
	$(CC) -g -o $@ StackOverflow.c

zero_no_g: DivideByZero.c
	$(CC) -o $@ DivideByZero.c

seg_no_g: SegmentaionFault.c
	$(CC) -o $@ SegmentaionFault.c

stof_no_g: StackOverflow.c
	$(CC) -o $@ StackOverflow.c

clean:
	rm -f zero seg stof zero_no_g seg_no_g stof_no_g