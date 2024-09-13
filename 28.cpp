#include <stdio.h>
#include <math.h>

// Function prototypes
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod);
unsigned long long compute_shared_key(unsigned long long base, unsigned long long exponent, unsigned long long mod);

int main() {
    unsigned long long p = 23; // A prime number (public)
    unsigned long long g = 5;  // A primitive root modulo p (public)
    unsigned long long a_secret = 6; // Alice's private key
    unsigned long long b_secret = 15; // Bob's private key

    // Alice computes A = g^a_secret mod p and sends A to Bob
    unsigned long long A = mod_exp(g, a_secret, p);
    printf("Alice sends A: %llu\n", A);

    // Bob computes B = g^b_secret mod p and sends B to Alice
    unsigned long long B = mod_exp(g, b_secret, p);
    printf("Bob sends B: %llu\n", B);

    // Alice computes the shared key as B^a_secret mod p
    unsigned long long alice_shared_key = mod_exp(B, a_secret, p);
    printf("Alice computes shared key: %llu\n", alice_shared_key);

    // Bob computes the shared key as A^b_secret mod p
    unsigned long long bob_shared_key = mod_exp(A, b_secret, p);
    printf("Bob computes shared key: %llu\n", bob_shared_key);

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

