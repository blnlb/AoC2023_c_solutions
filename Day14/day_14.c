#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

void tiltNorth(int** map, int len);
void tiltSouth(int** map, int len);
void tiltEast(int** map, int len);
void tiltWest(int** map, int len);
void cycle(int** map, int, long);
int calculateLoad(int** map, int);

enum floorType {
    Empty,
    Rock,
    Wall
};

#if DEBUG
    void printGrid(int** map, int len) {
        char c;
        for (int i = 0; i < len; ++i) {
            for (int j = 0; j < len; ++j) {
                switch (map[i][j]) {
                    case Wall:
                        c = '#';
                        break;
                    case Rock:
                        c = '0';
                        break;
                    case Empty:
                    default:
                        c = '.';
                }
                printf("%c", c);
            }
            printf("\n");
        }
        printf("\n");
    }
#endif

int main (int argc, char** argv) {
    FILE *fp = fopen("input.txt", "r");
    size_t len = 100;
    size_t dum = 102;
    char *buff = malloc(len);

    int **map = calloc(len, sizeof(int*));
    for (int i = 0; i < len; ++i) {
        map[i] = calloc(len,sizeof(int));
    }

    int col, row = 0;
    char c;
    while (getline(&buff, &dum, fp) != EOF) {
        col = 0;
        while ((c = (buff + col)[0]) != '\n')  {
            if (c == 'O') map[row][col] = Rock;
            if (c == '#') map[row][col] = Wall;
            ++col;
        }
        if (row == 0) {
            len = col;
        }
        ++row;
    }
//     #if DEBUG
//         printGrid(map, len);
//     #endif
//     // tiltNorth(map, len);
//    #if DEBUG
//         printGrid(map, len);
//     #endif
    printf("Part 1: %d\n", calculateLoad(map, len));
    cycle(map, len, 1000000000);
    printf("Part 2: %d\n", calculateLoad(map, len));
    return 0;
}

void cycle(int **map, int len, long m) {
    int numCalcs = 0;
    int *allCalculations = calloc(10000, sizeof(int));
    int curr;
    long n = m;
    int dist = 0;
    while (n-- > 0) {
        tiltNorth(map, len);
        tiltWest(map,len);
        tiltSouth(map,len);
        tiltEast(map,len);
        #if DEBUG
            printGrid(map, len);
        #endif
        curr = calculateLoad(map, len);
        for (int i = 0; i < numCalcs/2; ++i) {
            if (allCalculations[i] == curr) {
                dist = numCalcs - i + 1;
            }   
        }
        if (dist) break;
        allCalculations[numCalcs++] = curr;
    }
    long remaining = n % dist;
    while (remaining-- > 0) {
        tiltNorth(map, len);
        tiltWest(map,len);
        tiltSouth(map,len);
        tiltEast(map,len);
    }
}

void tiltNorth(int** map, int len) {
    // start on row 1, since row 0 contributes nothing
    int curr, temp;
    for (int i = 1; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            curr = i;
            while (curr > 0 && map[curr][j] == Rock && map[curr-1][j] == Empty) {
                temp = map[curr][j];
                map[curr][j] = map[curr-1][j];
                map[curr-1][j] = temp;
                --curr;
            }
        }
    }
}

void tiltSouth(int** map, int len) {
    // start on row 1, since row 0 contributes nothing
    int curr, temp;
    for (int i = len - 2; i >= 0; --i) {
        for (int j = 0; j < len; ++j) {
            curr = i;
            while (curr < len - 1 && map[curr][j] == Rock && map[curr+1][j] == Empty) {
                temp = map[curr][j];
                map[curr][j] = map[curr+1][j];
                map[curr+1][j] = temp;
                ++curr;
            }
        }
    }
}

void tiltWest(int** map, int len) {
    // start on row 1, since row 0 contributes nothing
    int curr, temp;
    // i is column index now
    // j is row index
    for (int i = 1; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            curr = i;
            while (curr > 0 && map[j][curr] == Rock && map[j][curr - 1] == Empty) {
                temp = map[j][curr];
                map[j][curr] = map[j][curr-1];
                map[j][curr-1] = temp;
                --curr;
            }
        }
    }
}


void tiltEast(int** map, int len) {
    int curr, temp;
    for (int i = len - 2; i >= 0; --i) {
        for (int j = 0; j < len; ++j) {
            curr = i;
            while (curr < len - 1 && map[j][curr] == Rock && map[j][curr+1] == Empty) {
                temp = map[j][curr];
                map[j][curr] = map[j][curr+1];
                map[j][curr+1] = temp;
                ++curr;
            }
        }
    }
}



int calculateLoad(int** map, int len) {
    int total = 0;
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            if (map[i][j] == Rock) 
                total += (len-i);
        }
    }
    return total;
}