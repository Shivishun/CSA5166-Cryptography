#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Constants
#define BLOCK_SIZE 16 // 128 bits (16 bytes)

// Simple block cipher function (for demonstration purposes)
void simple_block_cipher(uint8_t *block, uint8_t *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key[i];
    }
}

// CBC-MAC function
void cbc_mac(uint8_t *message, size_t length, uint8_t *key, uint8_t *mac) {
    uint8_t block[BLOCK_SIZE];
    uint8_t prev_block[BLOCK_SIZE] = {0}; // Initialization vector (IV) is all zeros

    for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(block, message + i, BLOCK_SIZE);
        
        // XOR with previous block
        for (int j = 0; j < BLOCK_SIZE; j++) {
            block[j] ^= prev_block[j];
        }
        
        // Encrypt the block
        simple_block_cipher(block, key);
        
        // Copy the encrypted block to the previous block for the next iteration
        memcpy(prev_block, block, BLOCK_SIZE);
    }

    // The final block is the MAC
    memcpy(mac, prev_block, BLOCK_SIZE);
}

// XOR function
void xor_blocks(uint8_t *block1, uint8_t *block2, uint8_t *result) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        result[i] = block1[i] ^ block2[i];
    }
}

// Main function
int main() {
    uint8_t key[BLOCK_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    uint8_t message[BLOCK_SIZE] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20};
    uint8_t mac[BLOCK_SIZE];
    uint8_t x_xor_mac[BLOCK_SIZE];
    
    // Compute CBC-MAC for message X
    cbc_mac(message, BLOCK_SIZE, key, mac);
    
    // Prepare the extended message X || (X ? T)
    xor_blocks(message, mac, x_xor_mac);

    // Concatenate message and (message ? MAC) to form the extended message
    uint8_t extended_message[2 * BLOCK_SIZE];
    memcpy(extended_message, message, BLOCK_SIZE);
    memcpy(extended_message + BLOCK_SIZE, x_xor_mac, BLOCK_SIZE);

    // Compute CBC-MAC for the extended message
    uint8_t extended_mac[BLOCK_SIZE];
    cbc_mac(extended_message, 2 * BLOCK_SIZE, key, extended_mac);

    // Print results
    printf("Original MAC: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", mac[i]);
    }
    printf("\n");

    printf("Extended MAC: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02X ", extended_mac[i]);
    }
    printf("\n");

    return 0;
}

