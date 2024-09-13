#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE 2
#define MOD 26

void matrixPrint(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void textToVector(char text[], int vector[]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        vector[i] = toupper(text[i]) - 'A';
    }
}

void vectorToText(int vector[], char text[]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        text[i] = vector[i] + 'A';
    }
    text[MATRIX_SIZE] = '\0';
}

void matrixMultiply(int matrix[MATRIX_SIZE][MATRIX_SIZE], int vector[MATRIX_SIZE], int result[MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= MOD;
    }
}

void matrixInverse(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inverse[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % MOD;
    if (det < 0) det += MOD;
    
    int detInv = 0;
    for (int i = 0; i < MOD; i++) {
        if ((det * i) % MOD == 1) {
            detInv = i;
            break;
        }
    }

    inverse[0][0] = matrix[1][1] * detInv % MOD;
    inverse[0][1] = -matrix[0][1] * detInv % MOD;
    inverse[1][0] = -matrix[1][0] * detInv % MOD;
    inverse[1][1] = matrix[0][0] * detInv % MOD;

    if (inverse[0][0] < 0) inverse[0][0] += MOD;
    if (inverse[0][1] < 0) inverse[0][1] += MOD;
    if (inverse[1][0] < 0) inverse[1][0] += MOD;
    if (inverse[1][1] < 0) inverse[1][1] += MOD;
}

void solveKeyMatrix(int pt1[], int ct1[], int pt2[], int ct2[], int keyMatrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int ptMatrix[MATRIX_SIZE][MATRIX_SIZE] = { {pt1[0], pt1[1]}, {pt2[0], pt2[1]} };
    int ctMatrix[MATRIX_SIZE][MATRIX_SIZE] = { {ct1[0], ct1[1]}, {ct2[0], ct2[1]} };
    int ptMatrixInv[MATRIX_SIZE][MATRIX_SIZE];

    matrixInverse(ptMatrix, ptMatrixInv);

    int keyMatrixTemp[MATRIX_SIZE][MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            keyMatrixTemp[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                keyMatrixTemp[i][j] += ctMatrix[i][k] * ptMatrixInv[k][j];
            }
            keyMatrixTemp[i][j] %= MOD;
        }
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            keyMatrix[i][j] = keyMatrixTemp[i][j];
        }
    }
}

int main() {
    char plaintext1[] = "ME";
    char ciphertext1[] = "PA";
    char plaintext2[] = "AT";
    char ciphertext2[] = "EX";

    int pt1[MATRIX_SIZE], ct1[MATRIX_SIZE];
    int pt2[MATRIX_SIZE], ct2[MATRIX_SIZE];
    int keyMatrix[MATRIX_SIZE][MATRIX_SIZE];

    textToVector(plaintext1, pt1);
    textToVector(ciphertext1, ct1);
    textToVector(plaintext2, pt2);
    textToVector(ciphertext2, ct2);

    solveKeyMatrix(pt1, ct1, pt2, ct2, keyMatrix);

    printf("Recovered Key Matrix:\n");
    matrixPrint(keyMatrix);

    return 0;
}

