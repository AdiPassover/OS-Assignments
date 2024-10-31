#include "MaxSubarrraySum.h"

void generateRandomArray(int* arr, size_t size, unsigned int seed) {
    srand(seed);
    for (size_t i = 0; i < size; i++) // Uniform distribution of random (integer) numbers in [MIN_VALUE, MAX_VALUE]
        arr[i] = (rand() % (MAX_VALUE + MIN_VALUE + 1)) + MIN_VALUE;
}

int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int maxSubarraySum_n(size_t size, int arr[]) {
    int maxSum = 0, sumSoFar = 0;
    for (size_t i = 0; i < size; i++) {
        sumSoFar = max(arr[i], arr[i]+sumSoFar);
        maxSum = max(maxSum, sumSoFar);
    }
    return maxSum;
}

int maxSubarraySum_n2(size_t size, int arr[]) {
    int maxSum = 0;
    for (size_t i = 0; i < size; i++) {
        int currSum = 0;
        for (size_t j = i; j < size; j++) {
            currSum += arr[j];
            if (maxSum < currSum) maxSum = currSum;
        }
    }
    return maxSum;
}

int maxSubarraySum_n3(size_t size, int arr[]) {
    int maxSum = 0;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = i; j < size; j++) {
            int currSum = 0;
            for (size_t k = i; k <= j; k++)
                currSum += arr[k];
            if (maxSum < currSum) maxSum = currSum;
        }
    }
    return maxSum;
}

void runProgram(int maxSubarraySum(size_t, int[]), int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid number of parameters\n");
        exit(1);
    }

    unsigned int seed = atoi(argv[1]);
    size_t size = (size_t)strtoull(argv[2], NULL, 10);;

    int* arr = (int*)malloc(sizeof(int) * size);
    if (arr == NULL) {
        perror("malloc failed");
        exit(1);
    }
    generateRandomArray(arr, size, seed);
    printf("For seed: %u, and size: %zu, the results are: %d ", seed, size, maxSubarraySum(size, arr));

    free(arr);
}