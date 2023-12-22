#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MIN(int A,int B) {return A < B ? A : B;}

typedef struct tile {
    int heatValue;
    int traveled;
    int lowestYet;
    char c;
} tile;

tile *createTile(int n) {
    tile* res = calloc(1, sizeof(tile));
    res->heatValue = n;
    res->traveled = 0;
    res->lowestYet = 2147483600;
    res->c = '.';
    return res;
}

int recur(tile*** map, int len, int row, int col, int steps, int, char);

void printMap(tile***map, int len) {
    for (int i = 0; i < len; ++i) {
        for (int j=0; j < len; ++j) {
            printf("%c", map[i][j]->c);
        }
        printf("\n");
    }
    printf("\n");
}

int main (int argc, char** argv) {
    FILE *fp = fopen("test.txt", "r");
    size_t len = 142;
    size_t dum = 102;
    char *buff = malloc(len);

    tile*** map = calloc(len, sizeof(tile**));
    for (int i = 0; i < len; ++i) {
        map[i] = calloc(len, sizeof(tile*));
        for (int j = 0; j < len; ++j) {
            map[i][j] = calloc(1, sizeof(tile));
        }
    } 
    int row = 0 , col;
    while (getline(&buff, &dum, fp) != EOF) {
        col = 0;
        while (buff[col] != '\n') {
            map[row][col] = createTile(buff[col] - '0');
            ++col;
        }
        if (row == 0) len = col;
        ++row;
    }

    int part1 = recur(map, len, 0, 0, 0, 0, 0);

    printf("%d\n", part1);

    return 0;
}

int recur(tile*** map, int len, int row, int col, int steps, int consecutiveSteps, char dir) {
    if (row == len -1 && col == len - 1) {
        printMap(map, len);
        return steps;
    }
    switch (dir) {
        case 'u':
            map[row][col]->c = '^';
            break;
        case 'd':
            map[row][col]->c = 'v';
            break;
        case 'l':
            map[row][col]->c = '<';
            break;
        case 'r':
            map[row][col]->c = '>';
            break;
    }
    map[row][col]->traveled = 1;
    int doNotGoForward = 0;
    int newLow;
    if (consecutiveSteps == 3) {
        doNotGoForward = 1;
        //consecutiveSteps = 0;
    }
    if (row > 0 && !map[row-1][col]->traveled //&& map[row-1][col]->lowestYet >= map[row][col]->lowestYet 
    && (dir != 'u' || !doNotGoForward) && dir != 'd') {
        if (dir == 'u') ++consecutiveSteps;
        else consecutiveSteps = 1;
        map[row][col]->lowestYet = MIN(map[row][col]->lowestYet, recur(map, len, row-1, col, steps + map[row-1][col]->heatValue, consecutiveSteps, 'u'));
    }
    if (row < len - 1 && !map[row+1][col]->traveled //&& map[row+1][col]->lowestYet >= map[row][col]->lowestYet 
    && (dir != 'd' || !doNotGoForward) && dir != 'u') {
        if (dir == 'd') ++consecutiveSteps;
        else consecutiveSteps = 1;
        map[row][col]->lowestYet = MIN(map[row][col]->lowestYet, recur(map, len, row+1, col, steps + map[row+1][col]->heatValue, consecutiveSteps, 'd'));
    }
    if (row == 0 && col == 0) {
        ;
    }
    if (col > 0 && !map[row][col-1]->traveled //&& map[row][col-1]->lowestYet >= map[row][col]->lowestYet 
    && (dir != 'l' || !doNotGoForward) && dir != 'r') {
        if (dir == 'l') ++consecutiveSteps;
        else consecutiveSteps = 1;
        map[row][col]->lowestYet = MIN(map[row][col]->lowestYet, recur(map, len, row, col-1, steps + map[row][col-1]->heatValue, consecutiveSteps, 'l'));
    }
    if (col < len - 1 && !map[row][col + 1]->traveled //&& map[row][col+1]->lowestYet >= map[row][col]->lowestYet 
    && (dir != 'r' || !doNotGoForward) && dir != 'l') {
        if (map[row][col]->lowestYet < map[row][col+1]->lowestYet) {
            
        }
        if (dir == 'r') ++consecutiveSteps;
        else consecutiveSteps = 1;
        map[row][col]->lowestYet = MIN(map[row][col]->lowestYet, recur(map, len, row, col+1, steps + map[row][col+1]->heatValue, consecutiveSteps, 'r'));
    }
    map[row][col]->traveled = 0;
    map[row][col]->c = '.';
    return map[row][col]->lowestYet;
    
}
