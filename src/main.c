//
// Created by bendm on 21/10/23.
//
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree/tree.h"
#include "compress.h"
#include "extract.h"
#include "sort.h"

int main(int argc, char *argv[]) {
    //possible arguments
    char *command = argv[1];
    int opt;
    char *inputFile = NULL;
    char *outputFile = NULL;
    int bufferSize = 0;
    char *prefixCodingFile = NULL;

    //first argument is already parsed as the function to execute
    argc--;
    argv++;


    //getopts for options
    while((opt = getopt(argc, argv, "i:o:m:t:")) != -1) {
        switch(opt) {
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'm':
                bufferSize = atoi(optarg);
                break;
            case 't':
                prefixCodingFile = optarg;
                break;
            default:
                break;
        }
    }

    //errorhandling
    if(strcmp(command, "compress") != 0 && prefixCodingFile != NULL){
        printf("Error: option -t is only allowed for compress command");
        return -1;
    }

    if(inputFile == NULL){
        printf("Error: option -i is required");
        return -1;
    }
    if(outputFile == NULL){
        printf("Error: option -o is required");
        return -1;
    }
    if(bufferSize == 0){
        printf("Error: option -m is required");
        return -1;
    }

    int temp =0;

    //call for the right function
    if(strcmp(command,"tree")==0){
        tree(inputFile, outputFile, bufferSize);
    } else if(strcmp(command,"compress")==0){
        if(prefixCodingFile == NULL){

            tree(inputFile, "tmp.txt",bufferSize);
            prefixCodingFile = "tmp.txt";
            temp = 1;
        }
        compress(inputFile, outputFile, bufferSize, prefixCodingFile);
        if(temp==1){
            remove("tmp.txt");
        }
    } else if (strcmp(command,"extract")==0){
        extract(inputFile, outputFile, bufferSize);
    } else if(strcmp(command,"sort")==0){
        sort(inputFile, outputFile, bufferSize);
    } else {
        printf("Error: unknown command");
        return -1;
    }

    return 0;
}
