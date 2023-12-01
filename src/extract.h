//
// Created by bendm on 21/10/23.
//

#ifndef AD3_PROJECT_EXTRACT_H
#define AD3_PROJECT_EXTRACT_H

#include "tree/linked_list.h"
#include <stdio.h>


int extract(char *inputFile, char *outputFile, int bufferSize);
node* readHeader(FILE *input);
int freeTree(node* root);

#endif //AD3_PROJECT_EXTRACT_H
