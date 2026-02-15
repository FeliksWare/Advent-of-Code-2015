#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

struct Cookie {
    long long capacity;
    long long durability;
    long long flavor;
    long long texture;
    long long calories;
};

struct Node {
    struct Node *next;
    struct Cookie cookie;
};

struct Node* new_node(struct Node *next, long long capacity, long long durability, long long flavor, long long texture, long long calories) {
    struct Node *node = malloc(sizeof(struct Node));
    *node = (struct Node){next, {capacity, durability, flavor, texture, calories}};
    return node;
}

long long highest_score(struct Node *root, int teaspoons, struct Cookie cookie) {
    if (root == NULL) {
        if (teaspoons != 0) return 0;
        if (cookie.capacity < 0) return 0;
        if (cookie.durability < 0) return 0;
        if (cookie.flavor < 0) return 0;
        if (cookie.texture < 0) return 0;
        return cookie.capacity * cookie.durability * cookie.flavor * cookie.texture;
    }

    long long max_score = 0;

    for (int i = 0; i <= teaspoons; i++) {
        struct Cookie cookie_copy = cookie;
        cookie_copy.capacity += root->cookie.capacity * i;
        cookie_copy.durability += root->cookie.durability * i;
        cookie_copy.flavor += root->cookie.flavor * i;
        cookie_copy.texture += root->cookie.texture * i;

        long long score = highest_score(root->next, teaspoons - i, cookie_copy);

        if (score > max_score) {
            max_score = score;
        }
    }

    return max_score;
}

long long highest_score_exact_calories(struct Node *root, int teaspoons, struct Cookie cookie, long long calories) {
    if (root == NULL) {
        if (teaspoons != 0) return 0;
        if (cookie.capacity < 0) return 0;
        if (cookie.durability < 0) return 0;
        if (cookie.flavor < 0) return 0;
        if (cookie.texture < 0) return 0;
        if (cookie.calories != calories) return 0;
        return cookie.capacity * cookie.durability * cookie.flavor * cookie.texture;
    }

    long long max_score = 0;

    for (int i = 0; i <= teaspoons; i++) {
        struct Cookie cookie_copy = cookie;
        cookie_copy.capacity += root->cookie.capacity * i;
        cookie_copy.durability += root->cookie.durability * i;
        cookie_copy.flavor += root->cookie.flavor * i;
        cookie_copy.texture += root->cookie.texture * i;
        cookie_copy.calories += root->cookie.calories * i;

        long long score = highest_score_exact_calories(root->next, teaspoons - i, cookie_copy, calories);

        if (score > max_score) {
            max_score = score;
        }
    }

    return max_score;
}

int main(void) {
    struct Node *root = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') break;

        long long capacity, durability, flavor, texture, calories;

        char test[256];
        sscanf(buffer, "%s", test);

        if (
            sscanf(buffer, "%*[A-Za-z]: capacity %lld, durability %lld, flavor %lld, texture %lld, calories %lld",
            &capacity, &durability, &flavor, &texture, &calories) != 5
        ) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        root = new_node(root, capacity, durability, flavor, texture, calories);
    }

    printf("Part 1: %lld\n", highest_score(root, 100, (struct Cookie){0}));
    printf("Part 2: %lld\n", highest_score_exact_calories(root, 100, (struct Cookie){0}, 500));

    return 0;
}
