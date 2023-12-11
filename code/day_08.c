#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int ind[2];
    char* val;
    struct node* left;
    struct node* right;
};

struct node* createNode(char* val) {
    struct node* result = calloc(1, sizeof(struct node));
    result->val = malloc(4);
        memcpy(result->val, val, 3);
        val[3] = 0; 
    result->left = NULL;
    result->right = NULL;

    return result;
}

int nodeSort(const void* a, const void* b) {
    const struct node* nodeA = *(const struct node**)a;
    const struct node* nodeB = *(const struct node**)b;
    return strcmp((nodeA->val), (nodeB->val));
}

void nodeIndexer(struct node **allTrees, int numTrees, struct node* curr, int choice) {
    int left = 0, right = numTrees - 1;
    struct node* next = (choice ? curr->right : curr->left);
    if (!strcmp(next->val, allTrees[left]->val)) curr->ind[choice] = left;
    if (!strcmp(next->val, allTrees[right]->val)) curr->ind[choice] = right;
    while (right - left != 1) {
        int m = (left + 1)/2 + right/2;
        if (strcmp(next->val, allTrees[m]->val) > 0) left = m;
        else if (strcmp(next->val, allTrees[m]->val) < 0) right = m;
        else {
            curr->ind[choice] = m;
            break;
        }
    }
}

int walk (struct node** allTrees, char* dir) {
    int ind = 0;
    int total = 0;
    int start = 0;
    while (strcmp(allTrees[start]->val, "ZZZ")) {
        int newDir = *(dir+(ind++)) == 'L' ? 0 : 1;
        start = allTrees[start]->ind[newDir];
        if (!*(dir + ind)) ind = 0;
        ++total;
    }
    return total;
}


int main(int argc, char **argv) {
    FILE *fp = fopen("day_08.txt", "r");
    size_t len = 300;
    char* buff = malloc(len);
    int ind = 0;

    char* directions = malloc(len);
    struct node** allTrees = (struct node**)calloc(800, sizeof(struct node*));
    for (int i = 0; i < 800; ++i) {
        allTrees[i] = calloc(1, sizeof(struct node));
    }
    int numTrees = 0;

    struct node* root = NULL;
    struct node* p = NULL;

    getline(&buff,&len, fp);
    strncpy(directions, buff, strchr(buff, '\n') - buff);
    getline(&buff,&len, fp); // get ride of newline

    while (getline(&buff,&len, fp) != EOF) {
        root = createNode(buff);
        allTrees[numTrees++] = root;
        root->left = createNode(buff + 7);
        root->right = createNode(buff + 12);
    }

    qsort((struct node*)allTrees, numTrees, sizeof(struct node*), nodeSort);

    for (int i = 0; i < numTrees; ++i) {
        nodeIndexer(allTrees, numTrees, allTrees[i], 0);
        nodeIndexer(allTrees, numTrees, allTrees[i], 1);
    }

    int total = walk(allTrees, directions);

    printf("Part 1: %d\n", total);


    free(buff);
    free(directions);
    fclose(fp);
    return 0;
}