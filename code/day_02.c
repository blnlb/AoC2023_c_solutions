#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

struct game {
    size_t blue;
    size_t green;
    size_t red;
} game;

struct game* createGame(int b, int g, int r) {
    struct game *result = calloc(1, sizeof(struct game));
    result->blue = b;
    result->green = g;
    result->red = r;
    return result;
}

void updateGame(struct game* ptr, int col, int c) {
    if (c == 'b') ptr->blue = col;
    else if (c == 'g') ptr->green = col;
    else if (c == 'r') ptr->red = col;
}

size_t part1();
size_t part2();
int searchForCharacterAndJudge(char* str, char *cmp, size_t maxValue);
int searchForCharacterAndGetValue(char* str, char* cmp);

int main(int argc, char** argv) {
    printf("Part 1: %zu\n", part1());
    printf("Part 2: %zu\n", part2());
    return 0;
}

size_t part2() {
    FILE *fp = fopen("test.txt", "r");
    size_t len = 512;
    char *buff = malloc(len);

    struct game* currGame = createGame(0, 0, 0);

    size_t total = 0;
    size_t curr = 1;
    char* cpy = malloc(len);
    int colNum = 0;

    while (getline(&buff, &len, fp) != EOF) {
        updateGame(currGame, 0, 'b');
        updateGame(currGame, 0, 'g');
        updateGame(currGame, 0, 'r');
        curr = 1;
        cpy = strstr(buff, ":") + 2;
        char *p = strtok(cpy, ";");
        while (p != NULL) {
            if ((colNum = searchForCharacterAndGetValue(p, "red")) > currGame->red) 
                currGame->red = colNum;
            if ((colNum = searchForCharacterAndGetValue(p, "blue")) > currGame->blue) 
                currGame->blue = colNum;
            if ((colNum = searchForCharacterAndGetValue(p, "green")) > currGame->green) 
                currGame->green = colNum;
            p = strtok(NULL, ";");
        }
        total += currGame->red*currGame->blue*currGame->green;
    }

    free(currGame);
    return total;
}

size_t part1() {
    FILE *fp = fopen("test.txt", "r");
    size_t len = 512;
    char *buff = malloc(len);

    struct game* currGame = createGame(14, 13, 12);

    size_t curr = 0;
    size_t total = 0;
    char* cpy = malloc(len);
    int badgame = 0;

    while (getline(&buff, &len, fp) != EOF) {
        badgame = 0;
        ++curr;
        cpy = strstr(buff, ":") + 2;
        char *p = strtok(cpy, ";");
        while (p != NULL) {
            badgame = searchForCharacterAndJudge(p, "red", currGame->red) |
                      searchForCharacterAndJudge(p, "green", currGame->green) |
                      searchForCharacterAndJudge(p, "blue", currGame->blue);
            if (badgame) break;
            p = strtok(NULL, ";");
        }
        if (!badgame) {
            total += curr;
        }
    }
    return total;
}

int searchForCharacterAndJudge(char* str, char *cmp, size_t maxValue) {
    char* characterSearch = malloc(512);
    int currNum = 0;
    if ((characterSearch = strstr(str, cmp)) != NULL) {
        characterSearch -= 2;
        while (isdigit(characterSearch[0])) {
            --characterSearch;
        }
        currNum = atoi(characterSearch);
        if (currNum > maxValue) {
            return 1;
        }
    }
    return 0;
}

int searchForCharacterAndGetValue(char* str, char* cmp) {
    char* characterSearch = malloc(512);

    if ((characterSearch = strstr(str, cmp)) != NULL) {
        characterSearch -= 2;
        while (isdigit(characterSearch[0])) {
            --characterSearch;
        }
        return atoi(characterSearch);

    }
    return 0;
}