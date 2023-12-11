#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define DEBUG 0
int32_t MAX(int32_t a, int32_t b) { return((a) > (b) ? a : b); }
int32_t MIN(int32_t a, int32_t b) { return((a) > (b) ? b : a); }

enum Directions {
    left = 1,
    up = 2,
    right = 4,
    down = 8
};

struct tile {
    #if DEBUG
        char letter;
    #endif
    int mainLoop;
    int isStart;
    int* directions;
    int lowestDistance;
    
    int intersections;
};

void destroyTile(struct tile* t) {
    free(t->directions);
    free(t);
}

struct tile* createTile(char c) {
    struct tile* result = calloc(1, sizeof(struct tile));
    #if DEBUG
    result->letter = c;
    #endif
    result->directions = calloc(2, sizeof(int)); // 0'd out.
    result->isStart = 0;
    result->lowestDistance = 2147483647;
    result->mainLoop = 0;
    result->intersections = 0;
    if (c == '.') {
        return result;
    }
    switch (c) {
        case '|':
            result->directions[0] = up;
            result->directions[1] = down;
            break;
        case '-':
            result->directions[0] = left;
            result->directions[1] = right;
            break;
        case 'L':
            result->directions[0] = up;
            result->directions[1] = right;
            break;
        case 'J':
            result->directions[0] = up;
            result->directions[1] = left;
            break;
        case '7':
            result->directions[0] = left;
            result->directions[1] = down;
            break;
        case 'F':
            result->directions[0] = right;
            result->directions[1] = down;
            break;
        case 'S':
        default:
            result->mainLoop = 1;
            result->lowestDistance = 0;
            result->isStart = 1;
    }
    return result;
}

// I don't care about bounds checking here, as the input file has S squarely inside the bounds
void getStartDir(struct tile **map, int ind, int len) {
    int num = 0;
    if (ind > len && (map[ind - len]->directions[0] == down || map[ind - len]->directions[1] == down))
        map[ind]->directions[num++] = up;
    if (map[ind + len]->directions[0] == up || map[ind + len]->directions[1] == up)
        map[ind]->directions[num++] = down;
    if (map[ind - 1]->directions[0] == right || map[ind - 1]->directions[1] == right)
        map[ind]->directions[num++] = left;
    if (map[ind + 1]->directions[0] == left || map[ind + 1]->directions[1] == left)
        map[ind]->directions[num++] = right;
}

#if DEBUG
void printMap(struct tile** map, int len, int dim2) {
    for (int i = 0; i < dim2; ++i) {
        for (int j = 0; j < len; ++j) {
            printf("%c", map[i*len + j]->letter);
        }
        printf("\n");
    }
}
#endif

//uggghhhhhh
int bfs(struct tile** map, int ind, int oldind, int len, int steps) {
    int newDir;
    map[ind]->mainLoop = 1;
    map[ind]->lowestDistance = MIN(map[ind]->lowestDistance, steps);
    if (oldind != ind - 1 && 
        (map[ind]->directions[0] == left || map[ind]->directions[1] == left))
            newDir = -1;
    else if (oldind != ind + 1 &&
        (map[ind]->directions[0] == right || map[ind]->directions[1] == right))
            newDir = 1;
    else if (oldind != ind - len &&
        (map[ind]->directions[0] == up || map[ind]->directions[1] == up))
            newDir = -len;
    else
        newDir = len;

    if (map[ind]->lowestDistance >= map[ind + newDir]->lowestDistance) {
        return map[ind]->lowestDistance;
    }
    
    return bfs(map, ind + newDir, ind, len, steps + 1);
}

//EVEN WORSE NOW
int part2(struct tile** map, int len) {
    struct tile *curr = NULL;
    int total = 0;
    int dirs;
    int isUp, isDown;
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < len; ++j) {
            isUp = 0; isDown = 0;
            if (map[i*len + j]->mainLoop) continue;
            for (int rt = j + 1; rt < len; ++rt) {
                curr = map[i*len + rt];
                if (curr->mainLoop) {
                    dirs = (curr->directions[0] | curr->directions[1]); 
                    if (dirs == (up|down))
                        ++(map[i*len + j]->intersections);
                    else if (dirs & up) {
                        if (isDown) {
                            ++(map[i*len + j]->intersections);
                        } else {
                            isUp = 1;
                        }
                    }
                    else if (dirs & down) {
                        if (isUp) {
                            ++(map[i*len + j]->intersections);
                        } else {
                            isDown = 1;
                        }
                    }

                }  
            }
            if ((map[i*len + j]->intersections) % 2) {
                ++total; continue;
            }
        }
    }
    return total;
}


int main(int argc, char** argv) {
    FILE *fp = fopen("test.txt", "r");
    size_t len = 150;
    size_t len_dummy;
    char *buff = malloc(len);

    // Tiles at (x,y) are accessed by map[y*len + x]
    // Only allocate the FIRST row with a liberal amount of space.
    // We will realloc once we parse the first line.
    struct tile **map = calloc(len, sizeof(struct tile*));
    for (int i = 0; i < len; ++i) {
        map[i] = calloc(1, sizeof(struct tile));
    }

    int row = 0, col;
    int startind = 0;
    while (getline(&buff, &len_dummy, fp) != EOF) {
        col = 0;
        while (strcmp(buff + col, "\n")) {
            map[row*len + col] = createTile((buff + col)[0]);
            if ((buff + col)[0] == 'S') {
                startind = row*len + col;
            }
            ++col;
        }
        // Here I'm going to actually set the map COLUMNS dimension correctly. Earlier was a guess.
        if (row == 0) {
            len = col;
            struct tile **p = realloc(map, sizeof(struct tile*)*len*len);
            if (p == NULL) exit(-1);
            map = p;
            p = NULL;
            free(p);
            for (int i = len; i < len*len; ++i) {
                map[i] = calloc(1, sizeof(struct tile));
            }
        }
        ++row;
    } 
    //Now for the map's ROWS dimension
    struct tile **p = realloc(map, sizeof(struct tile*)*len*row);
    if (p == NULL) exit(-1);
    map = p;
    p = NULL;

    getStartDir(map, startind, len);

    #if DEBUG
        printMap(map, len, row);
        printf("Start Position: (%lu, %d)\n", startind % len, (int)(startind / len));
        printf("Start Directions: %d and %d\n", map[startind]->directions[0], map[startind]->directions[1]);
    #endif

    //bfs both directions.
    int newDir[2]; int ct = 0;
    if (map[startind]->directions[0] == left || map[startind]->directions[1] == left)
            newDir[ct++] = -1;
    if (map[startind]->directions[0] == right || map[startind]->directions[1] == right)
        newDir[ct++] = 1;
    if (map[startind]->directions[0] == up || map[startind]->directions[1] == up)
        newDir[ct++] = -len;
    if (map[startind]->directions[0] == down || map[startind]->directions[1] == down)
        newDir[ct++] = len;

    bfs(map, startind + newDir[0], startind, len, 1);
    printf("Part 1: %d\n", bfs(map, startind + newDir[1], startind, len, 1));
    printf("Part 2: %d\n", part2(map, len));

    fclose(fp);
    free(buff);
    free(p);
    for (int i = 0; i < len; ++i) {
        destroyTile(map[i]);
    }
    free(map);
    return 0;
}