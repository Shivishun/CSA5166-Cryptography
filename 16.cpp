#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define MAX_CIPHERTEXT_LENGTH 1000
#define MAX_PLAINTEXTS 10

// English letter frequencies (approximate)
const double ENGLISH_FREQ[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.317, 9.056, 
    2.758, 0.978, 2.560, 0.150, 1.929, 0.074
};

// Frequency analysis function
void calculateFrequency(char *text, double freq[ALPHABET_SIZE]) {
    int count[ALPHABET_SIZE] = {0};
    int len = strlen(text);

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            int index = toupper(text[i]) - 'A';
            count[index]++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (double)count[i] / len * 100;
    }
}

// Calculate the score of a substitution based on letter frequencies
double calculateScore(double freq[ALPHABET_SIZE]) {
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += (ENGLISH_FREQ[i] - freq[i]) * (ENGLISH_FREQ[i] - freq[i]);
    }
    return score;
}

// Decrypt the ciphertext using a specific substitution key
void decryptWithSubstitution(char *ciphertext, char *key, char *plaintext) {
    int len = strlen(ciphertext);

    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = isupper(ciphertext[i]) ? key[ciphertext[i] - 'A'] : tolower(key[ciphertext[i] - 'A']);
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

// Generate permutations of a given string
void permute(char *key, int left, int right, char keys[][ALPHABET_SIZE + 1], int *count) {
    if (left == right) {
        strcpy(keys[*count], key);
        (*count)++;
    } else {
        for (int i = left; i <= right; i++) {
            // Swap characters
            char temp = key[left];
            key[left] = key[i];
            key[i] = temp;

            // Recurse
            permute(key, left + 1, right, keys, count);

            // Swap back
            temp = key[left];
            key[left] = key[i];
            key[i] = temp;
        }
    }
}

// Perform frequency attack on a monoalphabetic substitution cipher
void findTopPlaintexts(char *ciphertext, int topN) {
    double freq[ALPHABET_SIZE];
    double scores[MAX_PLAINTEXTS];
    char decryptedText[MAX_CIPHERTEXT_LENGTH];
    char key[ALPHABET_SIZE + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char keys[100000][ALPHABET_SIZE + 1]; // Adjust size as necessary
    int count = 0;

    permute(key, 0, ALPHABET_SIZE - 1, keys, &count);

    for (int i = 0; i < count; i++) {
        decryptWithSubstitution(ciphertext, keys[i], decryptedText);
        calculateFrequency(decryptedText, freq);
        scores[i] = calculateScore(freq);
    }

    int topIndices[MAX_PLAINTEXTS];
    for (int i = 0; i < topN; i++) {
        topIndices[i] = 0;
    }

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < topN; j++) {
            if (scores[i] < scores[topIndices[j]]) {
                for (int k = topN - 1; k > j; k--) {
                    topIndices[k] = topIndices[k - 1];
                }
                topIndices[j] = i;
                break;
            }
        }
    }

    printf("Top %d Possible Plaintexts:\n", topN);
    for (int i = 0; i < topN; i++) {
        decryptWithSubstitution(ciphertext, keys[topIndices[i]], decryptedText);
        printf("Key %s: %s\n", keys[topIndices[i]], decryptedText);
    }
}

int main() {
    char ciphertext[MAX_CIPHERTEXT_LENGTH];
    int topN;

    printf("Enter ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove newline character

    printf("Enter number of top plaintexts to display: ");
    scanf("%d", &topN);

    if (topN > MAX_PLAINTEXTS) {
        topN = MAX_PLAINTEXTS;
    }

    findTopPlaintexts(ciphertext, topN);

    return 0;
}

