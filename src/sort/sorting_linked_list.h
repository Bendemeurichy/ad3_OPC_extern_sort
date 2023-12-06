//
// Created by bendm on 4/12/23.
//

#ifndef AD3_PROJECT_SORTING_LINKED_LIST_H
#define AD3_PROJECT_SORTING_LINKED_LIST_H
#include <stdio.h>

typedef struct temp_file{
    char* name;
    FILE* file;

    struct temp_file *next;
}temp_file;

typedef struct temp_file_list{
    int size;
    temp_file *firstNode;
    temp_file *lastNode;
}temp_file_list;

temp_file_list * create_temp_list();
temp_file* create_temp_file(char* value,FILE* file);
void add_temp_file(temp_file_list* ,char*,FILE*);
void remove_temp_file(temp_file_list * ,char*);
void remove_temp_file_list(temp_file_list*);
temp_file * get_temp_file(temp_file_list*,int index);


#endif //AD3_PROJECT_SORTING_LINKED_LIST_H
