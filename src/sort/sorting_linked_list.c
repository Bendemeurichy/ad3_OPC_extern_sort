//
// Created by bendm on 4/12/23.
//

#include <string.h>
#include <stdlib.h>
#include "sorting_linked_list.h"

temp_file_list * create_temp_list(){
    temp_file_list *list = (temp_file_list*) malloc(sizeof (temp_file_list));
        list->firstNode=NULL;
        list->lastNode=NULL;
        list->size=0;
        return list;
}
temp_file* create_temp_file(char* value,FILE* file){
    temp_file * newNode=(temp_file *) malloc(sizeof (temp_file));
    newNode->file=file;
    strcpy(newNode->name,value);
    newNode->next=NULL;
    return newNode;
}
void add_temp_file(temp_file_list* list ,char* newVal,FILE* file){
    temp_file * newNode= create_temp_file(newVal,file);
    if(list->size==0){
        list->firstNode=newNode;
    }else {
        list->lastNode->next = newNode;

    }
    list->lastNode = newNode;
    list->size++;

}
void remove_temp_file(temp_file_list * list,char* value){
    temp_file * current=list->firstNode;
    temp_file * prev=NULL;
    while(current!=NULL){
        if(strcmp(current->name,value)==0){
            if(prev==NULL){
                list->firstNode=current->next;
            }else{
                prev->next=current->next;
            }
            if(current->next==NULL){
                list->lastNode=prev;
            }
            fclose(current->file);
            remove(current->name);
            free(current->name);
            free(current);
            list->size--;
            return;
        }
        prev=current;
        current=current->next;
    }

}
void remove_temp_file_list(temp_file_list* list){
    temp_file * current=list->firstNode;
    temp_file * next=NULL;
    while(current!=NULL){
        next=current->next;

        fclose(current->file);
        remove(current->name);
        free(current->name);
        free(current);
        current=next;
    }
    free(list);
}