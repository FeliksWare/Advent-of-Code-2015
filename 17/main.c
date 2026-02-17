#include <stdlib.h>
#include <stdio.h>

#define LITERS (150)

struct Node {
    struct Node *next;
    int liters;
};

struct Node* new_node(struct Node *next, int liters) {
    struct Node *node = malloc(sizeof(struct Node));
    *node = (struct Node){next, liters};
    return node;
}

int combinations(struct Node *root, int liters) {
    if (liters > LITERS) return 0;
    if (root == NULL) {
        if (liters == LITERS) return 1;
        return 0;
    }

    return combinations(root->next, liters) + combinations(root->next, liters + root->liters);
}

int combinations_min_containers(struct Node *root, int liters, int *containers) {
    if (liters > LITERS) return 0;
    if (root == NULL) {
        if (liters == LITERS) return 1;
        return 0;
    }

    int containers_a = *containers;
    int solutions_a = combinations_min_containers(root->next, liters, &containers_a);

    int containers_b = *containers + 1;
    int solutions_b = combinations_min_containers(root->next, liters + root->liters, &containers_b);

    if (solutions_a == 0) {
        *containers = containers_b;
        return solutions_b;
    }

    if (solutions_b == 0) {
        *containers = containers_a;
        return solutions_a;
    }

    if (containers_a == containers_b) {
        *containers = containers_a;
        return solutions_a + solutions_b;
    } else if (containers_a < containers_b) {
        *containers = containers_a;
        return solutions_a;
    } else {
        *containers = containers_b;
        return solutions_b;
    }
}

int main(void) {
    struct Node *root = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') continue;

        int liters;

        if (sscanf(buffer, "%d", &liters) != 1) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        root = new_node(root, liters);
    }

    printf("Part 1: %d\n", combinations(root, 0));
    int containers = 0;
    printf("Part 2: %d\n", combinations_min_containers(root, 0, &containers));

    return 0;
}
