#include <stdio.h>

int main(void) {
    int floor = 0;
    int basement = 0;

    for (int c = getc(stdin), i = 1; c != EOF; c = getc(stdin), i++) {
        switch (c) {
            case '(': floor += 1; break;
            case ')': floor -= 1; break;
        }

        if (basement == 0 && floor == -1) {
            basement = i;
        }
    }

    printf("Floor: %d\n", floor);
    printf("Basement: %d\n", basement);

    return 0;
}
