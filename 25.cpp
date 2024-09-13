#include <stdio.h>
#include <stdint.h>
#include <math.h>

// Function prototypes
uint32_t gcd(uint32_t a, uint32_t b);
uint32_t trial_factorize(uint32_t n);
uint32_t mod_inverse(uint32_t a, uint32_t m);
uint32_t mod_exp(uint32_t base, uint32_t exp, uint32_t mod);

int main() {
    uint32_t n = 3233; // Example n = p * q
    uint32_t e = 17;   // Example public exponent
    uint32_t m = 1234; // Example plaintext block with a common factor with n

    printf("Given n: %u\n", n);
    printf("Public key e: %u\n", e);
    printf("Plaintext block m: %u\n", m);

    // Compute gcd(m, n)
    uint32_t common_factor = gcd(m, n);

    printf("Common factor of m and n: %u\n", common_factor);

    if (common_factor == 1 || common_factor == n) {
        printf("No useful common factor found.\n");
    } else {
        // Determine p and q
        uint32_t p = common_factor;
        uint32_t q = n / p;

        printf("Factors of n: p = %u, q = %u\n", p, q);

        // Compute phi(n)
        uint32_t phi_n = (p - 1) * (q - 1);
        printf("Euler's totient function phi(n): %u\n", phi_n);

        // Compute private key d
        uint32_t d = mod_inverse(e, phi_n);
        printf("Private key d: %u\n", d);
    }

    return 0;
}

// Compute gcd using Euclidean algorithm
uint32_t gcd(uint32_t a, uint32_t b) {
    while (b != 0) {
        uint32_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Modular multiplicative inverse
uint32_t mod_inverse(uint32_t a, uint32_t m) {
    int32_t x, y;
    uint32_t g = gcd_extended(a, m, &x, &y);
    if (g != 1) {
        printf("Modular inverse does not exist.\n");
        return 0;
    }
    return (x % m + m) % m;
}

// Extended Euclidean algorithm
uint32_t gcd_extended(uint32_t a, uint32_t b, int32_t *x, int32_t *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    int32_t x1, y1;
    uint32_t gcd = gcd_extended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

