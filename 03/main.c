#include <stdlib.h>
#include <stdio.h>

struct Node {
    int x, y;
    struct Node *l, *r;
};

struct Node* new_node(int x, int y) {
    struct Node *node = malloc(sizeof(struct Node));
    *node = (struct Node){x, y, NULL, NULL};
    return node;
}

void add_node(struct Node **root, struct Node *node) {
    while (*root != NULL) {
        if (node->y > (*root)->y) {
            root = &(*root)->r;
        } else if (node->y < (*root)->y) {
            root = &(*root)->l;
        } else if (node->x > (*root)->x) {
            root = &(*root)->r;
        } else if (node->x < (*root)->x) {
            root = &(*root)->l;
        } else {
            return;
        }
    }

    *root = node;
}

size_t count_nodes(struct Node *node) {
    if (node == NULL) {
        return 0;
    }

    size_t count = 0;
    for (;;) {
        count += 1;

        if (node->l == NULL && node->r == NULL) {
            return count;
        } else if (node->l == NULL) {
            node = node->r;
        } else if (node->r == NULL) {
            node = node->l;
        } else {
            count += count_nodes(node->r);
            node = node->l;
        }
    }
}

int main(void) {
    int x = 0, y = 0;
    int santa_x = 0, santa_y = 0;
    int robot_x = 0, robot_y = 0;
    struct Node *root = new_node(x, y);
    struct Node *robo_santa = new_node(santa_x, santa_y);

    for (int i = 0, c = getc(stdin); c != EOF; i++, c = getc(stdin)) {
        int delta_y = 0, delta_x = 0;

        switch (c) {
            case '^': delta_y = +1; break;
            case '>': delta_x = +1; break;
            case 'v': delta_y = -1; break;
            case '<': delta_x = -1; break;
            default: continue;
        }

        y += delta_y;
        x += delta_x;
        add_node(&root, new_node(x, y));

        if (i % 2 == 0) {
            santa_y += delta_y;
            santa_x += delta_x;
            add_node(&robo_santa, new_node(santa_x, santa_y));
        } else {
            robot_y += delta_y;
            robot_x += delta_x;
            add_node(&robo_santa, new_node(robot_x, robot_y));
        }
    }

    printf("Part 1: %zu\n", count_nodes(root));
    printf("Part 2: %zu\n", count_nodes(robo_santa));

    return 0;
}
