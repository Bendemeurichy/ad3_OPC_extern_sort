//
// Created by bendm on 2/11/23.
//

#ifndef AD3_PROJECT_LINKED_LIST_H
#define AD3_PROJECT_LINKED_LIST_H
typedef struct node {
    int value;
    int frequency;
    char *prefix;
    struct node *left;
    struct node *right;
    int depth;
}node;

typedef struct linked_node{
    node* val;
    int compatible;

    struct linked_node *next;
}linked_node;

typedef struct linkedList{
    int size;
    linked_node *firstNode;
    linked_node *lastNode;
}linkedList;

linkedList * createList(node* value);
linked_node* createNode(node* value);
void addNode(linkedList* ,node*,int compatible);
void addNodeOnPosition(linkedList* list, node* newVal,int index, int compatible);
void removeNode(linkedList * ,node*);
void removeList(linkedList*);
void sortList(linkedList**);
linkedList * copyList(linkedList*);
linked_node * get_node(linkedList * list,int i);
int get_firt_index_at_level(linkedList * list,int start,int level);

#endif //AD3_PROJECT_LINKED_LIST_H
