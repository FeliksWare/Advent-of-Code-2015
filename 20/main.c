#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int presents = 0;
    int house;
    scanf("%d", &presents);
    int size = presents / 10;

    int *presents_delivered = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        presents_delivered[i] = 0;
    }

    for (int i = 1; i < size; i++) {
        for (int j = i; j < size; j += i) {
            presents_delivered[j] += i * 10;
        }
    }

    for (house = 0; presents_delivered[house] < presents; house++);

    printf("Part 1: %d\n", house);

    for (int i = 0; i < size; i++) {
        presents_delivered[i] = 0;
    }

    for (int i = 1; i < size; i++) {
        for (int j = i; j < size && j <= i * 50; j += i) {
            presents_delivered[j] += i * 11;
        }
    }

    for (house = 0; presents_delivered[house] < presents; house++);

    printf("Part 2: %d\n", house);

    free(presents_delivered);

    return 0;
}
