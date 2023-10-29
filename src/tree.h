//
// Created by bendm on 21/10/23.
//

#ifndef AD3_PROJECT_TREE_H
#define AD3_PROJECT_TREE_H
#include "dfs.h"

int tree(char *inputFile, char *outputFile, int bufferSize);
node* processFrequency(int frequency[],int nodeCount);
int compareNodes(const void *node1, const void *node2);
int sortAlfabetically(const void *a, const void *b);



#endif //AD3_PROJECT_TREE_H
