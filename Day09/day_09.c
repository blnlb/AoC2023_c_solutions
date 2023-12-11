#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int numDigits(int n) {
    int res = 0;
    if (n < 0) { n *= -1; ++res; }
    do {
        ++res;
        n /= 10;
    } while (n > 0);
    return res;
}

int allZeros(int** pyr, int j, int len) {
    for (int i = 0; i < len; ++i) {
        if (pyr[j][i] != 0) return 0;
    }
    return 1;
}

int future(int** pyr, int steps, int len) {
    if (allZeros(pyr, steps, len)) return 0;
    for (int i = 0; i < len - 1; ++i) {
        pyr[steps + 1][i] = pyr[steps][i+1] - pyr[steps][i];
    }
    return pyr[steps][len-1] + future(pyr, steps + 1, len-1);
}

int past(int **pyr, int steps, int len) {
    if (allZeros(pyr, steps, len)) return 0;
    for (int i = 0; i < len - 1; ++i) {
        pyr[steps + 1][i] = pyr[steps][i+1] - pyr[steps][i];
    }
    return pyr[steps][0] - past(pyr, steps + 1, len-1);
}

int main(int argc, char** argv) {
    FILE *fp = fopen("day_09.txt", "r");
    size_t len = 200;
    char *buff = malloc(len);

    int **pyr = calloc(len, sizeof(int*));
    int *p = NULL;
    for (int i = 0; i < len; ++i) {
        pyr[i] = calloc(len, sizeof(int));
    } 

    int ind, ct, n;
    int part1 = 0, part2 = 0;
    while(getline(&buff, &len, fp) != EOF) {
        ind = 0; ct = 0;
        // Get all of the numbers, put them into pyr[0]
        while (*(buff + ind - 1) != '\n') {
            n = atoi(buff + ind);
            pyr[0][ct++] = n;
            ind += numDigits(n) + 1;
        }
        // This is just to avoid having to send array lengths into every function
        for (int i = 0; i < ct; ++i) {
            p = realloc(pyr[i], (ct - i)*sizeof(int));
            if (p == NULL) exit(-1);
            pyr[i] = p;
            p = NULL;
        }
        // Now do the differences.
        part1 += future(pyr, 0, ct);
        part2 += past(pyr, 0, ct);
    }
    printf("Part 1: %d\nPart 2: %d\n", part1, part2);

    return 0;
}