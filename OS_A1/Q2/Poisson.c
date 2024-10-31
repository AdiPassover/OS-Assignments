#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LAMBDA_INDEX 1
#define K_INDEX 2

int toLongDouble(const char* str, long double* ld) {
    char *end;
    *ld = strtold(str, &end);
    return *end == 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Error\n");
        exit(1);
    }

    long double lambda;
    if (!toLongDouble(argv[LAMBDA_INDEX], &lambda) || lambda < 0) {
        printf("Error: lambda is illegal\n");
        exit(1);
    }

    long unsigned int k = atoi(argv[K_INDEX]);

    long double ans = pow(lambda,k) * expf(-1*lambda) / tgamma(k+1);
    printf("P_%ld (%Lf) = %.12Lf\n", k, lambda, ans);

    return 0;
}