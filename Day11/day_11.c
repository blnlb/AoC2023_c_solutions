#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEBUG 0

#if DEBUG
    void printCoor(int** map, int len) {
        for (int i = 0; i < len; ++i) {
            printf("Coor %d: (%d, %d)\n", i, map[i][0], map[i][1]);
        } 
    }
#endif

long emptyLinesPassedThrough(int* filledCols, int* filledRows, int* a, int* b, long expansionVal) {
    long total = 0;
    int gr = a[0] > b[0] ? a[0] : b[0];
    int ls = a[0] > b[0] ? b[0] : a[0];
    for (int i = ls + 1; i < gr; ++i) {
        if (!filledCols[i]) total += expansionVal;
    }
    gr = a[1] > b[1] ? a[1] : b[1];
    ls = a[1] > b[1] ? b[1] : a[1];
    for (int i = ls + 1; i < gr; ++i) {
        if (!filledRows[i]) total += expansionVal;
    }
    return total;
}

int dist(int* a, int* b) {
    int xdist = a[0] > b[0] ? a[0] - b[0] : b[0] - a[0];
    int ydist = a[1] > b[1] ? a[1] - b[1] : b[1] - a[1];
    return xdist + ydist;
}

int main(int argc, char** argv) {
    FILE *fp = fopen("day_11.txt", "r");
    size_t len = 200;
    size_t len_dummy = 200;
    char *buff = malloc(len);


    // Alternatively just store positions
    // Galaxy n at (x,y) is (galPos[n][0], galPos[n][1])
    int** galPos = calloc(len*5, sizeof(int*));
    for (int i = 0; i < len*5; ++i) {
        galPos[i] = calloc(2, sizeof(int));
    }
    int galCount = 0;
    // AND alternatively just store column numbers that correspond
    // with empty columns. Same with rows.
    int *filledCols = calloc(len, sizeof(int));
    int *filledRows = calloc(len, sizeof(int));
    int numEmptyCols = 0, numEmptyRows = 0;

    int col, row = 0;
    while(getline(&buff, &len_dummy, fp) != EOF) {
        col = 0;
        while (strcmp(buff + col, "\n")) {
            if ((buff + col)[0] == '#') {
                if (!filledCols[col]) filledCols[col] = 1;
                if (!filledRows[row]) filledRows[row] = 1;
                galPos[galCount][0] = col;
                galPos[galCount++][1] = row;
            }
            ++col;
        }
        ++row;
    }
    long total = 0;
    int d;
    for (int i = 0; i < galCount - 1; ++i) {
        for (int j = i + 1; j < galCount; ++j) {
            // Pass expansionVal=1 for Part 1 and expansionVal=1000000 for Part 2
            total += (long)(dist(galPos[i], galPos[j])) 
                        + emptyLinesPassedThrough(filledCols, filledRows, galPos[i], galPos[j], 999999);
        }
    }

    #if DEBUG
        printCoor(galPos, galCount);
    #endif
    printf("Sum of Distances = %ld\n", total);

    free(buff);
    fclose(fp);
    for (int i = 0; i < len*5; ++i) {
        free(galPos[i]);
    }
    free(galPos);
    free(filledRows);
    free(filledCols);
    return 0;
}