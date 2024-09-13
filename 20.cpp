#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>  // Requires OpenSSL

#define BLOCK_SIZE 8

void des_encrypt_ecb(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key, size_t length) {
    DES_key_schedule ks;
    DES_cblock des_key;
    
    // Set up the key
    memcpy(des_key, key, DES_KEY_SZ);
    DES_set_key_unchecked(&des_key, &ks);

    size_t num_blocks = length / BLOCK_SIZE;

    for (size_t i = 0; i < num_blocks; i++) {
        DES_ecb_encrypt((DES_cblock *)(plaintext + i * BLOCK_SIZE), (DES_cblock *)(ciphertext + i * BLOCK_SIZE), &ks, DES_ENCRYPT);
    }
}

int main() {
    const unsigned char key[DES_KEY_SZ] = "12345678";  // 8-byte key for DES
    const unsigned char plaintext[] = "This is a test message for ECB mode encryption with DES.";
    unsigned char ciphertext[sizeof(plaintext)];

    size_t length = sizeof(plaintext) - 1;  // Exclude the null terminator

    des_encrypt_ecb(plaintext, ciphertext, key, length);

    printf("Ciphertext: ");
    for (size_t i = 0; i < length; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    return 0;
}

