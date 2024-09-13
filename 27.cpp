#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototypes
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod);
unsigned long long gcd(unsigned long long a, unsigned long long b);
unsigned long long mod_inverse(unsigned long long a, unsigned long long m);
void generate_keys(unsigned long long *e, unsigned long long *d, unsigned long long *n);

// Function to encrypt a message
void encrypt_message(const char *message, unsigned long long e, unsigned long long n, unsigned long long *ciphertext) {
    for (int i = 0; message[i] != '\0'; i++) {
        unsigned long long m = message[i] - 'A'; // Convert char to integer (0-25)
        ciphertext[i] = mod_exp(m, e, n);
    }
}

// Function to decrypt a message
void decrypt_message(const unsigned long long *ciphertext, char *message, unsigned long long d, unsigned long long n) {
    for (int i = 0; ciphertext[i] != 0; i++) {
        unsigned long long m = mod_exp(ciphertext[i], d, n);
        message[i] = m + 'A'; // Convert integer (0-25) to char
    }
    message[strlen(message)] = '\0'; // Null-terminate the string
}

// Main function
int main() {
    unsigned long long e, d, n;
    char plaintext[] = "HELLO";
    unsigned long long ciphertext[100];

    // Generate RSA keys
    generate_keys(&e, &d, &n);
    printf("Public Key: (e = %llu, n = %llu)\n", e, n);
    printf("Private Key: (d = %llu)\n", d);

    // Encrypt the message
    encrypt_message(plaintext, e, n, ciphertext);

    printf("Encrypted message:\n");
    for (int i = 0; plaintext[i] != '\0'; i++) {
        printf("%llu ", ciphertext[i]);
    }
    printf("\n");

    // Decrypt the message
    char decrypted[100];
    decrypt_message(ciphertext, decrypted, d, n);
    printf("Decrypted message: %s\n", decrypted);

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

