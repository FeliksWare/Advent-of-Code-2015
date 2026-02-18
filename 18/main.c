#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH (100)
#define HEIGHT (100)

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

struct Grid {
    bool *lights;
    bool *lights_next;
    int width;
    int height;
};

struct Grid create_grid(int width, int height) {
    struct Grid grid;
    grid.lights = calloc(width * height, sizeof(bool));
    grid.lights_next = calloc(width * height, sizeof(bool));
    grid.width = width;
    grid.height = height;
    return grid;
}

void debug_lighs(struct Grid grid) {
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.width; x++) {
            if (grid.lights[grid.width*y + x]) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int get_neighbours(struct Grid grid, int x, int y) {
    static const int offset[8][2] = {
        {-1, -1}, { 0, -1}, {+1, -1},
        {-1,  0},           {+1,  0},
        {-1, +1}, { 0, +1}, {+1, +1},
    };

    int neighbours = 0;

    for (size_t i = 0; i < ARRAY_LENGTH(offset); i++) {
        int ox = x + offset[i][0];
        int oy = y + offset[i][1];

        if (ox < 0 || ox >= grid.width || oy < 0 || oy >= grid.height || !grid.lights[grid.width*oy + ox]) {
            continue;
        }

        neighbours += 1;
    }

    return neighbours;
}

void next_lighs(struct Grid grid) {
    for (int y = 0; y < grid.height; y++) {
        for (int x = 0; x < grid.height; x++) {
            int neighbours = get_neighbours(grid, x, y);

            if (grid.lights[y*grid.width + x]) {
                grid.lights_next[y*grid.width + x] = neighbours == 2 || neighbours == 3;
            } else {
                grid.lights_next[y*grid.width + x] = neighbours == 3;
            }
        }
    }

    for (int i = 0; i < grid.width*grid.height; i++) {
        grid.lights[i] = grid.lights_next[i];
    }
}

int main(void) {
    struct Grid grid = create_grid(WIDTH, HEIGHT);
    struct Grid grid_corners = create_grid(WIDTH, HEIGHT);

    for (int c = fgetc(stdin), i = 0; c != EOF && i < WIDTH*HEIGHT; c = fgetc(stdin)) {
        switch (c) {
            case '.': i++; break;
            case '#': grid.lights[i++] = true; break;
            default: break;
        }
    }

    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        grid_corners.lights[i] = grid.lights[i];
    } 

    for (int i = 0; i < 100; i++) {
        grid_corners.lights[WIDTH*0 + 0] = 1;
        grid_corners.lights[WIDTH*(HEIGHT-1) + 0] = 1;
        grid_corners.lights[WIDTH*(HEIGHT-1) + (WIDTH-1)] = 1;
        grid_corners.lights[WIDTH*0 + (WIDTH-1)] = 1;

        next_lighs(grid);
        next_lighs(grid_corners);
    }

    grid_corners.lights[WIDTH*0 + 0] = 1;
    grid_corners.lights[WIDTH*(HEIGHT-1) + 0] = 1;
    grid_corners.lights[WIDTH*(HEIGHT-1) + (WIDTH-1)] = 1;
    grid_corners.lights[WIDTH*0 + (WIDTH-1)] = 1;

    int count1 = 0;
    int count2 = 0;

    for (int i = 0; i < WIDTH*HEIGHT; i++) {
        if (grid.lights[i]) count1 += 1;
        if (grid_corners.lights[i]) count2 += 1;
    }

    printf("Part 1: %d\n", count1);
    printf("Part 2: %d\n", count2);

    return 0;
}
