#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

struct List {
    struct List *next;
    char *value;
};

struct Matrix {
    int *matrix;
    size_t capacity;
};

char* duplicate_string(const char *string) {
    if (string == NULL) return NULL;
    char *copy = malloc(strlen(string) + 1);
    strcpy(copy, string);
    return copy;
}

struct List *new_list(struct List *next, const char *value) {
    struct List *list = malloc(sizeof(struct List));
    *list = (struct List){next, duplicate_string(value)};
    return list;
}

size_t list_add(struct List **root, struct List *list) {
    size_t index;
    for (index = 0; *root != NULL; index++) {
        if (strcmp(list->value, (*root)->value) == 0) {
            return index;
        }

        root = &(*root)->next;
    }

    *root = list;
    return index;
}

void free_matrix(struct Matrix *matrix) {
    if (matrix->matrix != NULL) {
        free(matrix->matrix);
    }
    *matrix = (struct Matrix){0};
}

void matrix_set(struct Matrix *matrix, size_t from, size_t to, int distance) {
    if (matrix->capacity == 0) {
        matrix->capacity = 8;
        matrix->matrix = malloc(sizeof(int) * matrix->capacity * matrix->capacity);
        for (size_t i = 0; i < matrix->capacity * matrix->capacity; i++) {
            matrix->matrix[i] = -1;
        }
    }

    if (matrix->capacity <= from || matrix->capacity <= to) {
        size_t capacity = matrix->capacity;
        while (matrix->capacity <= from || matrix->capacity <= to) {
            matrix->capacity *= 2;
        }

        int *new_matrix = malloc(sizeof(int) * matrix->capacity * matrix->capacity);
        for (size_t i = 0; i < matrix->capacity * matrix->capacity; i++) {
            new_matrix[i] = -1;
        }

        for (size_t x = 0; x < matrix->capacity; x++) {
            for (size_t y = 0; y < matrix->capacity; y++) {
                new_matrix[y*matrix->capacity + x] = matrix->matrix[y*capacity + x];
            }
        }

        free(matrix->matrix);
        matrix->matrix = new_matrix;
    }

    matrix->matrix[from*matrix->capacity + to] = distance;
}

int matrix_get(const struct Matrix *matrix, size_t from, size_t to) {
    return matrix->matrix[from*matrix->capacity + to];
}

void debug_matrix(const struct Matrix *matrix) {
    printf("\n");
    for (size_t y = 0; y < matrix->capacity; y++) {
        for (size_t x = 0; x < matrix->capacity; x++) {
            if (matrix_get(matrix, y, x) == -1) {
                printf("0 ");
            } else {
                printf("1 ");
            }
        }

        printf("\n");
    }
    printf("\n");
}

struct Matrix matrix_copy(const struct Matrix *other) {
    struct Matrix matrix = {malloc(sizeof(int) * other->capacity * other->capacity), other->capacity};
    memcpy(matrix.matrix, other->matrix, sizeof(int) * other->capacity * other->capacity);
    return matrix;
}

bool matrix_is_empty(const struct Matrix *matrix) {
    for (size_t i = 0; i < matrix->capacity * matrix->capacity; i++) {
        if (matrix->matrix[i] != -1) return false;
    }
    return true;
}

int shortest_distance_impl(struct Matrix *matrix, size_t from) {
    int min_distance = INT_MAX;

    if (matrix_is_empty(matrix)) {
        return 0;
    }

    for (size_t to = 0; to < matrix->capacity; to++) {
        int distance = matrix_get(matrix, from, to);
        if (distance == -1) continue;

        struct Matrix copy = matrix_copy(matrix);
        for (size_t i = 0; i < matrix->capacity; i++) {
            matrix_set(&copy, i, from, -1);
            matrix_set(&copy, from, i, -1);
            matrix_set(&copy, i, to, -1);
        }

        int shortest = shortest_distance_impl(&copy, to);
        free_matrix(&copy);
        if (shortest == INT_MAX) continue;
        distance += shortest;

        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    return min_distance;
}

int shortest_distance(struct Matrix *matrix) {
    int min_distance = INT_MAX;

    for (size_t from = 0; from < matrix->capacity; from++) {
        int distance = shortest_distance_impl(matrix, from);
        
        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    return min_distance;
}

int longest_distance_impl(struct Matrix *matrix, size_t from) {
    int max_distance = INT_MIN;

    if (matrix_is_empty(matrix)) {
        return 0;
    }

    for (size_t to = 0; to < matrix->capacity; to++) {
        int distance = matrix_get(matrix, from, to);
        if (distance == -1) continue;

        struct Matrix copy = matrix_copy(matrix);
        for (size_t i = 0; i < matrix->capacity; i++) {
            matrix_set(&copy, i, from, -1);
            matrix_set(&copy, from, i, -1);
            matrix_set(&copy, i, to, -1);
        }

        int longest = longest_distance_impl(&copy, to);
        free_matrix(&copy);
        if (longest == INT_MIN) continue;
        distance += longest;

        if (distance > max_distance) {
            max_distance = distance;
        }
    }

    return max_distance;
}

int longest_distance(struct Matrix *matrix) {
    int max_distance = 0;

    for (size_t from = 0; from < matrix->capacity; from++) {
        int distance = longest_distance_impl(matrix, from);
        
        if (distance > max_distance) {
            max_distance = distance;
        }
    }

    return max_distance;
}

int main(void) {
    struct Matrix matrix = {0};
    struct List *root = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') break;

        char from[256], to[256];
        int distance;

        if (sscanf(buffer, "%s to %s = %d", from, to, &distance) != 3) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        size_t from_index = list_add(&root, new_list(NULL, from));
        size_t to_index = list_add(&root, new_list(NULL, to));
        matrix_set(&matrix, from_index, to_index, distance);
        matrix_set(&matrix, to_index, from_index, distance);
    }

    printf("Part 1: %d\n", shortest_distance(&matrix));
    printf("Part 2: %d\n", longest_distance(&matrix));

    return 0;
}
