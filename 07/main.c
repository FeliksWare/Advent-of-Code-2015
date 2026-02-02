#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

enum WireKind {
    LSHIFT,
    RSHIFT,
    AND,
    OR,
    NOT,
    SET,
};

struct Wire {
    enum WireKind kind;
    char *lhs;
    char *rhs;
};

struct Node {
    char *key;
    struct Wire value;
    uint16_t output;
    bool computed;
    struct Node *l, *r;
};

char* duplicate_string(const char *string) {
    if (string == NULL) return NULL;
    char *copy = malloc(strlen(string) + 1);
    strcpy(copy, string);
    return copy;
}

struct Node* new_node(char *key, struct Wire value) {
    struct Node *node = malloc(sizeof(struct Node));
    *node = (struct Node){duplicate_string(key), value, 0, false, NULL, NULL};
    return node;
}

void set_node(struct Node **root, struct Node *node) {
    while (*root != NULL) {
        int cmp = strcmp(node->key, (*root)->key);

        if (cmp < 0) {
            root = &(*root)->l;
        } else if (cmp > 0) {
            root = &(*root)->r;
        } else {
            break;
        }
    }

    *root = node;
}

uint16_t get(struct Node *root, const char *key) {
    struct Node *current = root;

    while (current != NULL) {
        int cmp = strcmp(key, current->key);

        if (cmp < 0) {
            current = current->l;
        } else if (cmp > 0) {
            current = current->r;
        } else {
            break;
        }
    }

    if (current == NULL) {
        return atoi(key);
    }

    if (!current->computed) switch (current->value.kind) {
        case LSHIFT: current->output = get(root, current->value.lhs) << get(root, current->value.rhs); break;
        case RSHIFT: current->output = get(root, current->value.lhs) >> get(root, current->value.rhs); break;
        case AND: current->output = get(root, current->value.lhs) & get(root, current->value.rhs); break;
        case OR: current->output = get(root, current->value.lhs) | get(root, current->value.rhs); break;
        case NOT: current->output = ~get(root, current->value.lhs); break;
        case SET: current->output = get(root, current->value.lhs); break;
    }

    current->computed = true;
    return current->output;

    assert(0);
}

void reset_node(struct Node *root) {
    if (root == NULL) return;
    root->computed = false;
    reset_node(root->l);
    reset_node(root->r);
}

int main(void) {
    struct Node *root = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') continue;

        char x[16] = {0}, y[16] = {0}, z[16] = {0};
        enum WireKind kind;

        if (sscanf(buffer, "%s LSHIFT %s -> %s", x, y, z) == 3) {
            kind = LSHIFT;
        } else if (sscanf(buffer, "%s RSHIFT %s -> %s", x, y, z) == 3) {
            kind = RSHIFT;
        } else if (sscanf(buffer, "%s AND %s -> %s", x, y, z) == 3) {
            kind = AND;
        } else if (sscanf(buffer, "%s OR %s -> %s", x, y, z) == 3) {
            kind = OR;
        } else if (sscanf(buffer, "NOT %s -> %s", x, z) == 2) {
            kind = NOT;
        } else if (sscanf(buffer, "%s -> %s", x, z) == 2) {
            kind = SET;
        } else {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
        }

        set_node(&root, new_node(z, (struct Wire){kind, duplicate_string(x), duplicate_string(y)}));
    }

    uint16_t wire_a = get(root, "a");
    printf("Part 1: %d\n", wire_a);
    reset_node(root);

    snprintf(buffer, sizeof(buffer), "%d", wire_a);
    set_node(&root, new_node("b", (struct Wire){SET, buffer, NULL}));

    wire_a = get(root, "a");
    printf("Part 2: %d\n", wire_a);

    return 0;
}
