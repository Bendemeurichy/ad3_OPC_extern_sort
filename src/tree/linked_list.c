//
// Created by bendm on 2/11/23.
//

#include "linked_list.h"
#include "stdio.h"
#include "stdlib.h"

linkedList* createList(node* value){
    linkedList *list = (linkedList*) malloc(sizeof (linkedList));
    linked_node* firstNode= createNode(value);
    list->firstNode=firstNode;
    list->lastNode=firstNode;
    list->size=1;
    return list;
}

linked_node * createNode(node* value){
    linked_node * newNode=(linked_node *) malloc(sizeof (linked_node));
    newNode->val=value;
    newNode->compatible=1;
    newNode->next=NULL;
    return newNode;
}

void addNode(linkedList *list,node* newVal,int compatible){
    linked_node* newNode= createNode(newVal);
    newNode->compatible=compatible;
    if(list->size==0){
        list->firstNode=newNode;
    }else {
        list->lastNode->next = newNode;

    }
    list->lastNode = newNode;
    list->size++;
}

void addNodeOnPosition(linkedList* list, node* newVal,int index, int compatible){
    if(index==0){
        linked_node* newNode=createNode(newVal);
        newNode->compatible=compatible;
        newNode->next=list->firstNode;
        list->firstNode=newNode;
        list->size++;
        return;
    }
    linked_node* current=list->firstNode;
    while(index>0 && current->next!=NULL){
        current=current->next;
        index--;
    }
    if(list->size==0){
        list->firstNode=createNode(newVal);
        list->lastNode=list->firstNode;
        list->size++;
        return;
    } else {
        linked_node *newNode = createNode(newVal);
        newNode->compatible = compatible;
        newNode->next = (current->next == NULL) ? NULL : current->next;
        current->next = newNode;
        if (newNode->next == NULL) {
            list->lastNode = newNode;
        }
        list->size++;
    }
}

void removeNode(linkedList *list, node* value){
    if (list->firstNode == NULL) {
        return;
    }
    linked_node* prevNode = NULL;
    linked_node* currentNode = list->firstNode;

    while(currentNode != NULL && currentNode->val != value){
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    if(currentNode == NULL){
        return;
    }

    if(prevNode == NULL){
        list->firstNode = currentNode->next;
    } else {
        prevNode->next = currentNode->next;
    }

    if(currentNode == list->lastNode){
        list->lastNode = prevNode;
    }

    free(currentNode);
    list->size--;
}


void removeList(linkedList* list){
    linked_node* currentNode=list->firstNode;
    linked_node* nextNode=NULL;
    while(currentNode!=NULL){
        nextNode=currentNode->next;
        free(currentNode);
        currentNode=nextNode;
    }
    free(list);
}

int cmpfunc (const void * a, const void * b) {
    const node* nodeA = *(const node**)a;
    const node* nodeB = *(const node**)b;

    return ( nodeA->value - nodeB->value );
}

linked_node * get_node_by_index(linkedList * list,int i){
    linked_node* currentNode=list->firstNode;
    while(i>0){
        currentNode=currentNode->next;
        i--;
    }
    return currentNode;
}

linkedList * copyList(linkedList* list){

    linked_node* currentNode=list->firstNode;
    linkedList* newList=createList(currentNode->val);
    while(currentNode!=NULL){
        currentNode=currentNode->next;
        if(currentNode!=NULL){
            addNode(newList,currentNode->val,currentNode->compatible);
        }
    }
    return newList;
}

int get_firt_index_at_level(linkedList * list,int start,int level){
    linked_node* currentNode=get_node_by_index(list,start);
    while(currentNode!=NULL){
        if(currentNode->val->depth==level){
            return start;
        }
        currentNode=currentNode->next;
        start++;
    }
    return -1;
}

linked_node * get_node_by_value(linkedList * list,char value){
    linked_node* currentNode=list->firstNode;
    while(currentNode!=NULL){
        if(currentNode->val->value==value){
            return currentNode;
        }
        currentNode=currentNode->next;
    }
    return NULL;
}