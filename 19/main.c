#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

struct List {
    struct List *next;
    char *key;
    char *value;
};

struct Substitution {
    const char *original;
    const char *start;
    const char *end;
    const char *substitution;
};

struct Tree {
    struct Substitution value;
    struct Tree *left;
    struct Tree *right;
};

int compare_substitution(struct Substitution lhs, struct Substitution rhs) {
    int lhs_value = 0;
    int rhs_value = 0;

    for (;;) {
        if (lhs.original == lhs.start) {
            lhs_value = *lhs.substitution++;
            if (lhs_value == '\0') lhs.original = lhs.end;
        } 
        if (lhs.original != lhs.start) {
            lhs_value = *lhs.original++;
        }

        if (rhs.original == rhs.start) {
            rhs_value = *rhs.substitution++;
            if (rhs_value == '\0') rhs.original = rhs.end;
        } 
        if (rhs.original != rhs.start) {
            rhs_value = *rhs.original++;
        }

        if (lhs_value != rhs_value || lhs_value == 0 || rhs_value == 0) {
            break;
        }
    }

    return lhs_value - rhs_value;
}

char *substitution_to_string(struct Substitution substitution) {
    char *string = malloc(strlen(substitution.original) + strlen(substitution.substitution));

    int value = 0;
    size_t length = 0;

    for (;;) {
        if (substitution.original == substitution.start) {
            value = *substitution.substitution++;
            if (value == '\0') substitution.original = substitution.end;
        } 
        if (substitution.original != substitution.start) {
            value = *substitution.original++;
        }

        string[length++] = value;

        if (value == 0) {
            break;
        }
    }

    return realloc(string, length);
}

char* duplicate_string(const char *string) {
    if (string == NULL) return NULL;
    char *copy = malloc(strlen(string) + 1);
    strcpy(copy, string);
    return copy;
}

struct List* new_list(struct List *next, const char *key, const char *value) {
    struct List *list = malloc(sizeof(struct List));
    *list = (struct List){next, duplicate_string(key), duplicate_string(value)};
    return list;
}

struct Tree* new_tree(struct Substitution value, struct Tree *left, struct Tree *right) {
    struct Tree *tree = malloc(sizeof(struct Tree));
    *tree = (struct Tree){value, left, right};
    return tree;
}

void tree_add(struct Tree **root, struct Substitution value) {
    while (*root != NULL) {
        int compare = compare_substitution(value, (*root)->value);

        if (compare < 0) {
            root = &(*root)->left;
        } else if (compare > 0) {
            root = &(*root)->right;
        } else {
            return;
        }
    }

    *root = new_tree(value, NULL, NULL);
}

size_t tree_count(const struct Tree *root) {
    if (root == NULL) return 0;
    return 1 + tree_count(root->left) + tree_count(root->right);
}

size_t reduce(const struct List *list, char *molecule, size_t count) {
    if (strcmp(molecule, "e\n") == 0) {
        return count;
    }

    for (size_t i = strlen(molecule); i > 0; i--) {
        for (const struct List *current = list; current != NULL; current = current->next) {
            if (strncmp(&molecule[i - 1], current->value, strlen(current->value)) != 0) continue;

            struct Substitution substitution;
            substitution.original = molecule;
            substitution.start = &molecule[i - 1];
            substitution.end = &molecule[i - 1 + strlen(current->value)];
            substitution.substitution = current->key;

            size_t temp = reduce(list, substitution_to_string(substitution), count + 1);
            if (temp != SIZE_MAX) return temp;
        }
    }

    free(molecule);
    return SIZE_MAX;
}

int main(void) {
    struct List *list = NULL;
    struct Tree *tree = NULL;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') break;

        char key[256];
        char value[256];

        if (sscanf(buffer, "%s => %s", key, value) != 2) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        list = new_list(list, key, value);
    }

    fgets(buffer, sizeof(buffer), stdin);

    for (size_t i = 0; buffer[i] != '\0'; i++) {
        for (struct List *current = list; current != NULL; current = current->next) {
            if (strncmp(&buffer[i], current->key, strlen(current->key)) != 0) continue;

            struct Substitution substitution;
            substitution.original = buffer;
            substitution.start = &buffer[i];
            substitution.end = &buffer[i + strlen(current->key)];
            substitution.substitution = current->value;

            tree_add(&tree, substitution);
        }
    }

    printf("Part 1: %zu\n", tree_count(tree));
    printf("Part 2: %zu\n", reduce(list, duplicate_string(buffer), 0));

    return 0;
}
