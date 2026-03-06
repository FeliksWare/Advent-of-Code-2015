#include <stdio.h>

long long next_code(long long code) {
    return (code * 252533) % 33554393;
}

long long pairing_function(long long row, long long column) {
    return (row + column - 2) * (row + column - 1)  / 2 + column;
}

int main(void) {
    long long row, column;
    scanf("To continue, please consult the code grid in the manual.  Enter the code at row %lld, column %lld.\n", &row, &column);

    long long code = 20151125;
    long long position = pairing_function(row, column);

    for (long long i = 1; i < position; i++) {
        code = next_code(code);
    }

    printf("Part 1: %lld\n", code);

    return 0;
}
