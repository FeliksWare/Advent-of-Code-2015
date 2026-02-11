#include <stdio.h>
#include <stdbool.h>

void accounting(bool is_obj, long long *sum1, long long *sum2) {
    long long sum1_temp = 0;
    long long sum2_temp = 0;
    long long sum = 0;
    int state = 0;

    for (;;) {
        long long num = 0;

        while (scanf("%lld", &num) != 1) {
            switch (fgetc(stdin)) {
                case '{': accounting(true, &sum1_temp, &sum2_temp); break;
                case '[': accounting(false, &sum1_temp, &sum2_temp); break;
                case '}': case ']': case EOF: goto eof;
                case 'r': if (state != 3) {
                    if (state == 0) state = 1; else state = 0;
                } break;
                case 'e': if (state != 3) {
                    if (state == 1) state = 2; else state = 0;
                } break;
                case 'd': if (state != 3) {
                    if (state == 2) state = 3; else state = 0;
                } break;
                default: if (state != 3) {
                    state = 0;
                } break;
            }
        }

        sum += num;

        while (scanf("%lld", &num) == 1) {
            switch (fgetc(stdin)) {
                case '{': accounting(true, &sum1_temp, &sum2_temp); break;
                case '[': accounting(false, &sum1_temp, &sum2_temp); break;
                case '}': case ']': case EOF: goto eof;
                case 'r': if (state != 3) {
                    if (state == 0) state = 1; else state = 0;
                } break;
                case 'e': if (state != 3) {
                    if (state == 1) state = 2; else state = 0;
                } break;
                case 'd': if (state != 3) {
                    if (state == 2) state = 3; else state = 0;
                } break;
                default: if (state != 3) {
                    state = 0;
                } break;
            }
        }

        continue;
        eof: break;
    }

    *sum1 += sum + sum1_temp;
    if (!is_obj || state != 3) {
        *sum2 += sum + sum2_temp;
    }
}

int main (void) {
    long long sum1 = 0;
    long long sum2 = 0;

    accounting(false, &sum1, &sum2);


    printf("Part 1: %lld\n", sum1);
    printf("Part 1: %lld\n", sum2);

    return 0;
}
