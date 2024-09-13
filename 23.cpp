#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 8
#define COUNTER_SIZE 8

// Define the S-DES key and permutation tables
const uint8_t P10[10] = {2, 4, 1, 6, 3, 9, 5, 10, 7, 8};
const uint8_t P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
const uint8_t IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
const uint8_t IP_INV[8] = {4, 1, 3, 5, 7, 2, 8, 6};
const uint8_t EP[8] = {4, 1, 2, 3, 2, 3, 4, 1};
const uint8_t P4[4] = {2, 4, 3, 1};
const uint8_t P4_INV[4] = {4, 1, 3, 2};

// Define S-Boxes
const uint8_t S0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
const uint8_t S1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

// Function prototypes
void permute(uint8_t *input, const uint8_t *permutation, uint8_t *output, int size);
void key_schedule(uint8_t key[10], uint8_t k1[8], uint8_t k2[8]);
void f(uint8_t r, uint8_t k, uint8_t *output);
void sdes_encrypt(uint8_t plaintext, uint8_t key[10], uint8_t *ciphertext);
void sdes_decrypt(uint8_t ciphertext, uint8_t key[10], uint8_t *plaintext);
void xor_blocks(uint8_t *block1, uint8_t *block2, uint8_t *result);
void sdes_ctr_encrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t *counter, size_t length);
void sdes_ctr_decrypt(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *key, uint8_t *counter, size_t length);

// Main function
int main() {
    // Key and Counter
    uint8_t key[10] = {0, 1, 1, 1, 1, 1, 1, 0, 1, 1}; // Key: 01111 11101
    uint8_t counter[COUNTER_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0}; // Counter: 0000 0000

    // Test plaintext
    uint8_t plaintext[BLOCK_SIZE] = {0, 0, 0, 1, 0, 0, 1, 0}; // 0000 0001 0000 0010 0000 0100

    uint8_t ciphertext[BLOCK_SIZE];
    uint8_t decrypted[BLOCK_SIZE];

    // Encryption
    sdes_ctr_encrypt(plaintext, ciphertext, key, counter, BLOCK_SIZE);
    printf("Encrypted ciphertext: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decryption
    sdes_ctr_decrypt(ciphertext, decrypted, key, counter, BLOCK_SIZE);
    printf("Decrypted plaintext: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    return 0;
}

// Permute input using a given permutation table
void permute(uint8_t *input, const uint8_t *permutation, uint8_t *output, int size) {
    for (int i = 0; i < size; i++) {
        output[i] = input[permutation[i] - 1];
    }
}

// Generate K1 and K2 keys from the original key
void key_schedule(uint8_t key[10], uint8_t k1[8], uint8_t k2[8]) {
    uint8_t permuted_key[10];
    permute(key, P10, permuted_key, 10);

    // Split into two 5-bit halves
    uint8_t left[5] = {0};
    uint8_t right[5] = {0};
    memcpy(left, permuted_key, 5);
    memcpy(right, permuted_key + 5, 5);

    // Left shift
    for (int i = 0; i < 1; i++) {
        uint8_t temp = left[0];
        for (int j = 0; j < 4; j++) {
            left[j] = left[j + 1];
        }
        left[4] = temp;
        temp = right[0];
        for (int j = 0; j < 4; j++) {
            right[j] = right[j + 1];
        }
        right[4] = temp;
    }

    // Generate K1
    uint8_t combined[10] = {0};
    memcpy(combined, left, 5);
    memcpy(combined + 5, right, 5);
    permute(combined, P8, k1, 8);

    // Left shift
    for (int i = 0; i < 2; i++) {
        uint8_t temp = left[0];
        for (int j = 0; j < 4; j++) {
            left[j] = left[j + 1];
        }
        left[4] = temp;
        temp = right[0];
        for (int j = 0; j < 4; j++) {
            right[j] = right[j + 1];
        }
        right[4] = temp;
    }

    // Generate K2
    memcpy(combined, left, 5);
    memcpy(combined + 5, right, 5);
    permute(combined, P8, k2, 8);
}

// Function F used in encryption and decryption
void f(uint8_t r, uint8_t k, uint8_t *output) {
    // Expansion permutation
    uint8_t expanded[8];
    permute(&r, EP, expanded, 8);

    // XOR with key
    for (int i = 0; i < 8; i++) {
        expanded[i] ^= k;
    }

    // S-boxes
    uint8_t left = (expanded[0] << 2) | (expanded[1] << 1) | expanded[2];
    uint8_t right = (expanded[3] << 2) | (expanded[4] << 1) | expanded[5];
    uint8_t s0 = S0[left >> 2][left & 3];
    uint8_t s1 = S1[right >> 2][right & 3];
    uint8_t s_output[4] = {s0, s1};

    // P4 permutation
    permute(s_output, P4, output, 4);
}

// Encryption function
void sdes_encrypt(uint8_t plaintext, uint8_t key[10], uint8_t *ciphertext) {
    uint8_t k1[8], k2[8];
    key_schedule(key, k1, k2);

    // Initial Permutation
    uint8_t permuted[8];
    permute(&plaintext, IP, permuted, 8);

    // Split into two halves
    uint8_t left = permuted[0];
    uint8_t right = permuted[1];

    // Round 1
    uint8_t f_output[4];
    f(right, k1[0], f_output);
    left ^= f_output[0];
    right ^= f_output[1];

    // Round 2
    f(left, k2[0], f_output);
    left ^= f_output[0];
    right ^= f_output[1];

    // Inverse Permutation
    uint8_t final[8] = {right, left};
    permute(final, IP_INV, ciphertext, 8);
}

// Decryption function
void sdes_decrypt(uint8_t ciphertext, uint8_t key[10], uint8_t *plaintext) {
    uint8_t k1[8], k2[8];
    key_schedule(key, k1, k2);

    // Initial Permutation
    uint8_t permuted[8];
    permute(&ciphertext, IP, permuted, 8);

    // Split into two halves
    uint8_t left = permuted[0];
    uint8_t right = permuted[1];

    // Round 1
    uint8_t f_output[4];
    f(right, k2[0], f_output);
    left ^= f_output[0];
    right ^= f_output[1];

    // Round 2
    f(left, k1[0], f_output);
    left ^= f_output[0];
    right ^= f_output[1];

    // Inverse Permutation
    uint8_t final[8] = {right, left};
    permute(final, IP_INV, plaintext, 8);
}

// XOR function
void xor_blocks(uint8_t *block1, uint8_t *block2, uint8_t *result) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        result[i] = block1[i] ^ block2[i];
    }
}

// Encryption in CTR mode
void sdes_ctr_encrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t *counter, size_t length) {
    uint8_t counter_block[BLOCK_SIZE];
    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        // Encrypt the counter block
        sdes_encrypt(*counter, key, counter_block);

        // XOR with plaintext block
        xor_blocks(plaintext + i, counter_block, ciphertext + i);

        // Increment counter
        for (int j = BLOCK_SIZE - 1; j >= 0; j--) {
            if (++counter[j] != 0) break;
        }
    }
}

// Decryption in CTR mode (same as encryption)
void sdes_ctr_decrypt(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *key, uint8_t *counter, size_t length) {
    sdes_ctr_encrypt(ciphertext, plaintext, key, counter, length);
}

