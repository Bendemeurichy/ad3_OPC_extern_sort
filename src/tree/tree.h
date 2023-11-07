//
// Created by bendm on 21/10/23.
//

#ifndef AD3_PROJECT_TREE_H
#define AD3_PROJECT_TREE_H
#include "prefix_util.h"

int tree(char *inputFile, char *outputFile, int bufferSize);
int sortAlfabetically(const void *a, const void *b);
void cleanupMemory(node *pNode[],int count);

#endif //AD3_PROJECT_TREE_H
