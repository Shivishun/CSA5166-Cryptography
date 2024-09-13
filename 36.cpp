#include <stdio.h>
#include <ctype.h>

// Function to compute the greatest common divisor
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to compute the modular inverse of 'a' under modulo 'm'
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;  // Modular inverse does not exist
}

// Function to encrypt plaintext using the affine cipher
void encrypt_affine(const char *plaintext, int a, int b, char *ciphertext) {
    int mod = 26; // Number of letters in the alphabet
    for (int i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = (a * (plaintext[i] - base) + b) % mod + base;
        } else {
            ciphertext[i] = plaintext[i];  // Copy non-alphabetic characters unchanged
        }
    }
    ciphertext[strlen(plaintext)] = '\0';  // Null-terminate the ciphertext
}

// Function to decrypt ciphertext using the affine cipher
void decrypt_affine(const char *ciphertext, int a, int b, char *plaintext) {
    int mod = 26; // Number of letters in the alphabet
    int a_inv = mod_inverse(a, mod); // Modular inverse of 'a'
    if (a_inv == -1) {
        printf("Error: Modular inverse does not exist. Choose a different 'a' value.\n");
        return;
    }
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (a_inv * ((ciphertext[i] - base) - b + mod)) % mod + base;
        } else {
            plaintext[i] = ciphertext[i];  // Copy non-alphabetic characters unchanged
        }
    }
    plaintext[strlen(ciphertext)] = '\0';  // Null-terminate the plaintext
}

int main() {
    int a = 5; // Multiplicative key
    int b = 8; // Additive key
    
    if (gcd(a, 26) != 1) {
        printf("Error: 'a' must be coprime with 26 for the cipher to be bijective.\n");
        return 1;
    }

    char plaintext[] = "Hello, World!";
    char ciphertext[256];
    char decryptedtext[256];
    
    printf("Plaintext: %s\n", plaintext);

    // Encrypt the plaintext
    encrypt_affine(plaintext, a, b, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);
    
    // Decrypt the ciphertext
    decrypt_affine(ciphertext, a, b, decryptedtext);
    printf("Decrypted Text: %s\n", decryptedtext);

    return 0;
}

