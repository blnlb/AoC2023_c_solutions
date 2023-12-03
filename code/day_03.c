#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** getSymbols(char *file);
int** getNumPositions(char *file);
int numDigits(int n);
int checkNums(char *file, char** sym);
int checkGears(char *file, int** numPositions);
int symAdjacent(int row, int col, int colLength, int rowMax, int colMax, char** sym);

int main(int argc, char **argv) {
    //replace test file with your input
    char *file = "/Users/bworley/Documents/SHELL/AoC2023_c_solutions/data/day_03.txt";
    char **symbolMap = getSymbols(file);
    int **numMap = getNumPositions(file);
    printf("Part 1: %d\n", checkNums(file, symbolMap));
    printf("Part 2: %d\n", checkGears(file, numMap));
    return 0;
}

int checkGears(char *file, int** numPositions) {
    FILE *fp = fopen(file, "r");
    size_t len = 142;
    char *buff = malloc(len);

    int *nums = calloc(2, sizeof(int));
    int numNumsLOL = 0, num1Length, num2Length, total = 0;
    int row = -1, col;

    while (getline(&buff, &len, fp) != EOF) {
        col=0;
        ++row;
        while (strncmp(buff + col, "\n", 1)) {
            numNumsLOL = 0;
            while(strncmp(buff+ col, "*", 1) && strncmp(buff+col, "\n", 1)) {
                ++col;
            }
            if (!strncmp(buff + col, "\n", 1)) 
                break;
            //here's the action!!!
            if (numPositions[row][col-1])
                nums[numNumsLOL++] = numPositions[row][col-1];
            if (numPositions[row][col+1])
                nums[numNumsLOL++] = numPositions[row][col+1];
            if (numPositions[row-1][col])
                nums[numNumsLOL++] = numPositions[row-1][col];
            else {
                if (numPositions[row-1][col-1]) 
                    nums[numNumsLOL++] = numPositions[row-1][col-1];
                if (numPositions[row-1][col+1])
                    nums[numNumsLOL++] = numPositions[row-1][col+1];
                
            }
            if (numPositions[row+1][col])
                nums[numNumsLOL++] = numPositions[row+1][col];
            else {
                if (numPositions[row+1][col-1]) 
                    nums[numNumsLOL++] = numPositions[row+1][col-1];
                if (numPositions[row+1][col+1])
                    nums[numNumsLOL++] = numPositions[row+1][col+1];
                
            }
            if (numNumsLOL == 2) {
                total += nums[0]*nums[1];
            }
            ++col;
        }
    }
    fclose(fp);
    free(buff);
    return total;
}

int checkNums(char *file, char** sym) {
    FILE *fp = fopen(file, "r");
    size_t len = 142;
    char *buff = malloc(len);

    int num = 0, numLength, total = 0;
    int row = -1, col = 0;
    while (getline(&buff, &len, fp) != EOF) {
        col=0;
        ++row;
        while (strncmp(buff + col, "\n", 1)) {
            while(sym[row][col] || !strncmp(buff + col, ".", 1)) {
                ++col;
            }
            if (!strncmp(buff + col, "\n", 1)) 
                break;
            num = atoi(buff + col);
            numLength = numDigits(num);
            
            if (symAdjacent(row, col, numLength, len, len, sym)) {
                total += num;
            }

            col += numLength;
        }
    }
    fclose(fp);
    free(buff);
    return total;
}

int symAdjacent(int row, int col, int colLength, int rowMax, int colMax, char** sym) {
    // check if we're on an edge first
    // I'm not checking right side and bottom because I've simply made the matrix big enough
    // and initialized all these values to be 0 in the matrix
    // We need to bypass left of the whole thing first if on left edge
    int i = -1;
    if (col == 0) ++i;
    for (; i <= colLength; ++i) {
        if (row > 0) {
            if (sym[row-1][col + i]) return 1;
        }
        if (sym[row][col+i] || sym[row+1][col+i]) 
            return 1;
    }
    
    return 0;
}

int numDigits(int n) {
    int res = 0;
    while (n > 0) {
        ++res;
        n /= 10;
    }
    return res;
}

int** getNumPositions(char *file) {
    FILE *fp = fopen(file, "r");
    size_t len = 142;
    char *buff = malloc(len);

    //init the return array with values of 0
    int** res = (int**)calloc(len, sizeof(int*));
    for (int i = 0; i < len; ++i) {
        res[i] = calloc(len, sizeof(int));
    }
    int row = 0, col=0;
    int num, numLength;

    while (getline(&buff, &len, fp) != EOF) {
        while (strncmp(buff + col, "\n", 1)) {
            if ((buff + col)[0] > 47 && (buff + col)[0] < 58) {
                num = atoi(buff + col);
                numLength = numDigits(num);
                for (int i = 0; i < numLength; ++i) {
                    res[row][col + i] = num;  
                }
                col += numLength - 1;
            }
            ++col;
        }
        col = 0;
        ++row;
    }
    fclose(fp);
    free(buff);
    return res;
}

char** getSymbols(char *file) {
    FILE *fp = fopen(file, "r");
    size_t len = 142;
    char *buff = malloc(len);

    //init the return array with values of 0
    char** res = (char**)calloc(len, sizeof(char*));
    for (int i = 0; i < len; ++i) {
        res[i] = calloc(len, 1);
    }
    int row = 0, col=0;

    while (getline(&buff, &len, fp) != EOF) {
        while (strncmp(buff, "\n", 1)) {
            if ((buff[0] < 65 && buff[0] > 57) 
                || buff[0] == 47 
                || (buff[0] < 46 && buff[0] > 32)) {
                    ++(res[row][col]);
                }
            ++col; ++buff;
        }
        buff -= col;
        col = 0;
        ++row;
    }
    fclose(fp);
    free(buff);
    return res;
}