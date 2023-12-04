//
// Created by bendm on 21/10/23.
//

#ifndef AD3_PROJECT_SORT_H
#define AD3_PROJECT_SORT_H

#include <stdio.h>

#include <bits/stdint-uintn.h>

int sort(char *inputFile, char *outputFile, int bufferSize);
int compareLines(const void *a, const void *b);
int freeLines(uint8_t **lines, long size);
int blockSort(FILE *input, int bufferSize);
int mergeFiles(int tempfiles, int bufferSize);

#endif //AD3_PROJECT_SORT_H
