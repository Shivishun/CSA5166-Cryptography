#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Constants for block sizes
#define BLOCK_SIZE_64 8   // 64 bits = 8 bytes
#define BLOCK_SIZE_128 16 // 128 bits = 16 bytes

// XOR constant based on block size
#define CONSTANT_64 0x1B
#define CONSTANT_128 0x87

// Function to perform a left shift with carry
void left_shift(uint8_t *block, size_t size) {
    uint8_t carry = 0;
    for (int i = size - 1; i >= 0; i--) {
        uint8_t new_carry = (block[i] & 0x80) ? 1 : 0;
        block[i] = (block[i] << 1) | carry;
        carry = new_carry;
    }
}

// Function to XOR a block with a constant
void xor_with_constant(uint8_t *block, uint8_t constant, size_t size) {
    for (int i = size - 1; i >= 0; i--) {
        block[i] ^= constant;
    }
}

// Function to generate CMAC subkeys
void generate_cmac_subkeys(uint8_t *key, uint8_t *subkey1, uint8_t *subkey2, size_t size) {
    uint8_t zero_block[BLOCK_SIZE_128] = {0}; // Block of all zero bits

    // Apply the block cipher (simulated here as an identity function for demonstration)
    // In practice, apply the actual block cipher (e.g., AES) to the zero block
    memcpy(subkey1, zero_block, size);

    // Left shift and XOR to get subkey1
    left_shift(subkey1, size);
    if (subkey1[0] & 0x80) { // Check if the most significant bit is set
        xor_with_constant(subkey1, (size == BLOCK_SIZE_64) ? CONSTANT_64 : CONSTANT_128, size);
    }

    // Generate subkey2 from subkey1
    memcpy(subkey2, subkey1, size);
    left_shift(subkey2, size);
    if (subkey2[0] & 0x80) { // Check if the most significant bit is set
        xor_with_constant(subkey2, (size == BLOCK_SIZE_64) ? CONSTANT_64 : CONSTANT_128, size);
    }
}

// Function to print a block
void print_block(const char *label, uint8_t *block, size_t size) {
    printf("%s: ", label);
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");
}

int main() {
    uint8_t key[BLOCK_SIZE_128] = {0}; // Key block (all zeros)
    uint8_t subkey1[BLOCK_SIZE_128];
    uint8_t subkey2[BLOCK_SIZE_128];

    // Generate subkeys for 128-bit block size
    generate_cmac_subkeys(key, subkey1, subkey2, BLOCK_SIZE_128);

    // Print subkeys
    print_block("Subkey 1", subkey1, BLOCK_SIZE_128);
    print_block("Subkey 2", subkey2, BLOCK_SIZE_128);

    return 0;
}

