#include <stdio.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define TOP_N 10

// Function to calculate frequency of letters in the ciphertext
void calculateFrequency(const char *text, int freq[ALPHABET_SIZE]) {
    int length = strlen(text);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = 0;
    }
    for (int i = 0; i < length; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            freq[text[i] - 'a']++;
        }
    }
}

// Function to decrypt text using an additive cipher with a given key
void decryptText(const char *ciphertext, char *plaintext, int key) {
    int length = strlen(ciphertext);
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = ((ciphertext[i] - 'a' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'a';
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[length] = '\0';
}

// Function to evaluate plaintexts based on letter frequency
double evaluateText(const char *text) {
    int freq[ALPHABET_SIZE] = {0};
    calculateFrequency(text, freq);

    // Assume these are the frequencies for 'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd'
    // In practice, you may want to adjust these or use actual English frequencies
    double expected[ALPHABET_SIZE] = {12.02, 9.10, 8.12, 7.31, 6.28, 5.00, 4.25, 3.99, 3.88, 3.56,
                                       3.50, 3.05, 2.73, 2.51, 2.33, 2.21, 2.02, 1.75, 1.66, 1.62,
                                       1.60, 1.45, 1.28, 1.02, 0.83, 0.80};

    double score = 0;
    int length = strlen(text);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += (freq[i] / (double)length) * expected[i];
    }

    return score;
}

// Function to find the top N plaintexts based on their scores
void findTopPlaintexts(const char *ciphertext, int topN) {
    double scores[ALPHABET_SIZE];
    char plaintext[256];

    for (int key = 0; key < ALPHABET_SIZE; key++) {
        decryptText(ciphertext, plaintext, key);
        scores[key] = evaluateText(plaintext);
    }

    // Simple sorting of top N scores
    for (int i = 0; i < topN; i++) {
        int maxIndex = 0;
        for (int j = 1; j < ALPHABET_SIZE; j++) {
            if (scores[j] > scores[maxIndex]) {
                maxIndex = j;
            }
        }
        decryptText(ciphertext, plaintext, maxIndex);
        printf("Possible Plaintext with Key %d: %s\n", maxIndex, plaintext);
        scores[maxIndex] = -1;  // Mark as processed
    }
}

int main() {
    char ciphertext[] = "wkh txlfn eurzq ira mpsv";  // Example ciphertext
    printf("Top %d possible plaintexts:\n", TOP_N);
    findTopPlaintexts(ciphertext, TOP_N);
    return 0;
}

