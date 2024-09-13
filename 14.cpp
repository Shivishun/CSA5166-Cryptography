#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 100
#define KEY_LENGTH 14 // Length of the key stream

void encryptVigenereOneTimePad(char *plaintext, int *key, int keyLength, char *ciphertext) {
    int ptLength = strlen(plaintext);

    for (int i = 0; i < ptLength; i++) {
        if (isalpha(plaintext[i])) {
            int shift = key[i % keyLength];
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = (plaintext[i] - base + shift) % 26 + base;
        } else {
            // Preserve non-alphabetic characters
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[ptLength] = '\0'; // Null-terminate the ciphertext
}

int main() {
    char plaintext[] = "send more money";
    int key[KEY_LENGTH] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    char ciphertext[MAX_LENGTH];

    encryptVigenereOneTimePad(plaintext, key, KEY_LENGTH, ciphertext);

    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}

