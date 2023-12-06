#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t atouint(char *s) {
    int ind = 0;
    size_t res = 0;
    while (isdigit((s+ind)[0])) {
        res = res*10 + ((s+ind)[0] - '0');
        ++ind;
    }
    return res;
}

int numDigits(size_t n) {
    int res = 0;
    do {
        ++res;
        n /= 10;
    } while (n > 0);
    return res;
}

int part1(char *file) {
    FILE *fp = fopen(file, "r");
    size_t len = 64;
    char *buff = malloc(len);

    // Arrays to store values
    size_t *times = calloc(5, sizeof(size_t));
    size_t *distances = calloc(5, sizeof(size_t));
    int numRaces = 0, ind = 5, n;

    // Going to store information and then process
    getline(&buff, &len, fp);
    while (strncmp(buff + ind, "\n", 1)) {
        while (!strncmp(buff + ind, " ", 1)) {
            ++ind;
        }
        times[numRaces] = atouint(buff+ind);
        ind += numDigits(times[numRaces++]);
    }

    getline(&buff, &len, fp);
    numRaces = 0;
    ind = 10;
    while (strncmp(buff + ind, "\n", 1)) {
        while (!strncmp(buff + ind, " ", 1)) {
            ++ind;
        }
        distances[numRaces] = atouint(buff+ind);
        ind += numDigits(distances[numRaces++]);
    }

    int result = 1;
    for (int i = 0; i < numRaces; ++i) {
        for (int j = 0; j < times[i]; ++j) {
            if ((times[i] - j)*j > distances[i]) {
                result *= (times[i] - j - j + 1);
                break;
            }
        }
    }
    return result;
}

size_t atouint_ignorespaces(char *s) {
    int ind = 0;
    size_t res = 0;
    while (strncmp(s + ind, "\n", 1)) {
        if ((s+ind)[0] == ' ') {
            ++ind;
            continue;
        }
        res = res*10 + ((s+ind)[0] - '0');
        ++ind;
    }
    return res;
}

int part2(char *file) {
    FILE *fp = fopen(file, "r");
    size_t len = 64;
    char *buff = malloc(len);

    // Arrays to store values
    size_t time;
    size_t distance;
    int n;

    // Going to store information and then process
    getline(&buff, &len, fp);
    time = atouint_ignorespaces(buff+5);

    getline(&buff, &len, fp);
    distance = atouint_ignorespaces(buff+10);

    int result = 1;
    for (int j = 0; j <= time; ++j) {
        if ((time - j)*j > distance) {
            result = time - j - j + 1;
            break;
        }
    }
    return result;
}

int main(int argc, char **argv) {
    char *file = "test.txt";
    printf("Part 1: %d\n", part1(file));
    printf("Part 2: %d\n", part2(file));
    return 0;
}