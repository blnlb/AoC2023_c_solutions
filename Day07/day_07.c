#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct hand {
    size_t rank;
    int strength;
    int* cards;
    size_t bid;
};

enum card {
    J = 1,
    T = 10,
    Q,
    K,
    A
};

enum strength {
    None,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind
};

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int findStrength(int *s) {
    int numMatches = 0, numPairs = 0, two = 0,
        three = 0, four = 0, j, numJs = 0, five = 0;

    int *cpy = calloc(5, sizeof(int));
    for (int i = 0; i < 5; ++i) {
        cpy[i] = s[i];
    }
    qsort(cpy, 5, sizeof(int), cmp);
    int i = 0;
    for (; i < 5; ++i) {
        if (cpy[i] != 1) {
            break;
        }
        ++numJs;
    }
    for (; i < 4; ++i) {
        if (cpy[i] == cpy[i + 1]) {
            ++numMatches;
            ++numPairs;
            switch (numJs) {
                case 0:
                    two = 1;
                    break;
                case 1:
                    if (three == 1) {
                        two = 1;
                    }
                    else {
                        three = 1;
                        --numPairs;
                    }
                    break;
                case 2:
                    four = 1;
                    break;
                case 3:
                    five = 1;
                    break;
            }
            j = i + 2;
            while (j < 5 && cpy[i] == cpy[j++]) {
                if (two) {
                    two = 0;
                    three = 1;
                    --numPairs;
                    continue;
                }
                if (three) {
                    three = 0;
                    four = 1;
                    continue;
                }
                if (four) {
                    four = 0;
                    five = 1;
                }
            }
            i += (j-i) - 2;
        }
    }
    if (!numMatches) {
        switch (numJs) {
            case 0: return None;
            case 1: return OnePair;
            case 2: return ThreeOfAKind;
            case 3: return FourOfAKind;
            case 4:
            case 5: return FiveOfAKind;
        }
    }
    if (numMatches == 1 && numPairs == 1 && !four && !five) 
        return OnePair;
    if (numMatches == 2 && numPairs == 2)
        return TwoPair;
    if (numMatches == 1 && numPairs == 0 && three)
        return ThreeOfAKind;
    if (numMatches == 2 && numPairs == 1 && three)
        return FullHouse;
    if (numMatches == 1 && four) 
        return FourOfAKind;
        return FiveOfAKind;
}

int* convertHand(char* hand) {
    int* result = calloc(5, sizeof(int));
    for (int i = 0; i < 5; ++i) {
        if (isdigit(hand[i])) {
            result[i] = hand[i] - '0';
        } else {
            switch (hand[i]) {
                case 'T':
                    result[i] = T;
                    break;
                case 'J':
                    result[i] = J;
                    break;
                case 'Q':
                    result[i] = Q;
                    break;
                case 'K':
                    result[i] = K;
                    break;
                case 'A':
                default:
                    result[i] = A;
            }
        }
    }
    return result;
}

struct hand* createHand(size_t bid, char* cards) {
    struct hand* result = calloc(1, sizeof(struct hand));
    result->cards = calloc(5, sizeof(int));
    result->bid = bid;
    result->cards = convertHand(cards);
    result->strength = findStrength(result->cards);
    result->rank = 1;
    return result;
}

void rankHand(struct hand* curr, struct hand** allHands, int numHands) {
    for (int i = 0; i < numHands; ++i) {
        if (curr->strength > allHands[i]->strength) ++curr->rank;
        else if (curr->strength < allHands[i]->strength) continue;
        else {
            //hand comparison
            for (int j = 0; j < 5; ++j) {
                if (curr->cards[j] == allHands[i]->cards[j]) continue;
                else if (curr->cards[j] < allHands[i]->cards[j]) break;
                else if (curr->cards[j] > allHands[i]->cards[j]) {
                    ++curr->rank;
                    break;
                }
            }
        }
    }
}

size_t atouint(char *s) {
    int ind = 0;
    size_t res = 0;
    while (isdigit((s+ind)[0])) {
        res = res*10 + ((s+ind)[0] - '0');
        ++ind;
    }
    return res;
}
size_t atouint_ignorespaces(char *s) {
    int ind = 0;
    size_t res = 0;
    while (strncmp(s + ind, "\n", 1)) {
        if ((s+ind)[0] == ' ') {
            ++ind;
            continue;
        }
        res = res*10 + ((s+ind)[0] - '0');
        ++ind;
    }
    return res;
}

int numDigits(size_t n) {
    int res = 0;
    do {
        ++res;
        n /= 10;
    } while (n > 0);
    return res;
}

int part1(char *file) {
    FILE *fp = fopen(file, "r");
    size_t len = 64;
    char *buff = malloc(len);

    int total = 0;
    struct hand **allHands = (struct hand **)calloc(1000, sizeof(struct hand*));
    for (int i = 0; i < 1000; ++i) {
        allHands[i] = calloc(1, sizeof(struct hand));
    }

    //FIRST we parse the input and create a struct for each hand.
    char *s = malloc(6); s[5] = 0;
    size_t bid;
    int numHands = 0;
    while (getline(&buff, &len, fp) != EOF) {
        strncpy(s, buff, 5);
        bid = atouint(buff + 6);
        allHands[numHands++] = createHand(bid, s);
    }    
    //Next we rank the hands! Maybe we should sort by strength first.
    //BUT for now we do not.
    for (int i = 0; i < numHands; ++i) {
        rankHand(allHands[i], allHands, numHands);
    }

    //THEN calculate the total. Jesus.
    for (int i = 0; i < numHands; ++i) {
        total += allHands[i]->rank*allHands[i]->bid;
    }
    return total;
}


int main(int argc, char **argv) {
    char *file = "day_07.txt";
    printf("Part 1: %d\n", part1(file));
    return 0;
}