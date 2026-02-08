#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void increment_password(char password[8]) {
    for (int i = 7; i >= 0; i--) {
        if (password[i] == 'z') {
            password[i] = 'a';
        } else {
            password[i] += 1;
            break;
        }
    }

    for (int i = 0; i < 8; i++) {
        switch (password[i]) {
            case 'i': case 'o': case 'l': password[i] += 1;
        }
    }
}

bool valid_password(char password[8]) {
    bool rising = false;
    int pairs = 0;

    for (int i = 0; i < 7; i++) {
        if (password[i] == password[i + 1]) {
            pairs += 1;
            i += 1;
        }

        if (i < 6 && password[i] + 1 == password[i + 1] && password[i + 1] + 1 == password[i + 2]) {
            rising = true;
        }

        if (pairs >= 2 && rising) return true;
    }

    return false;
}

int main(void) {
    char password[8];
    fread(password, 1, sizeof(password), stdin);

    do {
        increment_password(password);
    } while (!valid_password(password));

    printf("Part 1: %s\n", password);

    do {
        increment_password(password);
    } while (!valid_password(password));

    printf("Part 2: %s\n", password);

    return 0;
}
