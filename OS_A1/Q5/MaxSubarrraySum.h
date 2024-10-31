#include <stdio.h>
#include <stdlib.h>

#define MAX_VALUE 76
#define MIN_VALUE -25

void generateRandomArray(int* arr, size_t size, unsigned int seed);

int maxSubarraySum_n(size_t size, int arr[]);
int maxSubarraySum_n2(size_t size, int arr[]);
int maxSubarraySum_n3(size_t size, int arr[]);

void runProgram(int maxSubarraySum(size_t, int[]), int argc, char* argv[]);