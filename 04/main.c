#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <md5.h>

int main(void) {
    char prefix[256];
    if (fgets(prefix, sizeof(prefix), stdin) == NULL) {
        fprintf(stderr, "Error: failed to read prefix from stdin\n");
        return 1;
    }

    char *buffer = strchr(prefix, '\0');
    if (buffer == NULL) {
        fprintf(stderr, "Error: failed to find end of string\n");
        return 1;
    }

    unsigned char digest[16];
    struct MD5Context context;

    unsigned long long hash5 = 0;
    unsigned long long hash6 = 0;

    for (unsigned long long i = 1; i != 0; i++) {
        sprintf(buffer, "%llu", i);

        MD5Init(&context);
        MD5Update(&context, (unsigned char*)prefix, strlen(prefix));
        MD5Final(digest, &context);

        if (hash5 == 0 && digest[0] == 0x00 && digest[1] == 0x00 && digest[2] <= 0x0F) {
            hash5 = i;
        }
        if (hash6 == 0 && digest[0] == 0x00 && digest[1] == 0x00 && digest[2] == 0x00) {
            hash6 = i;
        }
        if (hash5 != 0 && hash6 != 0) {
            break;
        }
    }

    printf("Part 1: %llu\n", hash5);
    printf("Part 2: %llu\n", hash6);

    return 0;
}
