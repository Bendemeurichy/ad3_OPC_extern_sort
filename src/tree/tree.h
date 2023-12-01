//
// Created by bendm on 21/10/23.
//

#ifndef AD3_PROJECT_TREE_H
#define AD3_PROJECT_TREE_H
#include "prefix_util.h"

int tree(char *inputFile, char *outputFile, int bufferSize);
void cleanupMemory(node *pNode[],int count);
void printToFile(node *pNode[], int count, FILE *output);

#endif //AD3_PROJECT_TREE_H
