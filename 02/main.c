#include <stdio.h>

int main(void) {
    int paper = 0;
    int ribbon = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == '\n') continue;

        int l, w, h;
        if (sscanf(buffer, "%dx%dx%d", &l, &w, &h) != 3) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n", buffer);
            return 1;
        }

        paper += 2*(l*w + w*h + h*l);
        ribbon += l*w*h;
        if (l > w && l > h) {
            paper += w*h;
            ribbon += 2*(w+h);
        } else if (w > h) {
            paper += h*l;
            ribbon += 2*(h+l);
        } else {
            paper += l*w;
            ribbon += 2*(l+w);
        }
    }

    printf("Paper: %d\n", paper);
    printf("Ribbon: %d\n", ribbon);

    return 0;
}
