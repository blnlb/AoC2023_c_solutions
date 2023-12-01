#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mycompare(char* str, char* sub, int num, int* first, int* last) {
    char* test = malloc(10);
    char* numAsStr = malloc(2);
    sprintf(numAsStr, "%d", num);
    int ind=0;

    // strcpy(cpy, str);
    while ((test = strstr(str + ind, sub))) {
        ind = test-str;
        if (first[num] == -1 || ind < first[num]) {
            first[num] = ind;
        }
        if (last[num] == -1 || ind > last[num]) {
            last[num] = ind;
        }
        ind += strlen(sub);
    }

    ind = 0;
    while ((test = strstr(str+ind, numAsStr))) {
        ind = test-str;
        if (first[num] == -1 || ind < first[num]) {
            first[num] = ind;
        }
        if (last[num] == -1 || ind > last[num]) {
            last[num] = ind;
        }
        ind += 1;
    }
}

int part1() {
    FILE *fp = fopen("/Users/bworley/Documents/SHELL/AoC2023_c_solutions/data/day_01.txt", "r");
    //2^7 covers the largest string size when reading line by line
    size_t len = 128;
    char *buff = malloc(len);
    
    //Going to update first and second coordinate as I read the string one-pass.
    char* nums = malloc(3);
    nums[2] = '\0';

    //Need to flag when the first is stored
    int first_found = 0;
    int c = '\0';

    //Store final tally
    int total = 0;


    while (getline(&buff, &len, fp) != EOF) {
        while (strncmp(buff, "\0", 1)) {
            c = buff[0];
            if (c > 47 && c < 58) {
                nums[1] = *buff;
                if (!first_found) {
                    first_found=1;
                    nums[0] = *buff;
                }
            }
            ++buff;
        }
        first_found=0;
        total += atoi(nums);
    }

    return total;
}

int part2() {
    FILE *fp = fopen("/Users/bworley/Documents/SHELL/AoC2023_c_solutions/data/day_01.txt", "r");
    size_t len = 128;
    char *buff = malloc(len);
    
    //This time, I update a hash map for the 10 values with the index for which they first and last appear.
    // Note we're ignoring the 0th index.
    int* firsts = calloc(10, sizeof(int));
    int* lasts = calloc(10, sizeof(int));

    char* num = malloc(3);
    num[2] = '\0';

    //Again have a running total.
    int total = 0;
    int line_number = 0;

    while (getline(&buff, &len, fp) != EOF) {
        //Refresh hash indices.
        memset(firsts, -1, 10*sizeof(int));
        memset(lasts, -1, 10*sizeof(int));
        //Checking one thing at a time.
        mycompare(buff, "one", 1, firsts, lasts);
        mycompare(buff, "two", 2, firsts, lasts);
        mycompare(buff, "three", 3, firsts, lasts);
        mycompare(buff, "four", 4, firsts, lasts);
        mycompare(buff, "five", 5, firsts, lasts);
        mycompare(buff, "six", 6, firsts, lasts);
        mycompare(buff, "seven", 7, firsts, lasts);
        mycompare(buff, "eight", 8, firsts, lasts);
        mycompare(buff, "nine", 9, firsts, lasts);
        //Then compiling all of that information.
        int lowest = firsts[1], highest = lasts[1];
        int lowestVALUE = 1, highestVALUE = 1;
        for (int i = 2; i < 10; ++i) {
            if ((firsts[i] < lowest && firsts[i] != -1) || lowest == -1) {
                lowest = firsts[i];
                lowestVALUE = i;
            }
            if (lasts[i] > highest) {
                highest = lasts[i];
                highestVALUE = i;
            }
        }
        total += lowestVALUE*10 + highestVALUE;
    }

    return total;
}

int main(int argc, char** argv) {
    printf("Part 1: %d\n", part1());
    printf("Part 2: %d\n", part2());
    return 0;
}