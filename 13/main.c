#include <stddef.h>
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
    size_t count;
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
            matrix->matrix[i] = 0;
        }
    }

    if (matrix->capacity <= from || matrix->capacity <= to) {
        size_t capacity = matrix->capacity;
        while (matrix->capacity <= from || matrix->capacity <= to) {
            matrix->capacity *= 2;
        }

        int *new_matrix = malloc(sizeof(int) * matrix->capacity * matrix->capacity);
        for (size_t i = 0; i < matrix->capacity * matrix->capacity; i++) {
            new_matrix[i] = 0;
        }

        for (size_t y = 0; y < matrix->count; y++) {
            for (size_t x = 0; x < matrix->count; x++) {
                new_matrix[y*matrix->capacity + x] = matrix->matrix[y*capacity + x];
            }
        }

        free(matrix->matrix);
        matrix->matrix = new_matrix;
    }

    if (from + 1 > matrix->count) matrix->count = from + 1;
    if (to + 1 > matrix->count) matrix->count = to + 1;
    matrix->matrix[from*matrix->capacity + to] = distance;
}

int matrix_get(const struct Matrix *matrix, size_t from, size_t to) {
    return matrix->matrix[from*matrix->capacity + to];
}

void debug_matrix(const struct Matrix *matrix) {
    printf("\n");
    for (size_t y = 0; y < matrix->count; y++) {
        for (size_t x = 0; x < matrix->count; x++) {
            printf("%d\t", matrix_get(matrix, y, x));
        }

        printf("\n");
    }
    printf("\n");
}

int maximum_happiness(struct Matrix *matrix) {
    int max_happiness = INT_MIN;

    size_t *seats = malloc(sizeof(size_t) * matrix->count);
    for (size_t i = 0; i < matrix->count; i++) {
        seats[i] = 0;
    }

    for (;;) {
        bool valid = true;
        for (size_t i = 0; i < matrix->count; i++) {
            for (size_t j = i + 1; j < matrix->count; j++) {
                if (seats[i] == seats[j]) {
                    valid = false;
                }
            }
        }


        if (valid) {
            int happiness = 0;

            for (size_t i = 0; i < matrix->count; i++) {
                happiness += matrix_get(matrix, seats[i], seats[(matrix->count + i - 1) % matrix->count]);
                happiness += matrix_get(matrix, seats[i], seats[(i + 1) % matrix->count]);
            }
            
            if (happiness > max_happiness) {
                max_happiness = happiness;
            }
        }

        bool quit = true;
        for (size_t i = 1; i < matrix->count; i++) {
            if (seats[i] >= matrix->count - 1) {
                seats[i] = 0;
            } else {
                seats[i] += 1;
                quit = false;
                break;
            }
        }

        if (quit) break;
    }

    free(seats);

    return max_happiness;
}

int main(void) {
    struct Matrix matrix = {0};
    struct List *root = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') break;

        char subject[256], object[256];
        int happiness;

        if (sscanf(buffer, "%[A-Za-z] would gain %d happiness units by sitting next to %[A-Za-z].", subject, &happiness, object) == 3) {
        } else if (sscanf(buffer, "%[A-Za-z] would lose %d happiness units by sitting next to %[A-Za-z].", subject, &happiness, object) == 3) {
            happiness = -happiness;
        } else {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        size_t subject_index = list_add(&root, new_list(NULL, subject));
        size_t object_index = list_add(&root, new_list(NULL, object));
        matrix_set(&matrix, subject_index, object_index, happiness);
    }

    printf("Part 1: %d\n", maximum_happiness(&matrix));
    matrix_set(&matrix, matrix.count, matrix.count, 0);
    printf("Part 2: %d\n", maximum_happiness(&matrix));

    return 0;
}
