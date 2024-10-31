#include "Poisson.h"

int main() {

    long double lambdas[] = {2,2,2,3,100};
    unsigned int ks[] = {1,10,2,3,3};

    for (unsigned int i = 0; i < 5; i++)
        Poisson(lambdas[i], ks[i]);

    return 0;
}