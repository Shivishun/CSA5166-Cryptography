#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

void generatePlayfairMatrix(char matrix[SIZE][SIZE]) {
    char defaultMatrix[SIZE][SIZE] = {
        {'M', 'F', 'H', 'I', 'K'},
        {'U', 'N', 'O', 'P', 'Q'},
        {'Z', 'V', 'W', 'X', 'Y'},
        {'E', 'L', 'A', 'R', 'G'},
        {'D', 'S', 'T', 'B', 'C'}
    };

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix[i][j] = defaultMatrix[i][j];
        }
    }
}

void findPosition(char letter, char matrix[SIZE][SIZE], int* row, int* col) {
    if (letter == 'J') letter = 'I'; // Treat 'I' and 'J' as the same letter
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == letter) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void encryptPlayfair(char text[], char matrix[SIZE][SIZE]) {
    int len = strlen(text);
    for (int i = 0; i < len; i += 2) {
        if (!isalpha(text[i]) || !isalpha(text[i+1])) continue;

        char first = toupper(text[i]);
        char second = toupper(text[i + 1]);

        int row1, col1, row2, col2;
        findPosition(first, matrix, &row1, &col1);
        findPosition(second, matrix, &row2, &col2);

        if (row1 == row2) {
            text[i] = matrix[row1][(col1 + 1) % SIZE];
            text[i + 1] = matrix[row2][(col2 + 1) % SIZE];
        } else if (col1 == col2) {
            text[i] = matrix[(row1 + 1) % SIZE][col1];
            text[i + 1] = matrix[(row2 + 1) % SIZE][col2];
        } else {
            text[i] = matrix[row1][col2];
            text[i + 1] = matrix[row2][col1];
        }
    }
}

void preprocessText(char text[], char processed[]) {
    int len = strlen(text);
    int j = 0;

    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            processed[j++] = toupper(text[i]);
        }
    }

    if (j % 2 != 0) {
        processed[j++] = 'X';  // Add 'X' if odd number of letters
    }

    processed[j] = '\0';
}

int main() {
    char matrix[SIZE][SIZE];
    char plaintext[] = "Must see you over Cadogan West. Coming at once.";
    char processedText[100];

    generatePlayfairMatrix(matrix);

    printf("Playfair Key Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    preprocessText(plaintext, processedText);
    printf("\nProcessed Plaintext: %s\n", processedText);

    encryptPlayfair(processedText, matrix);

    printf("\nEncrypted Message: %s\n", processedText);

    return 0;
}

