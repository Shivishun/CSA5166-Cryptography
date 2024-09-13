#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

void generatePlayfairKeyMatrix(char key[], char matrix[SIZE][SIZE]) {
    int used[26] = {0};
    used['J' - 'A'] = 1; // Treat 'I' and 'J' as the same letter
    int keyLen = strlen(key);
    int x = 0, y = 0;

    for (int i = 0; i < keyLen; i++) {
        if (isalpha(key[i])) {
            char letter = toupper(key[i]);
            if (!used[letter - 'A']) {
                matrix[x][y++] = letter;
                used[letter - 'A'] = 1;
                if (y == SIZE) {
                    y = 0;
                    x++;
                }
            }
        }
    }

    for (char letter = 'A'; letter <= 'Z'; letter++) {
        if (!used[letter - 'A']) {
            matrix[x][y++] = letter;
            if (y == SIZE) {
                y = 0;
                x++;
            }
        }
    }
}

void findPosition(char letter, char matrix[SIZE][SIZE], int* row, int* col) {
    if (letter == 'J') letter = 'I'; // Treat 'I' and 'J' the same
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

void decryptPlayfair(char text[], char matrix[SIZE][SIZE]) {
    int len = strlen(text);
    for (int i = 0; i < len; i += 2) {
        if (!isalpha(text[i]) || !isalpha(text[i+1])) continue;

        char first = toupper(text[i]);
        char second = toupper(text[i + 1]);

        int row1, col1, row2, col2;
        findPosition(first, matrix, &row1, &col1);
        findPosition(second, matrix, &row2, &col2);

        if (row1 == row2) {
            text[i] = matrix[row1][(col1 + SIZE - 1) % SIZE];
            text[i + 1] = matrix[row2][(col2 + SIZE - 1) % SIZE];
        } else if (col1 == col2) {
            text[i] = matrix[(row1 + SIZE - 1) % SIZE][col1];
            text[i + 1] = matrix[(row2 + SIZE - 1) % SIZE][col2];
        } else {
            text[i] = matrix[row1][col2];
            text[i + 1] = matrix[row2][col1];
        }
    }
}

int main() {
    char key[] = "PTBOAT";
    char matrix[SIZE][SIZE];
    char ciphertext[] = "KXJEYUREBEZWEHEWRYTUHEYFSKREHEGOYFIWTTTUOLKSYCAJPOBOTEIZONTXBYBNTGONECUZWRGDSONSXBOUYWRHEBAAHYUSEDQ";

    generatePlayfairKeyMatrix(key, matrix);

    printf("Playfair Key Matrix:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nOriginal Ciphertext: %s\n", ciphertext);

    decryptPlayfair(ciphertext, matrix);

    printf("\nDecrypted Message: %s\n", ciphertext);

    return 0;
}

