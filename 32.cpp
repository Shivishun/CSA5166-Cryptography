#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/dsa.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

// Function to generate a DSA signature for a given message
void generate_dsa_signature(const unsigned char *message, size_t message_len, unsigned char **sig, size_t *sig_len) {
    DSA *dsa = DSA_new();
    DSA_generate_parameters_ex(dsa, 2048, NULL, 0, NULL, NULL, NULL);
    DSA_generate_key(dsa);

    // Create a digest of the message
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(message, message_len, digest);

    // Generate the signature
    *sig = malloc(DSA_size(dsa));
    if (!DSA_sign(0, digest, SHA256_DIGEST_LENGTH, *sig, (unsigned int *)sig_len, dsa)) {
        fprintf(stderr, "Error generating DSA signature.\n");
        exit(1);
    }

    DSA_free(dsa);
}

// Function to generate an RSA signature for a given message
void generate_rsa_signature(const unsigned char *message, size_t message_len, unsigned char **sig, size_t *sig_len) {
    RSA *rsa = RSA_new();
    RSA_generate_key_ex(rsa, 2048, RSA_F4, NULL);

    // Create a digest of the message
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(message, message_len, digest);

    // Generate the signature
    *sig = malloc(RSA_size(rsa));
    if (!RSA_sign(NID_sha256, digest, SHA256_DIGEST_LENGTH, *sig, (unsigned int *)sig_len, rsa)) {
        fprintf(stderr, "Error generating RSA signature.\n");
        exit(1);
    }

    RSA_free(rsa);
}

// Function to print the signature in hex format
void print_signature(const char *label, unsigned char *sig, size_t sig_len) {
    printf("%s: ", label);
    for (size_t i = 0; i < sig_len; i++) {
        printf("%02x", sig[i]);
    }
    printf("\n");
}

int main() {
    // Define a message
    const unsigned char message[] = "This is a test message.";
    size_t message_len = strlen((const char *)message);

    // Generate and print DSA signatures for the same message
    unsigned char *dsa_sig1, *dsa_sig2;
    size_t dsa_sig_len1, dsa_sig_len2;

    generate_dsa_signature(message, message_len, &dsa_sig1, &dsa_sig_len1);
    generate_dsa_signature(message, message_len, &dsa_sig2, &dsa_sig_len2);

    print_signature("DSA Signature 1", dsa_sig1, dsa_sig_len1);
    print_signature("DSA Signature 2", dsa_sig2, dsa_sig_len2);

    // Generate and print RSA signatures for the same message
    unsigned char *rsa_sig1, *rsa_sig2;
    size_t rsa_sig_len1, rsa_sig_len2;

    generate_rsa_signature(message, message_len, &rsa_sig1, &rsa_sig_len1);
    generate_rsa_signature(message, message_len, &rsa_sig2, &rsa_sig_len2);

    print_signature("RSA Signature 1", rsa_sig1, rsa_sig_len1);
    print_signature("RSA Signature 2", rsa_sig2, rsa_sig_len2);

    // Free allocated memory
    free(dsa_sig1);
    free(dsa_sig2);
    free(rsa_sig1);
    free(rsa_sig2);

    return 0;
}

