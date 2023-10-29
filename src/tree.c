//
// Created by bendm on 21/10/23.
//
#include "tree.h"
#include "dfs.h"
#include <stdio.h>
#include <stdlib.h>


void printToFile(node *pNode[], int count, FILE *output);

void cleanupMemory(node *pNode);

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
    node* tree=processFrequency(frequency,nodeCount);

    //write tree to file using DFS
    //wipe file
    FILE *output=fopen(outputFile, "w");

    node* nodeList[nodeCount];
    //just process the tree and add the prefix to the node
    int index = 0;
    int* nodelistIndex=(int*) malloc(sizeof (int)) ;
    char *prefix=malloc(sizeof(char)*nodeCount);
    //just write every code to node and make list in other bit of code
    buildPrefixList(tree, nodeList,prefix , index,nodelistIndex);
    //dfs(tree, outputFile);
    qsort(nodeList, nodeCount, sizeof(node*),sortAlfabetically);
    printToFile( nodeList, nodeCount, output);
    cleanupMemory(tree);
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


node* processFrequency(int frequency[],int nodeCount){
    node *nodes[nodeCount] = {};
    int nextNode = 0;
    for(int i = 0; i < 127; i++){
        if(frequency[i]!=0){
            nodes[nextNode] = (node*) malloc(sizeof(node));
            nodes[nextNode]->value = i;
            nodes[nextNode]->frequency = frequency[i];
            nodes[nextNode]->left = NULL;
            nodes[nextNode]->right = NULL;
            nextNode++;
        }
    }
    //sort nodes
    qsort(nodes, nodeCount, sizeof(node*),compareNodes);
    //build tree
    while(nodeCount > 1){
        node *newNode = malloc(sizeof(node));
        if(nodes[nodeCount-1]->value<nodes[nodeCount-2]->value){
            newNode->left = nodes[nodeCount-1];
            newNode->right = nodes[nodeCount-2];
        }else{
            newNode->left = nodes[nodeCount-2];
            newNode->right = nodes[nodeCount-1];
        }
        newNode->left->parent = newNode;
        newNode->right->parent = newNode;
        newNode->frequency = newNode->left->frequency + newNode->right->frequency;
        newNode->value = newNode->left->value;
        nodes[nodeCount-2] = newNode;
        nodeCount--;
        //vies, hoe anders 2 laagste frequencies te vinden? quicksort op bijna gesorteerde array is toch snel
        qsort(nodes, nodeCount, sizeof(node*),compareNodes);
    }
    return nodes[0];
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








