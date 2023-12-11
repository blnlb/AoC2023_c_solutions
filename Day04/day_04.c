#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int part2(FILE *fp) {
    size_t len = 200;
    char *buff = malloc(len);

    int copies[202] = {0};
    int* win = calloc(10, sizeof(int));
    int* curr = calloc(25, sizeof(int));
    int total = 0, thisCard = 0;
    char* p = NULL;
    int winlen = 0, currlen = 0, cardNumber = 0;

    while (getline(&buff, &len, fp) != EOF) {
        ++copies[cardNumber];
        thisCard = 0;
        winlen = 0;
        currlen = 0;
        p = strstr(buff, ":") + 1;
        while (strncmp(p + 1, "|", 1)) {
            win[winlen++] = atoi(p);
            p += 3;
        }
        p += 2;
        while (strncmp(p, "\n", 1)) {
            curr[currlen++] = atoi(p);
            p += 3;
        }
        for (int i = 0; i < currlen; ++i) {
            for (int j = 0; j < winlen; ++j) {
                if (curr[i] == win[j]) {
                    ++thisCard;
                    break;
                }
            }
        }

        for (int i = 1; i <= thisCard; ++i) {
            copies[cardNumber + i] += copies[cardNumber];
        }
        ++cardNumber;
    }
    for (int i = 0; i < cardNumber; ++i) {
        total += copies[i];
    }

    free(win);
    free(curr);
    free(buff);
    return total;
}

int part1(FILE *fp) {
    size_t len = 200;
    char *buff = malloc(len);

    int* win = calloc(10, sizeof(int));
    int* curr = calloc(25, sizeof(int));
    int total = 0, thisCard = 0;
    char* p = NULL;
    int winlen = 0, currlen = 0;

    while (getline(&buff, &len, fp) != EOF) {
        thisCard = 0;
        winlen = 0;
        currlen = 0;
        p = strstr(buff, ":") + 1;
        while (strncmp(p + 1, "|", 1)) {
            win[winlen++] = atoi(p);
            p += 3;
        }
        p += 2;
        while (strncmp(p, "\n", 1)) {
            curr[currlen++] = atoi(p);
            p += 3;
        }
        for (int i = 0; i < currlen; ++i) {
            for (int j = 0; j < winlen; ++j) {
                if (curr[i] == win[j]) {
                    thisCard = (thisCard == 0) ? 1 : thisCard * 2;
                    break;
                }
            }
        }

        total += thisCard;
    }

    free(win);
    free(curr);
    free(buff);
    return total;
}

int main(int argc, char** argv) {
    //replace filename with your input
    char *filename = "day_04.txt";
    FILE *fp = fopen(filename, "r");
        printf("Part 1: %d\n", part1(fp));
    
    freopen(filename, "r", fp);
        printf("Part 2: %d\n", part2(fp));
    
    fclose(fp);
    return 0;
}