//
// Created by bendm on 21/10/23.
//

#ifndef AD3_PROJECT_COMPRESS_H
#define AD3_PROJECT_COMPRESS_H

int compress(char *inputFile, char *outputFile, int bufferSize, char *prefixCodingFile);
void freePrefixes(char* prefixes[], int size);
#endif //AD3_PROJECT_COMPRESS_H
