#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototypes
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod);
unsigned long long gcd(unsigned long long a, unsigned long long b);
unsigned long long mod_inverse(unsigned long long a, unsigned long long m);
void generate_keys(unsigned long long *e, unsigned long long *d, unsigned long long *n);

// Main function
int main() {
    unsigned long long e, d, n;
    unsigned long long plaintext, ciphertext, decrypted;

    // Generate RSA keys
    generate_keys(&e, &d, &n);
    printf("Public Key: (e = %llu, n = %llu)\n", e, n);
    printf("Private Key: (d = %llu)\n", d);

    // Example plaintext
    plaintext = 1234;

    // Encrypt
    ciphertext = mod_exp(plaintext, e, n);
    printf("Encrypted ciphertext: %llu\n", ciphertext);

    // Decrypt
    decrypted = mod_exp(ciphertext, d, n);
    printf("Decrypted plaintext: %llu\n", decrypted);

    return 0;
}

// Function to compute modular exponentiation
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to compute gcd using Euclidean algorithm
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to compute modular multiplicative inverse
unsigned long long mod_inverse(unsigned long long a, unsigned long long m) {
    unsigned long long m0 = m, t, q;
    unsigned long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;

        m = a % m;
        a = t;
        t = x0;

        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

// Function to generate RSA keys
void generate_keys(unsigned long long *e, unsigned long long *d, unsigned long long *n) {
    unsigned long long p = 61; // Prime 1
    unsigned long long q = 53; // Prime 2
    unsigned long long phi, k = 0;

    *n = p * q;
    phi = (p - 1) * (q - 1);

    *e = 17; // Chosen public exponent

    while (gcd(*e, phi) != 1) {
        (*e)++;
    }

    *d = mod_inverse(*e, phi);
}

