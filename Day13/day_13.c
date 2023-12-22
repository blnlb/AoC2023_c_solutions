#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1

typedef struct grid {
    int** map;
    int numRows;
    int numCols;
} grid;

grid* createGrid(int** map, int rows, int cols) {
    grid* result = calloc(1, sizeof(grid));
    result->numRows = rows;
    result->numCols = cols;
    result->map = calloc(rows, sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        result->map[i] = calloc(cols, sizeof(int));
        for (int j = 0; j < cols; ++j){
            result->map[i][j] = map[i][j];
        }
    }
    

    return result;
}

#if DEBUG
void printMap(int** map, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d", map[i][j]);
        }
        printf("\n");
    }
}
#endif


int checkForMirrors(grid*);

int main(int argc, char** argv) {
    FILE *fp = fopen("day_13.txt", "r");
    size_t len = 20;
    size_t dum = 200;
    char *buff = malloc(len);

    grid** allMaps = calloc(150, sizeof(grid*));

    int **map = calloc(len, sizeof(int*));
    for (int j = 0; j < len; ++j) {
        map[j] = calloc(len, sizeof(int));
    }

    int ct = 0, col, row = 0;
    int retval = 0;
    while ((retval = getline(&buff, &dum, fp)) != -2) {
        if (retval == -1) {
            allMaps[ct] = createGrid(map, row, col);
            #if DEBUG
                printMap(allMaps[ct]->map, allMaps[ct]->numRows, allMaps[ct]->numCols);
                printf("\n");
            #endif
            ++ct;
            break;
        }
        if (!strcmp(buff, "\n")) {
            allMaps[ct] = createGrid(map, row, col);
            #if DEBUG
                printMap(allMaps[ct]->map, allMaps[ct]->numRows, allMaps[ct]->numCols);
                printf("\n");
            #endif
            ++ct;
            row = 0;
            for (int j = 0; j < len; ++j) {
                map[j] = calloc(len, sizeof(int));
            }
            continue;
        }

        col = 0;
        while (strcmp(buff + col, "\n")) {
            if ((buff + col)[0] == '#') { 
                ++(map[row][col]);
            }
            ++col;
        }
        ++row;
    }

    int total = 0;
    for (int i = 0; i < ct; ++i) {
        total += checkForMirrors(allMaps[i]);
        #if DEBUG
            printf("Grid %d: %d\n", i + 1, total);
        #endif
    }
    printf("Part 1: %d\n", total);

    return 0;
}

int checkForMirrors(grid* map) {
    int numDifferences;
    int rows = map->numRows;
    int cols = map->numCols;
    //Column mirror check
    for (int j = 1; j < cols; ++j) {
        numDifferences = 0;
        for (int i = 0; i < rows; ++i) {
            for (int off = 0; j-off > 0 && j+off < cols; ++off) {
                if (map->map[i][j-off - 1] != map->map[i][j+off]) {
                    ++numDifferences;
                    if (numDifferences > 1) break;
                }
            }
            if (numDifferences > 1) break;
        }
        if (numDifferences == 1) return j;
    }
    // If column did not return anything,
    // Time to parse the rows
    for (int i = 1; i < rows; ++i) {
        numDifferences = 0;
        for (int j = 0; j < cols; ++j) {
            for (int off = 0; i - off > 0 && i + off < rows; ++off) {
                if (map->map[i-off - 1][j] != map->map[i+off][j]) 
                {   ++numDifferences;
                    if (numDifferences > 1) break;
                }
            }
            if (numDifferences > 1) break;
        }
        if (numDifferences == 1) return i*100;
    }

    return -1;
}