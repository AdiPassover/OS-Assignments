#include "Poisson.h"

void Poisson(long double lambda, unsigned int k) {
    long double ans = pow(lambda,k) * expf(-1*lambda) / tgamma(k+1);
    printf("P_%u (%Lf) = %.12Lf\n", k, lambda, ans);
}