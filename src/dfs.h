//
// Created by bendm on 24/10/23.
//

//bfs nodig om boom te schrijven naar file
#ifndef AD3_PROJECT_DFS_H
#define AD3_PROJECT_DFS_H
#include <stdio.h>

typedef struct node {
    struct node* parent;
    int value;
    int frequency;
    char *prefix;
    struct node *left;
    struct node *right;
}node;
void buildPrefixList(node* current, struct node* nodeList[], char* prefix, int index, int* nodeListIndex);

#endif //AD3_PROJECT_DFS_H
