//
// Created by bendm on 21/10/23.
//
#include "tree.h"
#include "prefix_util.h"
#include <stdio.h>
#include <stdlib.h>


void printToFile(node *pNode[], int count, FILE *output);

//change huffman to optimal weighted binary tree
int tree(char *inputFile, char *outputFile, int bufferSize){
    FILE *input = fopen(inputFile, "r");
    if(input == NULL){
        printf("Error opening file %s\n", inputFile);
        return 1;
    }

    char buffer[bufferSize];
    int frequency[127] = {0};
    size_t bytesRead;
    while((bytesRead = fread(buffer, sizeof(char), bufferSize, input)) > 0){
        for(size_t i = 0; i < bytesRead; i++){
                frequency[buffer[i]]++;
        }
    }
    fclose(input);

    //process frequency array -> huffman encode
    int nodeCount = 0;
    for(int i = 0; i < 127; i++){
        if(frequency[i] != 0){
            nodeCount++;
        }
    }
    //TODO: optimal weighted binary search tree
    node ** nodeList= processFrequency(frequency,nodeCount);

    //write tree to file using DFS
    //wipe file
    FILE *output=fopen(outputFile, "w");

    //dfs(tree, outputFile);
    qsort(nodeList, nodeCount, sizeof(node*),sortAlfabetically);
    printToFile( nodeList, nodeCount, output);

    fclose(output);


    cleanupMemory(nodeList,nodeCount);

    return 0;
}

void cleanupMemory(node *pNode[],int count) {
    for (int i = 0; i < count; i++) {
        free(pNode[i]->prefix);
        free(pNode[i]->right);
        free(pNode[i]->left);
        free(pNode[i]);
    }
}



void printToFile(node* pNode[], int count, FILE* output) {
    for(int i = 0; i < count; i++){
        fprintf(output, "%d %d %s\n", pNode[i]->value, pNode[i]->frequency, pNode[i]->prefix);
    }
}

int sortAlfabetically(const void *a, const void *b) {
    node *node1 = *(node **)a, *node2 = *(node **)b;
    return (node1->value - node2->value);
}

//TODO: sort backwards so that largest freq el is in the front, then use nodecount to acces elements with lowest freq
//TODO: if value < other value, left, else right give parent value of left child








