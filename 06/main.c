#include <stdio.h>

static char lights1[1000 * 1000] = {0}; 
static int lights2[1000 * 1000] = {0}; 

int main(void) {
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') continue;

        int from_x, from_y, to_x, to_y;

        if (sscanf(buffer, "turn on %d,%d through %d,%d", &from_x, &from_y, &to_x, &to_y) == 4) {
            for (int x = from_x; x <= to_x; x++) for (int y = from_y; y <= to_y; y++) {
                lights1[y * 1000 + x] = 1;
                lights2[y * 1000 + x] += 1;
            }
        } else if (sscanf(buffer, "turn off %d,%d through %d,%d", &from_x, &from_y, &to_x, &to_y) == 4) {
            for (int x = from_x; x <= to_x; x++) for (int y = from_y; y <= to_y; y++) {
                lights1[y * 1000 + x] = 0;
                lights2[y * 1000 + x] -= 1;
                if (lights2[y * 1000 + x] < 0) {
                    lights2[y * 1000 + x] = 0;
                }
            }
        } else if (sscanf(buffer, "toggle %d,%d through %d,%d", &from_x, &from_y, &to_x, &to_y) == 4) {
            for (int x = from_x; x <= to_x; x++) for (int y = from_y; y <= to_y; y++) {
                lights1[y * 1000 + x] = !lights1[y * 1000 + x];
                lights2[y * 1000 + x] += 2;
            }
        } else {
            fprintf(stderr, "Error: failed to parse \"%s\"", buffer);
            return 1;
        }
    }

    int count1 = 0;
    int count2 = 0;

    for (int i = 0; i < 1000 * 1000; i++) {
        if (lights1[i]) {
            count1 += 1;
        }
        count2 += lights2[i];
    }

    printf("Part 1: %d\n", count1);
    printf("Part 2: %d\n", count2);

    return 0;
}
