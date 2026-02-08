#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct Array {
    int *array;
    size_t count;
    size_t capacity;
};

void array_reserve(struct Array *array, size_t minimum) {
    if (array->capacity >= minimum) {
        return;
    }

    if (array->capacity == 0) {
        array->capacity = 1024;
    }

    while (array->capacity < minimum) {
        array->capacity *= 2;
    }

    array->array = realloc(array->array, sizeof(int) * array->capacity);
}

void array_push(struct Array *array, int value) {
    array_reserve(array, array->count + 1);
    array->array[array->count++] = value;
}

void free_array(struct Array *array) {
    if (array->array != NULL) {
        free(array->array);
    }
    *array = (struct Array){0};
}

void look_and_say(struct Array *array) {
    struct Array new_array = {0};
    int value = array->array[0];
    int count = 1;

    for (size_t i = 1; i < array->count; i++) {
        if (array->array[i] == value) {
            count += 1;
        } else {
            array_push(&new_array, count);
            array_push(&new_array, value);
            value = array->array[i];
            count = 1;
        }
    }

    array_push(&new_array, count);
    array_push(&new_array, value);

    free_array(array);
    *array = new_array;
}

int main(void) {
    struct Array array = {0};

    for (int c = fgetc(stdin); c != EOF; c = fgetc(stdin)) {
        if (!isdigit(c)) continue;
        array_push(&array, c - '0');
    }

    for (size_t i = 0; i < 40; i++) {
        look_and_say(&array);
    }

    printf("%zu\n", array.count);

    for (size_t i = 0; i < 10; i++) {
        look_and_say(&array);
    }

    printf("%zu\n", array.count);

    return 0;
}
