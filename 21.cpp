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

void des_decrypt_ecb(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key, size_t length) {
    DES_key_schedule ks;
    DES_cblock des_key;
    
    // Set up the key
    memcpy(des_key, key, DES_KEY_SZ);
    DES_set_key_unchecked(&des_key, &ks);

    size_t num_blocks = length / BLOCK_SIZE;

    for (size_t i = 0; i < num_blocks; i++) {
        DES_ecb_encrypt((DES_cblock *)(ciphertext + i * BLOCK_SIZE), (DES_cblock *)(plaintext + i * BLOCK_SIZE), &ks, DES_DECRYPT);
    }
}

void des_encrypt_cbc(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key, const unsigned char *iv, size_t length) {
    DES_key_schedule ks;
    DES_cblock des_key;
    DES_cblock des_iv;

    // Set up the key and IV
    memcpy(des_key, key, DES_KEY_SZ);
    memcpy(des_iv, iv, DES_KEY_SZ);
    DES_set_key_unchecked(&des_key, &ks);

    size_t num_blocks = length / BLOCK_SIZE;
    DES_cblock prev_block;

    memcpy(prev_block, des_iv, BLOCK_SIZE);

    for (size_t i = 0; i < num_blocks; i++) {
        DES_cblock input_block;
        memcpy(input_block, plaintext + i * BLOCK_SIZE, BLOCK_SIZE);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            input_block[j] ^= prev_block[j];
        }

        DES_ecb_encrypt(&input_block, &input_block, &ks, DES_ENCRYPT);
        memcpy(ciphertext + i * BLOCK_SIZE, input_block, BLOCK_SIZE);
        memcpy(prev_block, input_block, BLOCK_SIZE);
    }
}

void des_decrypt_cbc(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key, const unsigned char *iv, size_t length) {
    DES_key_schedule ks;
    DES_cblock des_key;
    DES_cblock des_iv;

    // Set up the key and IV
    memcpy(des_key, key, DES_KEY_SZ);
    memcpy(des_iv, iv, DES_KEY_SZ);
    DES_set_key_unchecked(&des_key, &ks);

    size_t num_blocks = length / BLOCK_SIZE;
    DES_cblock prev_block;
    memcpy(prev_block, des_iv, BLOCK_SIZE);

    for (size_t i = 0; i < num_blocks; i++) {
        DES_cblock input_block;
        memcpy(input_block, ciphertext + i * BLOCK_SIZE, BLOCK_SIZE);

        DES_cblock decrypted_block;
        DES_ecb_encrypt(&input_block, &decrypted_block, &ks, DES_DECRYPT);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i * BLOCK_SIZE + j] = decrypted_block[j] ^ prev_block[j];
        }

        memcpy(prev_block, input_block, BLOCK_SIZE);
    }
}

void des_encrypt_cfb(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key, const unsigned char *iv, size_t length) {
    DES_key_schedule ks;
    DES_cblock des_key;
    DES_cblock des_iv;

    // Set up the key and IV
    memcpy(des_key, key, DES_KEY_SZ);
    memcpy(des_iv, iv, DES_KEY_SZ);
    DES_set_key_unchecked(&des_key, &ks);

    size_t num_blocks = length / BLOCK_SIZE;
    DES_cblock feedback;

    memcpy(feedback, des_iv, BLOCK_SIZE);

    for (size_t i = 0; i < num_blocks; i++) {
        DES_cblock input_block;
        memcpy(input_block, plaintext + i * BLOCK_SIZE, BLOCK_SIZE);

        DES_cblock encrypted_feedback;
        DES_ecb_encrypt(&feedback, &encrypted_feedback, &ks, DES_ENCRYPT);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            ciphertext[i * BLOCK_SIZE + j] = input_block[j] ^ encrypted_feedback[j];
        }

        memcpy(feedback, ciphertext + i * BLOCK_SIZE, BLOCK_SIZE);
    }
}

void des_decrypt_cfb(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key, const unsigned char *iv, size_t length) {
    DES_key_schedule ks;
    DES_cblock des_key;
    DES_cblock des_iv;

    // Set up the key and IV
    memcpy(des_key, key, DES_KEY_SZ);
    memcpy(des_iv, iv, DES_KEY_SZ);
    DES_set_key_unchecked(&des_key, &ks);

    size_t num_blocks = length / BLOCK_SIZE;
    DES_cblock feedback;

    memcpy(feedback, des_iv, BLOCK_SIZE);

    for (size_t i = 0; i < num_blocks; i++) {
        DES_cblock encrypted_feedback;
        DES_ecb_encrypt(&feedback, &encrypted_feedback, &ks, DES_ENCRYPT);

        DES_cblock ciphertext_block;
        memcpy(ciphertext_block, ciphertext + i * BLOCK_SIZE, BLOCK_SIZE);

        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i * BLOCK_SIZE + j] = ciphertext_block[j] ^ encrypted_feedback[j];
        }

        memcpy(feedback, ciphertext + i * BLOCK_SIZE, BLOCK_SIZE);
    }
}

void pad_plaintext(unsigned char *plaintext, size_t *length) {
    size_t padding_size = BLOCK_SIZE - (*length % BLOCK_SIZE);
    if (padding_size == BLOCK_SIZE) padding_size = 0;

    memset(plaintext + *length, 0, padding_size);
    plaintext[*length] = 0x80; // Padding with 1 bit followed by zero bits
    *length += padding_size + 1;
}

int main() {
    const unsigned char key[DES_KEY_SZ] = "12345678";  // 8-byte key for DES
    const unsigned char iv[BLOCK_SIZE] = "abcdefgh";    // Initialization vector for CBC and CFB modes

    unsigned char plaintext[] = "meet me at the usual place at ten rather than eight oclock";
    size_t length = sizeof(plaintext) - 1;

    // Padding plaintext to be a multiple of block size
    pad_plaintext(plaintext, &length);

    unsigned char ciphertext[sizeof(plaintext)];
    unsigned char decrypted[sizeof(plaintext)];

    printf("Original Plaintext: %s\n", plaintext);

    // ECB mode
    des_encrypt_ecb(plaintext, ciphertext, key, length);
    des_decrypt_ecb(ciphertext, decrypted, key, length);
    printf("ECB Decrypted: %s\n", decrypted);

    // CBC mode
    des_encrypt_cbc(plaintext, ciphertext, key, iv, length);
    des_decrypt_cbc(ciphertext, decrypted, key, iv, length);
    printf("CBC Decrypted: %s\n", decrypted);

    // CFB mode
    des_encrypt_cfb(plaintext, ciphertext, key, iv, length);
    des_decrypt_cfb(ciphertext, decrypted, key, iv, length);
    printf("CFB Decrypted: %s\n", decrypted);

    return 0;
}

