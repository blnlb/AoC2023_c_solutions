#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int numDigits(int n) {
    int res = 0;
    while (n >0) {
        ++res;
        n /= 10;
    }
    return res;
}

int main(int argc, char** argv) {
    FILE *fp = fopen("test.txt", "r");
    size_t len = 64;
    char *buff = malloc(65);

    int seedMax = 200;
    int *seedMap = calloc(seedMax, sizeof(int));
    for (int i = 0; i < seedMax; ++i) {
        seedMap[i] = i;
    }  
    int *seeds = calloc(4, sizeof(int));
    int seedCount = 0;

    int range, sourceStart, destStart;

    int ind, n;
    while (getline(&buff, &len, fp) != EOF) {
        ind = 0;
        if (strstr(buff, "seeds") != NULL) {
            ind += 6;
            while (strncmp(buff + ind, "\n", 1)) {
                n = atoi(buff +ind);
                seeds[seedCount++] = n;
                ind += numDigits(n) + 1;
            }
            continue;
        }
        if (strstr(buff, "map") != NULL) 
            continue;
        if (!strncmp(buff,"\n",1)) 
            continue;
        //Ugly. Ew. Whatever.
        n = atoi(buff);
        destStart = n;
        ind += numDigits(n);
        n = atoi(buff + ind);
        sourceStart = n;
        ind += numDigits(n);
        range = atoi(buff + ind);;

        for (int i = 0; i < range; ++i) {
            seedMap[sourceStart + i] = seedMap[destStart + i];
        }
    }   
    return 0;
}
