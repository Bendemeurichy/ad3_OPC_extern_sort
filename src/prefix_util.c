//
// Created by bendm on 24/10/23.
//
#include <stdlib.h>
#include <string.h>
#include "prefix_util.h"
#include <limits.h>

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
    if (current->left==NULL && current->right==NULL) {
        nodeList[*nodeListIndex] = current;

        nodeList[*nodeListIndex]->prefix = (char*)malloc((index + 1) * sizeof(char));
        strncpy(nodeList[*nodeListIndex]->prefix, prefix, index);
        nodeList[*nodeListIndex]->prefix[index] = '\0';
        (*nodeListIndex)++;
    }
}

//hu-tucker encoding/ Garsia-Wachs/ iets makkelijker
node *processFrequency(int frequency[]) {
    //node* nodes[nodeCount];
    linkedList* nodes=NULL;
    //int nextIndex=0;
    for(int i = 0; i < 127; i++){
        if(frequency[i]!=0){
            node* newNode = (node*) malloc(sizeof(node));
            newNode->value = i;
            newNode->frequency = frequency[i];
            newNode->depth = 0;
            newNode->left = NULL;
            newNode->right = NULL;
            if(nodes==NULL){
                nodes= createList(newNode);
            }else {
                addNode(nodes,newNode,1);
            }
            //nodes[nextIndex]=newNode;
            //nextIndex++;

        }
    }


    //build weighted bst
    //TODO: FIX
    node* root= create_prefix_tree(nodes);
    return root;

}


//hu tucker gevonden bij https://math.mit.edu/~djk/18.310/Lecture-Notes/PeterShor-hu-tucker.html
//phase 1
node *merge(linkedList *nodes) {

    while(nodes->size>1){
        int minimum=findminimum(nodes,0);
        //printf("minimum: %d\n", get_node(nodes,minimum)->val->value);
        int compatible=find_min_compatible(nodes,minimum);
        merge_nodes(nodes,minimum,compatible,0);

    }
    return get_node(nodes,0)->val;
}

int find_min_compatible(linkedList* nodes, int i){
    int compatible=1;
    int min= INT_MAX;
    int minLeft,minRight;
    minLeft=minRight=-1;
    //find left
    if(i!=0){
        int j=i-1;
        while(j>=0 && compatible==1){
            if(get_node(nodes,j)->val->frequency<min){
                min=get_node(nodes,j)->val->frequency;
                minLeft=j;
            }
            if(get_node(nodes,j)->compatible==1){
                compatible=0;
            }
            j--;
        }
    }
    //find right
    compatible=1;
    if(i!=nodes->size-1){
        int j=i+1;
        while(j<nodes->size && compatible==1){
            if(get_node(nodes,j)->val->frequency<min){
                min=get_node(nodes,j)->val->frequency;
                minRight=j;
            }
            if(get_node(nodes,j)->compatible==1){
                compatible=0;
            }
            j++;
        }
    }
    int min_index=-1;
    if(minLeft!=-1 && minRight!=-1){
        min_index=(get_node(nodes,minLeft)->val->frequency<get_node(nodes,minRight)->val->frequency)?minLeft:minRight;
    } else if(minLeft!=-1){
        min_index=minLeft;
    } else if(minRight!=-1){
        min_index=minRight;
    }

    return min_index;
}

int findminimum(linkedList* nodes,int i){
    int min=i;
    for(int j=i+1;j<nodes->size;j++){
        if(get_node(nodes,j)->val->frequency<get_node(nodes,min)->val->frequency){
            min=j;
        }
    }
    return min;
}

void merge_nodes(linkedList* nodes,int i,int j,int depth){
    node* left=get_node(nodes,i)->val;
    node* right=get_node(nodes,j)->val;
    node* newNode=(node*) malloc(sizeof(node));
    newNode->value= right->value;
    newNode->frequency=left->frequency+right->frequency;
    if(left->value<right->value){
        newNode->left=left;
        newNode->right=right;
    } else{
        newNode->left=right;
        newNode->right=left;
    }
    newNode->prefix=NULL;
    newNode->depth=depth;

    addNodeOnPosition(nodes,newNode,i,0);
    removeNode(nodes,left);
    removeNode(nodes,right);
}

//phase 2
void assign_depths(node* root, int depth, int* max_depth){
    if(root->left==NULL && root->right==NULL){
        root->depth=depth;
        //printf("depth for node: %d %d\n",root->value,depth);
        *max_depth = (*max_depth > depth) ? *max_depth : depth;
    } else{
        assign_depths(root->left,depth+1,max_depth);
        assign_depths(root->right,depth+1,max_depth);
    }
}

//phase 3
node* construct_alfabetic_tree(linkedList* nodes,int max_depth){
    int depth=max_depth;
    while(nodes->size>1){
        construct_level(nodes,depth);
        depth--;
    }
    return get_node(nodes,0)->val;
}

void construct_level(linkedList* nodes, int level){
    while(get_firt_index_at_level(nodes,0,level)!=-1){
        int index1= get_firt_index_at_level(nodes,0,level);
        node* node1= get_node(nodes,index1)->val;
        node* node2= get_node(nodes, get_firt_index_at_level(nodes,index1+1,level))->val;
        node* new_node=(node*) malloc(sizeof(node));
        new_node->value=node2->value;
        new_node->frequency=node1->frequency+node2->frequency;
        new_node->left=node1;
        new_node->right=node2;
        new_node->prefix=NULL;
        new_node->depth=level-1;
        addNodeOnPosition(nodes,new_node,index1,0);
        removeNode(nodes,node1);
        removeNode(nodes,node2);

    }
}

//Combine all phases
node *create_prefix_tree(linkedList *nodes) {
    //need a copy of the original list because it breaks from phase1
    linkedList* copy=copyList(nodes);
    node* root= merge(nodes);
    int max_depth=0;
    assign_depths(root,0,&max_depth);
    node* new_root=construct_alfabetic_tree(copy,max_depth);
    return new_root;
}
