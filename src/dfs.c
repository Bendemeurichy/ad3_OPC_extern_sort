//
// Created by bendm on 24/10/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfs.h"
void buildPrefixList(node* current, struct node* nodeList[], char* prefix, int index, int* nodeListIndex) {
    if (current == NULL) {
        return;
    }

    // Append '0' for left and '1' for right in the prefix
    if (current->left != NULL) {
        prefix[index] = '0';
        buildPrefixList(current->left, nodeList, prefix, index + 1, nodeListIndex);
    }

    if (current->right != NULL) {
        prefix[index] = '1';
        buildPrefixList(current->right, nodeList, prefix, index + 1, nodeListIndex);
    }

    // If the node is a leaf, store its details including the prefix code
    if (!current->left && !current->right) {
        nodeList[*nodeListIndex] = current;

        nodeList[*nodeListIndex]->prefix = (char*)malloc((index + 1) * sizeof(char));
        strncpy(nodeList[*nodeListIndex]->prefix, prefix, index);
        nodeList[*nodeListIndex]->prefix[index] = '\0';
        (*nodeListIndex)++;
    }
}
