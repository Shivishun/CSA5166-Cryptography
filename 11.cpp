#include <stdio.h>
#include <math.h>

int main() {
    // Calculating 25 factorial
    double factorial = 1;
    for (int i = 1; i <= 25; i++) {
        factorial *= i;
    }

    // Calculating log base 2 of 25 factorial
    double logBase2 = log2(factorial);

    // Output the result as an approximate power of 2
    printf("Number of possible keys: 2^%.2f\n", logBase2);

    return 0;
}

