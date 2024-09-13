#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ALPHABET_SIZE 26
#define TOP_N 10
#define MAX_TEXT_LENGTH 1024

// Expected frequency of letters in English text (as percentages)
const double englishFrequency[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.316, 9.056, 
    2.758, 0.978, 2.560, 0.150, 1.929, 0.095
};

// Function to calculate letter frequencies in a given text
void calculateFrequency(const char *text, double freq[ALPHABET_SIZE]) {
    int length = strlen(text);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = 0;
    }
    for (int i = 0; i < length; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            freq[text[i] - 'a']++;
        }
    }
    // Convert counts to percentage
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (freq[i] / length) * 100.0;
    }
}

// Function to decrypt text with a given key
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

// Function to evaluate a plaintext based on letter frequencies
double evaluateText(const double freq[ALPHABET_SIZE]) {
    double score = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += fabs(freq[i] - englishFrequency[i]);
    }
    return score;
}

// Function to find and print the top N plaintexts based on their scores
void findTopPlaintexts(const char *ciphertext, int topN) {
    double scores[ALPHABET_SIZE];
    char plaintext[MAX_TEXT_LENGTH];
    double freq[ALPHABET_SIZE];
    
    for (int key = 0; key < ALPHABET_SIZE; key++) {
        decryptText(ciphertext, plaintext, key);
        calculateFrequency(plaintext, freq);
        scores[key] = evaluateText(freq);
    }

    // Find the top N scores
    for (int i = 0; i < topN; i++) {
        int minIndex = 0;
        for (int j = 1; j < ALPHABET_SIZE; j++) {
            if (scores[j] < scores[minIndex]) {
                minIndex = j;
            }
        }
        decryptText(ciphertext, plaintext, minIndex);
        printf("Possible Plaintext with Key %d: %s\n", minIndex, plaintext);
        scores[minIndex] = INFINITY;  // Mark as processed
    }
}

int main() {
    char ciphertext[MAX_TEXT_LENGTH] = "gsv jfrxp yildm ulc qfnkh levi gsv ozab wlt"; // Example ciphertext
    printf("Top %d possible plaintexts:\n", TOP_N);
    findTopPlaintexts(ciphertext, TOP_N);
    return 0;
}

