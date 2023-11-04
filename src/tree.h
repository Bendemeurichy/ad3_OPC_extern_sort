//
// Created by bendm on 21/10/23.
//

#ifndef AD3_PROJECT_TREE_H
#define AD3_PROJECT_TREE_H
#include "prefix_util.h"

int tree(char *inputFile, char *outputFile, int bufferSize);
int compareNodes(const void *node1, const void *node2);
int sortAlfabetically(const void *a, const void *b);



#endif //AD3_PROJECT_TREE_H
