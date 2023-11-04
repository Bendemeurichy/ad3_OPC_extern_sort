//
// Created by bendm on 24/10/23.
//

//bfs nodig om boom te schrijven naar file
#ifndef AD3_PROJECT_PREFIX_UTIL_H
#define AD3_PROJECT_PREFIX_UTIL_H
#include <stdio.h>
#include "linked_list.h"


void buildPrefixList(node* current, struct node* nodeList[], char* prefix, int index, int* nodeListIndex);
node *processFrequency(int frequency[]);
node *create_prefix_tree(linkedList *nodes);
node *merge(linkedList *nodes);
int find_min_compatible(linkedList* nodes, int i);
int findminimum(linkedList* nodes,int i);
void merge_nodes(linkedList* nodes,int i,int j,int depth);
void construct_level(linkedList* nodes, int level);



#endif //AD3_PROJECT_PREFIX_UTIL_H
