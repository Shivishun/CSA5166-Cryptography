#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_CIPHERTEXT_LENGTH 1000
#define MAX_PLAINTEXTS 10

// English letter frequencies (approximate)
const double ENGLISH_FREQ[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.317, 9.056, 
    2.758, 0.978, 2.560, 0.150, 1.929, 0.074
};

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

double calculateScore(double freq[ALPHABET_SIZE]) {
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += (ENGLISH_FREQ[i] - freq[i]) * (ENGLISH_FREQ[i] - freq[i]);
    }
    return score;
}

void decryptWithShift(char *ciphertext, int shift, char *plaintext) {
    int len = strlen(ciphertext);
    
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (ciphertext[i] - base - shift + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}

void findTopPlaintexts(char *ciphertext, int topN) {
    double freq[ALPHABET_SIZE];
    double scores[ALPHABET_SIZE];
    char decryptedText[MAX_CIPHERTEXT_LENGTH];
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        decryptWithShift(ciphertext, i, decryptedText);
        calculateFrequency(decryptedText, freq);
        scores[i] = calculateScore(freq);
    }
    
    int topIndices[MAX_PLAINTEXTS];
    for (int i = 0; i < topN; i++) {
        topIndices[i] = 0;
    }
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
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
        decryptWithShift(ciphertext, topIndices[i], decryptedText);
        printf("Shift %2d: %s\n", topIndices[i], decryptedText);
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

