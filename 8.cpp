#include <stdio.h>
#include <string.h>
#include <ctype.h>

void generateKeyMap(char key[], char keyMap[26]) {
    int used[26] = {0};
    int keyLen = strlen(key);
    int mapIndex = 0;

    // Add unique letters from the keyword to the cipher map
    for (int i = 0; i < keyLen; i++) {
        if (isalpha(key[i])) {
            char letter = toupper(key[i]) - 'A';
            if (!used[letter]) {
                keyMap[mapIndex++] = letter + 'A';
                used[letter] = 1;
            }
        }
    }

    // Add the remaining letters of the alphabet
    for (int i = 0; i < 26; i++) {
        if (!used[i]) {
            keyMap[mapIndex++] = 'A' + i;
        }
    }
}

void encryptMonoalphabetic(char text[], char keyMap[]) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = keyMap[toupper(text[i]) - 'A'] - ('A' - base);
        }
    }
}

void decryptMonoalphabetic(char text[], char keyMap[]) {
    char reverseMap[26];

    // Create reverse mapping for decryption
    for (int i = 0; i < 26; i++) {
        reverseMap[keyMap[i] - 'A'] = 'A' + i;
    }

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            text[i] = reverseMap[toupper(text[i]) - 'A'] - ('A' - base);
        }
    }
}

int main() {
    char text[100], key[] = "CIPHER";
    char keyMap[26];

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';  // Remove newline character

    generateKeyMap(key, keyMap);

    printf("Original text: %s\n", text);
    
    encryptMonoalphabetic(text, keyMap);
    printf("Encrypted text: %s\n", text);
    
    decryptMonoalphabetic(text, keyMap);
    printf("Decrypted text: %s\n", text);

    return 0;
}

