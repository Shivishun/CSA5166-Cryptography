#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 8 // 64 bits / 8 bytes

void pad_block(unsigned char *block, size_t *length) {
    size_t padding_needed = BLOCK_SIZE - (*length % BLOCK_SIZE);
    if (padding_needed == BLOCK_SIZE) {
        padding_needed = 0; // No padding needed if block is already a multiple of BLOCK_SIZE
    }

    block[*length] = 0x80; // Padding bit (0x80 is 10000000 in binary)
    memset(block + *length + 1, 0, padding_needed - 1);
    *length += padding_needed;
}

void encrypt_ecb(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key) {
    // ECB mode encryption (simplified example)
    // Here we just XOR the plaintext with the key for demonstration
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        ciphertext[i] = plaintext[i] ^ key[i % BLOCK_SIZE];
    }
}

void decrypt_ecb(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key) {
    // ECB mode decryption (simplified example)
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        plaintext[i] = ciphertext[i] ^ key[i % BLOCK_SIZE];
    }
}

void encrypt_cbc(const unsigned char *plaintext, unsigned char *ciphertext, size_t length, const unsigned char *key, const unsigned char *iv) {
    unsigned char previous_block[BLOCK_SIZE];
    memcpy(previous_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        unsigned char block[BLOCK_SIZE];
        memcpy(block, plaintext + i, BLOCK_SIZE);
        
        // XOR with previous ciphertext block or IV for the first block
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= previous_block[j];
        }
        
        encrypt_ecb(block, ciphertext + i, key);
        memcpy(previous_block, ciphertext + i, BLOCK_SIZE);
    }
}

void decrypt_cbc(const unsigned char *ciphertext, unsigned char *plaintext, size_t length, const unsigned char *key, const unsigned char *iv) {
    unsigned char previous_block[BLOCK_SIZE];
    memcpy(previous_block, iv, BLOCK_SIZE);

    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        unsigned char block[BLOCK_SIZE];
        memcpy(block, ciphertext + i, BLOCK_SIZE);
        
        decrypt_ecb(block, block, key);
        
        // XOR with previous ciphertext block or IV for the first block
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= previous_block[j];
        }
        
        memcpy(plaintext + i, block, BLOCK_SIZE);
        memcpy(previous_block, ciphertext + i, BLOCK_SIZE);
    }
}

void encrypt_cfb(const unsigned char *plaintext, unsigned char *ciphertext, size_t length, const unsigned char *key, const unsigned char *iv) {
    unsigned char feedback[BLOCK_SIZE];
    memcpy(feedback, iv, BLOCK_SIZE);

    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        unsigned char block[BLOCK_SIZE];
        size_t bytes_to_process = (length - i < BLOCK_SIZE) ? length - i : BLOCK_SIZE;
        memcpy(block, plaintext + i, bytes_to_process);
        
        // Encrypt the feedback for CFB mode
        unsigned char encrypted_feedback[BLOCK_SIZE];
        encrypt_ecb(feedback, encrypted_feedback, key);
        
        // XOR the plaintext with the encrypted feedback
        for (size_t j = 0; j < bytes_to_process; j++) {
            ciphertext[i + j] = block[j] ^ encrypted_feedback[j];
        }
        
        // Update the feedback
        memcpy(feedback, ciphertext + i, BLOCK_SIZE);
    }
}

void decrypt_cfb(const unsigned char *ciphertext, unsigned char *plaintext, size_t length, const unsigned char *key, const unsigned char *iv) {
    unsigned char feedback[BLOCK_SIZE];
    memcpy(feedback, iv, BLOCK_SIZE);

    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        unsigned char block[BLOCK_SIZE];
        size_t bytes_to_process = (length - i < BLOCK_SIZE) ? length - i : BLOCK_SIZE;
        
        // Encrypt the feedback for CFB mode
        unsigned char encrypted_feedback[BLOCK_SIZE];
        encrypt_ecb(feedback, encrypted_feedback, key);
        
        // XOR the ciphertext with the encrypted feedback
        for (size_t j = 0; j < bytes_to_process; j++) {
            block[j] = ciphertext[i + j] ^ encrypted_feedback[j];
        }
        
        // Write decrypted block to plaintext
        memcpy(plaintext + i, block, bytes_to_process);
        
        // Update the feedback
        memcpy(feedback, ciphertext + i, BLOCK_SIZE);
    }
}

int main() {
    unsigned char key[BLOCK_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    unsigned char iv[BLOCK_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char plaintext[] = "This is an example text!";
    size_t plaintext_length = strlen((char*)plaintext);

    // Padding
    pad_block(plaintext, &plaintext_length);

    unsigned char ciphertext[1024] = {0};
    unsigned char decryptedtext[1024] = {0};

    // ECB Encryption and Decryption
    printf("ECB Mode:\n");
    encrypt_ecb(plaintext, ciphertext, key);
    decrypt_ecb(ciphertext, decryptedtext, key);
    printf("Plaintext: %s\n", plaintext);
    printf("Decrypted: %s\n", decryptedtext);

    // CBC Encryption and Decryption
    printf("\nCBC Mode:\n");
    encrypt_cbc(plaintext, ciphertext, plaintext_length, key, iv);
    decrypt_cbc(ciphertext, decryptedtext, plaintext_length, key, iv);
    printf("Plaintext: %s\n", plaintext);
    printf("Decrypted: %s\n", decryptedtext);

    // CFB Encryption and Decryption
    printf("\nCFB Mode:\n");
    encrypt_cfb(plaintext, ciphertext, plaintext_length, key, iv);
    decrypt_cfb(ciphertext, decryptedtext, plaintext_length, key, iv);
    printf("Plaintext: %s\n", plaintext);
    printf("Decrypted: %s\n", decryptedtext);

    return 0;
}

