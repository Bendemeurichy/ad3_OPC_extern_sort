//
// Created by bendm on 21/10/23.
//
#include "tree.h"
#include "prefix_util.h"
#include <stdio.h>
#include <stdlib.h>


void printToFile(node *pNode[], int count, FILE *output);

void cleanupMemory(node *pNode);

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
    node* tree= processFrequency(frequency);

    //write tree to file using DFS
    //wipe file
    FILE *output=fopen(outputFile, "w");

    node* nodeList[nodeCount];
    //just process the tree and add the prefix to the node
    int index = 0;
    int* nodelistIndex=(int*) malloc(sizeof (int)) ;
    *nodelistIndex = 0;
    char *prefix=malloc(sizeof(char)*nodeCount);
    //just write every code to node and make list in other bit of code
    buildPrefixList(tree, nodeList,prefix , index,nodelistIndex);
    //dfs(tree, outputFile);
    qsort(nodeList, nodeCount, sizeof(node*),sortAlfabetically);
    printToFile( nodeList, nodeCount, output);
    //cleanupMemory(tree);
    fclose(output);
    free(nodelistIndex);
    free(prefix);
    return 0;
}

void cleanupMemory(node *pNode) {

    if(pNode->left != NULL){
        cleanupMemory(pNode->left);
    }
    if(pNode->right != NULL){
        cleanupMemory(pNode->right);
    }
    free(pNode);

}

void printToFile(node* pNode[], int count, FILE* output) {
    for(int i = 0; i < count; i++){
        fprintf(output, "%c %d %s\n", pNode[i]->value, pNode[i]->frequency, pNode[i]->prefix);
    }
}



int compareNodes(const void *a, const void *b) {
    node *node1 = *(node **)a, *node2 = *(node **)b;
    if(node1->frequency == node2->frequency){
        return (node2->value - node1->value);
    }
    return (node2->frequency - node1->frequency);
}

int sortAlfabetically(const void *a, const void *b) {
    node *node1 = *(node **)a, *node2 = *(node **)b;
    return (node1->value - node2->value);
}

//TODO: sort backwards so that largest freq el is in the front, then use nodecount to acces elements with lowest freq
//TODO: if value < other value, left, else right give parent value of left child








