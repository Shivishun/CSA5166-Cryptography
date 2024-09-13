#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encrypt(char text[], char key[]) {
    int textLen = strlen(text);
    int keyLen = strlen(key);
    for (int i = 0, j = 0; i < textLen; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            char keyBase = isupper(key[j % keyLen]) ? 'A' : 'a';
            text[i] = (text[i] - base + (key[j % keyLen] - keyBase)) % 26 + base;
            j++;
        }
    }
}

void decrypt(char text[], char key[]) {
    int textLen = strlen(text);
    int keyLen = strlen(key);
    for (int i = 0, j = 0; i < textLen; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            char keyBase = isupper(key[j % keyLen]) ? 'A' : 'a';
            text[i] = (text[i] - base - (key[j % keyLen] - keyBase) + 26) % 26 + base;
            j++;
        }
    }
}

int main() {
    char text[100], key[100];
    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';
    printf("Enter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';
    encrypt(text, key);
    printf("Encrypted: %s\n", text);
    decrypt(text, key);
    printf("Decrypted: %s\n", text);
    return 0;
}

