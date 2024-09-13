#include <stdio.h>
#include <stdint.h>

// Function prototypes
uint32_t gcd_extended(uint32_t a, uint32_t b, int32_t *x, int32_t *y);
uint32_t mod_inverse(uint32_t a, uint32_t m);
uint32_t mod_exp(uint32_t base, uint32_t exp, uint32_t mod);
uint32_t trial_factorize(uint32_t n);
void rsa_encrypt_decrypt(uint32_t msg, uint32_t key, uint32_t n, uint32_t *result);

int main() {
    // Given public key parameters
    uint32_t e = 31;
    uint32_t n = 3599;

    // Determine p and q
    uint32_t p = trial_factorize(n);
    uint32_t q = n / p;

    // Compute Euler's totient function
    uint32_t phi_n = (p - 1) * (q - 1);

    // Compute the private key d
    uint32_t d = mod_inverse(e, phi_n);

    printf("Public key (e, n): (%u, %u)\n", e, n);
    printf("Private key (d): %u\n", d);

    // Example encryption and decryption
    uint32_t msg = 1234; // Example message
    uint32_t encrypted, decrypted;

    rsa_encrypt_decrypt(msg, e, n, &encrypted);
    rsa_encrypt_decrypt(encrypted, d, n, &decrypted);

    printf("Original message: %u\n", msg);
    printf("Encrypted message: %u\n", encrypted);
    printf("Decrypted message: %u\n", decrypted);

    return 0;
}

// Extended Euclidean algorithm to find gcd and coefficients
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

// Modular multiplicative inverse
uint32_t mod_inverse(uint32_t a, uint32_t m) {
    int32_t x, y;
    uint32_t gcd = gcd_extended(a, m, &x, &y);
    if (gcd != 1) {
        printf("Modular inverse does not exist.\n");
        return 0;
    }
    return (x % m + m) % m;
}

// Modular exponentiation
uint32_t mod_exp(uint32_t base, uint32_t exp, uint32_t mod) {
    uint32_t result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Trial factorization to find p and q
uint32_t trial_factorize(uint32_t n) {
    for (uint32_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return i;
        }
    }
    return 0; // Should not happen if n is a product of two primes
}

// RSA encryption and decryption
void rsa_encrypt_decrypt(uint32_t msg, uint32_t key, uint32_t n, uint32_t *result) {
    *result = mod_exp(msg, key, n);
}

