#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE 2
#define MOD 26

void matrixMultiply(int matrix[MATRIX_SIZE][MATRIX_SIZE], int vector[MATRIX_SIZE], int result[MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= MOD;
    }
}

void textToVector(char text[], int vector[], int index) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (text[index + i] == ' ') {
            vector[i] = -1; // Use -1 to denote spaces
        } else {
            vector[i] = toupper(text[index + i]) - 'A';
        }
    }
}

void vectorToText(int vector[], char text[]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        if (vector[i] == -1) {
            text[i] = ' ';
        } else {
            text[i] = vector[i] + 'A';
        }
    }
    text[MATRIX_SIZE] = '\0';
}

int main() {
    int keyMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {9, 4},
        {5, 7}
    };
    
    char plaintext[] = "meet me at the usual place at ten rather than eight oclock";
    char encryptedText[100];
    int len = strlen(plaintext);
    int vector[MATRIX_SIZE];
    int encryptedVector[MATRIX_SIZE];
    int index = 0;

    for (int i = 0; i < len; i += MATRIX_SIZE) {
        textToVector(plaintext, vector, i);
        
        // Encrypt only if the vector does not contain -1 (space)
        if (vector[0] != -1 && vector[1] != -1) {
            matrixMultiply(keyMatrix, vector, encryptedVector);
            vectorToText(encryptedVector, encryptedText + index);
            index += MATRIX_SIZE;
        }
    }
    encryptedText[index] = '\0';

    printf("Encrypted Message: %s\n", encryptedText);

    return 0;
}

