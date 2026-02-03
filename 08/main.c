#include <stdio.h>
#include <string.h>

int main(void) {
    char buffer[256];

    size_t total_string = 0;
    size_t total_memory = 0;
    size_t total_encoded = 0;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') continue;
        size_t length = strlen(buffer);

        total_string += length;
        total_memory += length - 2;
        total_encoded += length + 4;

        for (size_t i = 0; i < length; i++) {
            if (buffer[i] != '\\') continue;

            i += 1;
            if (buffer[i] == '\0') break;

            if (buffer[i] == '\\' || buffer[i] == '"') {
                total_memory -= 1;
                total_encoded += 2;
            } else if (buffer[i] == 'x') {
                total_memory -= 3;
                total_encoded += 1;
            }
        }
    }

    printf("Part 1: %zu\n", total_string - total_memory);
    printf("Part 1: %zu\n", total_encoded - total_string);
}
