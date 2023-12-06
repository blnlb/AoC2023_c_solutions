#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

struct list {
    struct list* next;
    u_int32_t val;
};

struct list* createList(u_int32_t val) {
    struct list *res = calloc(1,sizeof(struct list));
    res->val = val;
    res->next = NULL;
    return res;
}

void deleteList(struct list* l) {
    free(l->next);
    free(l);
}

int numDigits(u_int32_t n) {
    int res = 0;
    do {
        ++res;
        n /= 10;
    } while (n > 0);
    return res;
}

u_int32_t atouint(char* s) {
    int ind = 0;
    uint32_t res = 0;
    while (isdigit((s+ind)[0])) {
        ++ind;
    }
    int power = 0;
    while (ind-- > 0) {
        res += ((s+ind)[0] - '0')*pow(10,power++);
    }
    return res;
}

int main(int argc, char** argv) {
    char *filename = "/Users/bworley/Documents/code/AoC2023_c_solutions/data/day_05.txt"; 
    FILE *fp = fopen(filename, "r");
    size_t len = 64;
    char *buff = malloc(65);

    struct list *curr = NULL;
    u_int32_t n;
    int ind = 7, steps = -1;

    int maxSeeds = 1;
    struct list **seeds = (struct list **)calloc(maxSeeds, sizeof(struct list*));
    struct list **seedsPTR = NULL;
    for (int i = 0; i < maxSeeds; ++i) {
        seeds[i] = calloc(1, sizeof(struct list));
    }
    int seedCount = 0;
    getline(&buff, &len, fp);
    int even = 0;
    u_int32_t oldMax;
    while (strncmp(buff + ind - 1, "\n", 1)) {
        n = atouint(buff + ind);
        if (!even) {
            seeds[seedCount++] = createList(n);
            even = 1;
        } else {
            maxSeeds = seedCount + n;
            seedsPTR = realloc(seeds, maxSeeds*sizeof(struct list*));
            if (seedsPTR == NULL) {
                printf("Not good!\n");
                exit(-1);
            } else {
                seeds = seedsPTR;
                seedsPTR = NULL;
                for (; seedCount < maxSeeds - 1; ++seedCount) {
                    seeds[seedCount] = calloc(1, sizeof(struct list));
                    seeds[seedCount] = createList(seeds[seedCount-1]->val + 1);
                }
            }
            even = 0;
        }
        ind += numDigits(n) + 1;
    }

    u_int32_t range, sourceStart, destStart;

    while (getline(&buff, &len, fp) != EOF) {
        ind = 0;
        // if (strstr(buff, "fertilizer-to-water") != NULL) {
        //     ;
        // }
        if (strstr(buff, "map") != NULL) {
            ++steps;
            continue;
        }
        if ((!strncmp(buff,"\n",1))) 
        {   
            if (steps > -1) {
                for (int i = 0; i < seedCount; ++i) {
                    curr = seeds[i];
                    for (int i = 0; i < steps; ++i) {
                        curr = curr->next;
                    }
                    if (!curr->next) {
                        curr->next = createList(curr->val);
                    }
                }
            }
            continue;
        }
            
        //Ugly. Ew. Whatever.
        n = atouint(buff);
        destStart = n;
        ind += numDigits(n) + 1;
        n = atouint(buff + ind);
        sourceStart = n;
        ind += numDigits(n) + 1;
        range = atoi(buff + ind);

        for (int i = 0; i < seedCount; ++i) {
            curr = seeds[i];
            for (int i = 0; i < steps; ++i) {
                curr = curr->next;
            }
            if (!curr->next && (curr->val >= sourceStart && curr->val - sourceStart < range)) {
                if (destStart > sourceStart) {
                    curr->next = createList(curr->val + (destStart - sourceStart));
                } else {
                    curr->next = createList(curr->val - (sourceStart - destStart));
                }
            }
        }
    }   

    // printf("Current seed placement: \n");
    //     for (int i = 0; i < seedCount; ++i) {
    //         curr = seeds[i];
    //         printf("Seed %d: ", i);
    //         while(curr != NULL) {
    //             printf("%d\t", curr->val);
    //             curr = curr->next;
    //         }
    //         printf("\n");
    //     }
    u_int32_t lowest = UINT32_MAX;
    for (int i = 0; i < seedCount; ++i) {
        curr = seeds[i];
        while (curr->next) {
            curr = curr->next;
        }
        if (curr->val < lowest) {
            lowest = curr->val;
        }
    }
    printf("%d\n", lowest);

    fclose(fp);
    free(buff);
    for (int i = 0; i < seedCount; ++i) {
        deleteList(seeds[i]);
    }
    free(seeds);
    
    return 0;
}
