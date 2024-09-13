#include <stdio.h>
#include <string.h>
#include <ctype.h>

void generateKey(char key[], char map[26]) {
    int used[26] = {0};
    int j = 0;
    for (int i = 0; i < strlen(key); i++) {
        if (isalpha(key[i])) {
            char letter = toupper(key[i]) - 'A';
            if (!used[letter]) {
                map[j++] = letter + 'A';
                used[letter] = 1;
            }
        }
    }
    for (int i = 0; i < 26; i++) {
        if (!used[i]) {
            map[j++] = 'A' + i;
        }
    }
}

void monoalphabeticEncrypt(char text[], char keyMap[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = keyMap[toupper(text[i]) - 'A'] - ('A' - base);
        }
    }
}

int main() {
    char text[100], key[] = "CIPHER";
    char keyMap[26];

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);

    generateKey(key, keyMap);
    monoalphabeticEncrypt(text, keyMap);

    printf("Encrypted: %s\n", text);
    return 0;
}

