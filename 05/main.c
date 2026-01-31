#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_nice1(char *s) {
    int vowels = 0;
    int twice = 0;

    int p = -1;
    for (int c = *s++; c != '\0'; c = *s++) {
        vowels += c == 'a' || c == 'e' || c == 'i' || c == 'o' || c =='u';
        twice += c == p;
        if (p == 'a' && c == 'b') return false;
        if (p == 'c' && c == 'd') return false;
        if (p == 'p' && c == 'q') return false;
        if (p == 'x' && c == 'y') return false;

        p = c;
    }


    return vowels >= 3 && twice != 0;
}

bool is_nice2(char *s) {
    bool pair = false;
    bool inbetween = false;

    for (size_t i = 1; i < strlen(s) - 1; i++) {
        for (size_t j = i + 1; j < strlen(s) - 1; j++) {
            if (s[i-1] == s[j] && s[i] == s[j+1]) {
                pair = true;
            }
        }
        if (s[i-1] == s[i+1]) {
            inbetween = true;
        }

        if (pair && inbetween) {
            return true;
        }
    }

    return false;
}

int main(void) {
    char buffer[256];
    int count1 = 0;
    int count2 = 0;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (is_nice1(buffer)) {
            count1 += 1;
        }
        if (is_nice2(buffer)) {
            count2 += 1;
        }
    }

    printf("Part 1: %d\n", count1);
    printf("Part 2: %d\n", count2);

    return 0;
}
