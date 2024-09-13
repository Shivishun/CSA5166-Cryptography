#include <stdio.h>

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++)
        if ((a * x) % m == 1)
            return x;
    return 1;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

void affineEncrypt(char text[], int a, int b) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = (a * (text[i] - 'a') + b) % 26 + 'a';
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = (a * (text[i] - 'A') + b) % 26 + 'A';
        }
    }
}

void affineDecrypt(char text[], int a, int b) {
    int a_inv = modInverse(a, 26);
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = (a_inv * (text[i] - 'a' - b + 26)) % 26 + 'a';
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = (a_inv * (text[i] - 'A' - b + 26)) % 26 + 'A';
        }
    }
}

int main() {
    char text[100];
    int a, b;
    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    printf("Enter a (should be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter b: ");
    scanf("%d", &b);

    if (gcd(a, 26) != 1) {
        printf("Invalid value of 'a'. It must be coprime with 26.\n");
        return 1;
    }

    affineEncrypt(text, a, b);
    printf("Encrypted: %s\n", text);

    affineDecrypt(text, a, b);
    printf("Decrypted: %s\n", text);

    return 0;
}

