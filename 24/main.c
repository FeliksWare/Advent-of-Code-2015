#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_INITIAL_SIZE (32)
#define ARRAY_GROWTH_FACTOR (2)

struct Array {
    int *array;
    size_t capacity;
    size_t count;
};

struct State {
    long long qe;
    int *group;
    int *weight;
    int *count;

    long long best_qe;
    int best_count;
};

void array_reserve(struct Array *array, size_t minimum) {
    if (array->capacity > minimum) return;

    if (array->capacity == 0) {
        array->capacity = ARRAY_INITIAL_SIZE;
    }

    while (array->capacity < minimum) {
        array->capacity *= ARRAY_GROWTH_FACTOR;
    }

    array->array = realloc(array->array, sizeof(*array->array) * array->capacity);
}

void array_push(struct Array *array, int item) {
    array_reserve(array, array->count + 1);
    array->array[array->count++] = item;
}

int array_pop(struct Array *array) {
    return array->array[--array->count];
}

void group_impl(struct Array *weights, size_t index, int current_group, int groups, int target, struct State *state) {
    if (current_group >= groups) {
        state->best_qe = state->qe;
        state->best_count = state->count[0];
        return;
    }

    if (state->best_qe != 0) {
        if (state->count[0] > state->best_count) return;
        if (state->count[0] == state->best_count && state->qe >= state->best_qe) return;
    }
    if (state->weight[current_group] == target) {
        if (current_group > 0 && state->count[current_group - 1] > state->count[current_group]) {
            return;
        }
        group_impl(weights, 0, current_group + 1, groups, target, state);
        return;
    } else if (state->weight[current_group] > target) {
        return;
    }

    while (index < weights->count) {
        if (state->group[index] == -1) break;
        index += 1;
    }

    if (index >= weights->count) return;

    long long qe = state->qe;

    if (current_group == 0) {
        state->qe *= weights->array[index];
    }
    state->group[index] = current_group;
    state->weight[current_group] += weights->array[index];
    state->count[current_group] += 1;

    group_impl(weights, index + 1, current_group, groups, target, state);

    state->qe = qe;
    state->group[index] = -1;
    state->weight[current_group] -= weights->array[index];
    state->count[current_group] -= 1;

    group_impl(weights, index + 1, current_group, groups, target, state);
}

long long sleigh3(struct Array *weights) {
    int group[weights->count];
    int target = 0;

    for (size_t i = 0; i < weights->count; i++) {
        group[i] = -1;
        target += weights->array[i];
    }

    int group_weight[3];
    int group_count[3];

    memset(group_weight, 0, sizeof(group_weight));
    memset(group_count, 0, sizeof(group_count));

    struct State current = {0};
    current.qe = 1;
    current.group = group;
    current.weight = group_weight;
    current.count = group_count;

    group_impl(weights, 0, 0, 3, target / 3, &current);
    return current.best_qe;
}

long long sleigh4(struct Array *weights) {
    int group[weights->count];
    int target = 0;

    for (size_t i = 0; i < weights->count; i++) {
        group[i] = -1;
        target += weights->array[i];
    }

    int group_weight[4];
    int group_count[4];

    memset(group_weight, 0, sizeof(group_weight));
    memset(group_count, 0, sizeof(group_count));

    struct State current = {0};
    current.qe = 1;
    current.group = group;
    current.weight = group_weight;
    current.count = group_count;

    group_impl(weights, 0, 0, 4, target / 4, &current);
    return current.best_qe;
}

int main(void) {
    char buffer[256];

    struct Array array = {0};

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] =='\n') continue;

        int weight = 0;

        if (sscanf(buffer, "%d", &weight) != 1) {
            fprintf(stderr, "ERROR: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        array_push(&array, weight);
    }

    size_t left = 0;
    size_t right = array.count - 1;

    while (left < right) {
        int temp = array.array[left];
        array.array[left] = array.array[right];
        array.array[right] = temp;

        left++;
        right--;
    }

    printf("Part 1: %lld\n", sleigh3(&array));
    printf("Part 2: %lld\n", sleigh4(&array));
        
    return 0;
}

