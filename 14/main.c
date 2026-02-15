#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Raindeer {
    int speed;
    int duration;
    int rest;

    int duration_timer;
    int rest_timer;
    int distance;
    int points;
};

struct List {
    struct List *next;
    struct Raindeer raindeer;
};

struct List *new_list(struct List *next, int speed, int duration, int rest) {
    struct List *list = malloc(sizeof(struct List));
    *list = (struct List){next, {speed, duration, rest, 0, 0, 0, 0}};
    return list;
}

int maximum_distance(struct List *root) {
    int distance = 0;

    while (root != NULL) {
        if (root->raindeer.distance > distance) {
            distance = root->raindeer.distance;
        }

        root = root->next;
    }

    return distance;
}

void simulate_step_rainder(struct Raindeer *raindeer) {
    if (raindeer->duration_timer >= raindeer->duration && raindeer->rest_timer >= raindeer->rest) {
        raindeer->duration_timer = 0;
        raindeer->rest_timer = 0;
    }

    if (raindeer->duration_timer < raindeer->duration) {
        raindeer->duration_timer += 1;
        raindeer->distance += raindeer->speed;
    } else if (raindeer->rest_timer < raindeer->rest) {
        raindeer->rest_timer += 1;
    }
}

void simulate_step(struct List *root) {
    for (struct List *current = root; current != NULL; current = current->next) {
        simulate_step_rainder(&current->raindeer);
    }

    int distance = maximum_distance(root);

    for (struct List *current = root; current != NULL; current = current->next) {
        if (current->raindeer.distance < distance) continue;

        current->raindeer.points += 1;
    }
}

void simulate(struct List *root, int time) {
    for (int i = 0; i < time; i++) {
        simulate_step(root);
    }
}

int maximum_points(struct List *root) {
    int points = 0;

    while (root != NULL) {
        if (root->raindeer.points > points) {
            points = root->raindeer.points;
        }

        root = root->next;
    }

    return points;
}

int main(void) {
    struct List *root = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') break;

        int speed, duration, rest;

        if (sscanf(buffer, "%*s can fly %d km/s for %d seconds, but then must rest for %d seconds.", &speed, &duration, &rest) != 3) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        root = new_list(root, speed, duration, rest);
    }

    simulate(root, 2503);

    printf("%d\n", maximum_distance(root));
    printf("%d\n", maximum_points(root));

    return 0;
}
