#include <stdio.h>
#include <stdlib.h>

#define SIZE 2
#define MOD 26

// Function to find the modular inverse of a matrix
void matrixInverse(int matrix[SIZE][SIZE], int result[SIZE][SIZE]) {
    int det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    int detInv = 0;
    
    // Calculate determinant and its modular inverse
    for (int i = 1; i < MOD; i++) {
        if ((det * i) % MOD == 1) {
            detInv = i;
            break;
        }
    }
    
    result[0][0] = (matrix[1][1] * detInv) % MOD;
    result[0][1] = (-matrix[0][1] * detInv) % MOD;
    result[1][0] = (-matrix[1][0] * detInv) % MOD;
    result[1][1] = (matrix[0][0] * detInv) % MOD;
    
    // Ensure results are positive
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = (result[i][j] + MOD) % MOD;
        }
    }
}

// Function to multiply two matrices
void matrixMultiply(int A[SIZE][SIZE], int B[SIZE][SIZE], int result[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                result[i][j] = (result[i][j] + A[i][k] * B[k][j]) % MOD;
            }
        }
    }
}

// Function to decrypt using the Hill cipher
void hillDecrypt(int cipherText[SIZE], int keyMatrix[SIZE][SIZE], int plainText[SIZE]) {
    int keyInverse[SIZE][SIZE];
    matrixInverse(keyMatrix, keyInverse);
    matrixMultiply(keyInverse, (int(*)[SIZE])&cipherText, (int(*)[SIZE])&plainText);
}

// Function to solve for the key matrix using known plaintext attack
void solveKeyMatrix(int plaintext[SIZE], int ciphertext[SIZE], int keyMatrix[SIZE][SIZE]) {
    int P[SIZE][SIZE] = { {plaintext[0], plaintext[1]},
                          {plaintext[2], plaintext[3]} };
    int C[SIZE][SIZE] = { {ciphertext[0], ciphertext[1]},
                          {ciphertext[2], ciphertext[3]} };

    // Matrix inverse of P
    int P_inv[SIZE][SIZE];
    matrixInverse(P, P_inv);

    // Compute the key matrix: K = C * P_inv
    matrixMultiply(C, P_inv, keyMatrix);
}

// Function to print a matrix
void printMatrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Known plaintext-ciphertext pairs
    int plaintext[4] = {1, 2, 3, 4}; // Example plaintext blocks
    int ciphertext[4] = {8, 8, 5, 11}; // Example ciphertext blocks

    int keyMatrix[SIZE][SIZE];
    solveKeyMatrix(plaintext, ciphertext, keyMatrix);
    
    printf("Recovered Key Matrix:\n");
    printMatrix(keyMatrix);

    int cipherText[SIZE] = {4, 1}; // Example ciphertext to decrypt
    int decryptedText[SIZE];
    
    hillDecrypt(cipherText, keyMatrix, decryptedText);

    printf("Decrypted Text:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", decryptedText[i]);
    }
    printf("\n");

    return 0;
}

