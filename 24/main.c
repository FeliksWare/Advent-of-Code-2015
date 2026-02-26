#include <stdio.h>
#include <stdlib.h>

struct Node {
    struct Node *next;
    int weight;
};

struct State {
    long long qe;
    int weight[3];
    int count[3];
};

struct Node* new_node(struct Node *next, int weight) {
    struct Node *node = malloc(sizeof(struct Node));
    *node = (struct Node){next, weight};
    return node;
}

void debug_state(struct State state) {
    printf("QE: %lld\n", state.qe);
    printf("Group 1: %dkg (%d)\n", state.weight[0], state.count[0]);
    printf("Group 2: %dkg (%d)\n", state.weight[1], state.count[1]);
    printf("Group 3: %dkg (%d)\n", state.weight[2], state.count[2]);
    printf("\n");
}

void sleigh3_impl(struct Node *root, int target, struct State *current, struct State *best) {
    if (best->qe != 0) {
        if (current->count[0] > best->count[0]) return;
        if (current->count[0] == best->count[0] && current->qe > best->qe) return;
    }

    if (current->weight[0] > target) return;
    if (current->weight[1] > target) return;
    if (current->weight[2] > target) return;

    if (root == NULL) {
        if (current->count[0] > current->count[1]) return;
        if (current->count[1] > current->count[2]) return;
        *best = *current;
        return;
    }

    long long qe = current->qe;
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            current->qe = qe * root->weight;
        }
        current->weight[i] += root->weight;
        current->count[i] += 1;

        sleigh3_impl(root->next, target, current, best);

        current->qe = qe;
        current->weight[i] -= root->weight;
        current->count[i] -= 1;
    }
}

long long test(struct Node *root, int target) {
    struct State current = {0};
    struct State best = {0};
    current.qe = 1;
    sleigh3_impl(root, target, &current, &best);

    return best.qe;
}

int main(void) {
    char buffer[256];

    struct Node *root = NULL;
    int sum = 0;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] =='\n') continue;

        int weight = 0;

        if (sscanf(buffer, "%d", &weight) != 1) {
            fprintf(stderr, "ERROR: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        root = new_node(root, weight);
        sum += weight;
    }

    printf("Part 1: %lld\n", test(root, sum / 3));
        
    return 0;
}
