#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(void) {
    int children = 3;
    int cats = 7;
    int samoyeds = 2;
    int pomeranians = 3;
    int akitas = 0;
    int vizslas = 0;
    int goldfish = 5;
    int trees = 3;
    int cars = 2;
    int perfumes = 1;

    char buffer[256];

    int sue_part1 = -1;
    int sue_part2 = -1;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') continue;

        int sue;
        char key[3][256];
        int value[3];

        if (
            sscanf(buffer, "Sue %d: %[A-Za-z]: %d, %[A-Za-z]: %d, %[A-Za-z]: %d",
            &sue, key[0], &value[0], key[1], &value[1], key[2], &value[2]) != 7
        ) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        bool correct1 = true;
        bool correct2 = true;

        for (int i = 0; i < 3; i++) {
            if (strcmp(key[i], "children") == 0 && value[i] == children);
            else if (strcmp(key[i], "cats") == 0 && value[i] == cats);
            else if (strcmp(key[i], "samoyeds") == 0 && value[i] == samoyeds);
            else if (strcmp(key[i], "pomeranians") == 0 && value[i] == pomeranians);
            else if (strcmp(key[i], "akitas") == 0 && value[i] == akitas);
            else if (strcmp(key[i], "vizslas") == 0 && value[i] == vizslas);
            else if (strcmp(key[i], "goldfish") == 0 && value[i] == goldfish);
            else if (strcmp(key[i], "trees") == 0 && value[i] == trees);
            else if (strcmp(key[i], "cars") == 0 && value[i] == cars);
            else if (strcmp(key[i], "perfumes") == 0 && value[i] == perfumes);
            else {
                correct1 = false;
            }

            if (strcmp(key[i], "children") == 0 && value[i] == children);
            else if (strcmp(key[i], "cats") == 0 && value[i] > cats);
            else if (strcmp(key[i], "samoyeds") == 0 && value[i] == samoyeds);
            else if (strcmp(key[i], "pomeranians") == 0 && value[i] < pomeranians);
            else if (strcmp(key[i], "akitas") == 0 && value[i] == akitas);
            else if (strcmp(key[i], "vizslas") == 0 && value[i] == vizslas);
            else if (strcmp(key[i], "goldfish") == 0 && value[i] < goldfish);
            else if (strcmp(key[i], "trees") == 0 && value[i] > trees);
            else if (strcmp(key[i], "cars") == 0 && value[i] == cars);
            else if (strcmp(key[i], "perfumes") == 0 && value[i] == perfumes);
            else {
                correct2 = false;
            }
        }

        if (correct1) {
            sue_part1 = sue;
        }

        if (correct2) {
            sue_part2 = sue;
        } 
    }

    printf("Part 1: %d\n", sue_part1);
    printf("Part 2: %d\n", sue_part2);

    return 0;
}
