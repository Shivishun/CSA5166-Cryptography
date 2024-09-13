#include <stdio.h>

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return -1;
}

void affineDecrypt(char text[], int a, int b) {
    int a_inv = modInverse(a, 26);
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = (a_inv * (text[i] - 'A' - b + 26)) % 26 + 'A';
        }
    }
}

int main() {
    char text[] = "B....U....";  // Replace with actual ciphertext
    int a, b;

    // Frequency analysis shows B -> E, U -> T
    for (a = 1; a < 26; a++) {
        for (b = 0; b < 26; b++) {
            char decrypted[100];
            strcpy(decrypted, text);
            affineDecrypt(decrypted, a, b);
            printf("a = %d, b = %d: %s\n", a, b, decrypted);
        }
    }
    return 0;
}

