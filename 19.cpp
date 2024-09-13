#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>  // Requires OpenSSL

#define BLOCK_SIZE 8
#define KEY_SIZE 24

void xor_block(unsigned char *block1, unsigned char *block2, unsigned char *result) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        result[i] = block1[i] ^ block2[i];
    }
}

void des3_encrypt_cbc(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key, const unsigned char *iv, size_t length) {
    DES_key_schedule ks1, ks2, ks3;
    DES_cblock des_key1, des_key2, des_key3;

    // Set up the keys
    memcpy(des_key1, key, DES_KEY_SZ);
    memcpy(des_key2, key + DES_KEY_SZ, DES_KEY_SZ);
    memcpy(des_key3, key + 2 * DES_KEY_SZ, DES_KEY_SZ);

    DES_set_key_unchecked(&des_key1, &ks1);
    DES_set_key_unchecked(&des_key2, &ks2);
    DES_set_key_unchecked(&des_key3, &ks3);

    unsigned char previous_block[BLOCK_SIZE];
    unsigned char xor_block_result[BLOCK_SIZE];
    size_t num_blocks = length / BLOCK_SIZE;

    // Initialize the first block of ciphertext with IV
    memcpy(previous_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < num_blocks; i++) {
        // XOR plaintext with previous ciphertext block (or IV for the first block)
        xor_block(plaintext + i * BLOCK_SIZE, previous_block, xor_block_result);

        // Encrypt using 3DES
        DES_ecb3_encrypt((DES_cblock *)xor_block_result, (DES_cblock *)(ciphertext + i * BLOCK_SIZE), &ks1, &ks2, &ks3, DES_ENCRYPT);

        // Copy the current ciphertext block to previous_block for the next iteration
        memcpy(previous_block, ciphertext + i * BLOCK_SIZE, BLOCK_SIZE);
    }
}

int main() {
    const unsigned char key[KEY_SIZE] = "123456789012345678901234";  // 24-byte key for 3DES
    const unsigned char iv[BLOCK_SIZE] = "abcdefgh";  // Initialization Vector
    const unsigned char plaintext[] = "This is a test message for CBC mode encryption with 3DES.";
    unsigned char ciphertext[sizeof(plaintext)];

    size_t length = sizeof(plaintext) - 1;  // Exclude the null terminator

    des3_encrypt_cbc(plaintext, ciphertext, key, iv, length);

    printf("Ciphertext: ");
    for (size_t i = 0; i < length; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    return 0;
}

