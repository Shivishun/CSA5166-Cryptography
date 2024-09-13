#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to encrypt plaintext using one-time pad Vigenère cipher
void encrypt_vigenere(const char *plaintext, const int *key, size_t key_length, char *ciphertext) {
    size_t i, j = 0;
    for (i = 0; i < strlen(plaintext); i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = (plaintext[i] - base + key[j]) % 26 + base;
            j = (j + 1) % key_length;
        } else {
            ciphertext[i] = plaintext[i];  // Copy non-alphabetic characters unchanged
        }
    }
    ciphertext[i] = '\0';  // Null-terminate the ciphertext
}

// Function to decrypt ciphertext using one-time pad Vigenère cipher
void decrypt_vigenere(const char *ciphertext, const int *key, size_t key_length, char *plaintext) {
    size_t i, j = 0;
    for (i = 0; i < strlen(ciphertext); i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (ciphertext[i] - base - key[j] + 26) % 26 + base;
            j = (j + 1) % key_length;
        } else {
            plaintext[i] = ciphertext[i];  // Copy non-alphabetic characters unchanged
        }
    }
    plaintext[i] = '\0';  // Null-terminate the plaintext
}

int main() {
    // Example key and plaintext
    const int key[] = {3, 19, 5, 7, 13};  // Example key stream
    size_t key_length = sizeof(key) / sizeof(key[0]);
    
    char plaintext[] = "Encrypt this message!";
    char ciphertext[256];
    char decryptedtext[256];
    
    printf("Plaintext: %s\n", plaintext);

    // Encrypt the plaintext
    encrypt_vigenere(plaintext, key, key_length, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);
    
    // Decrypt the ciphertext
    decrypt_vigenere(ciphertext, key, key_length, decryptedtext);
    printf("Decrypted Text: %s\n", decryptedtext);

    return 0;
}

