#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define TOP_N 10

// Frequency of letters in English text (approximate percentages)
const float english_freq[ALPHABET_SIZE] = {
    0.082, 0.015, 0.028, 0.043, 0.127, 0.022, 0.020, 0.061, 0.070, 0.002,
    0.005, 0.040, 0.024, 0.067, 0.075, 0.019, 0.001, 0.060, 0.063, 0.091,
    0.028, 0.010, 0.023, 0.001, 0.020, 0.002, 0.020, 0.060, 0.000, 0.000
};

// Function to calculate the frequency of letters in the ciphertext
void calculate_freq(const char *ciphertext, float freq[ALPHABET_SIZE]) {
    int count[ALPHABET_SIZE] = {0};
    int total_chars = 0;

    for (int i = 0; ciphertext[i]; i++) {
        if (isalpha(ciphertext[i])) {
            char c = toupper(ciphertext[i]) - 'A';
            count[c]++;
            total_chars++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (float)count[i] / total_chars;
    }
}

// Function to calculate the score based on frequency similarity to English
float calculate_score(const float freq[ALPHABET_SIZE]) {
    float score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += fabs(freq[i] - english_freq[i]);
    }
    return score;
}

// Function to decrypt ciphertext using a substitution cipher
void decrypt(const char *ciphertext, const char *key, char *plaintext) {
    for (int i = 0; ciphertext[i]; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = key[toupper(ciphertext[i]) - 'A'];
            if (islower(ciphertext[i])) plaintext[i] = tolower(plaintext[i]);
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

// Function to generate all permutations of the alphabet (for brute-force attack)
void permute(char *alphabet, int l, int r, const char *ciphertext) {
    if (l == r) {
        char key[ALPHABET_SIZE + 1];
        strncpy(key, alphabet, ALPHABET_SIZE);
        key[ALPHABET_SIZE] = '\0';

        char decrypted_text[256];
        decrypt(ciphertext, key, decrypted_text);

        float freq[ALPHABET_SIZE];
        calculate_freq(decrypted_text, freq);

        float score = calculate_score(freq);
        printf("Key: %s\nScore: %f\nDecrypted Text: %s\n\n", key, score, decrypted_text);
    } else {
        for (int i = l; i <= r; i++) {
            // Swap
            char temp = alphabet[l];
            alphabet[l] = alphabet[i];
            alphabet[i] = temp;

            permute(alphabet, l + 1, r, ciphertext);

            // Swap back
            temp = alphabet[l];
            alphabet[l] = alphabet[i];
            alphabet[i] = temp;
        }
    }
}

int main() {
    char ciphertext[] = "GPIYF QKFP OMBR";  // Replace with your ciphertext
    char alphabet[ALPHABET_SIZE + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    // Brute-force approach: Generate all permutations of the alphabet
    printf("Performing frequency analysis...\n");
    permute(alphabet, 0, ALPHABET_SIZE - 1, ciphertext);
    
    return 0;
}

