//
// Created by bendm on 21/10/23.
//

#include <stdio.h>
#include "extract.h"
#include <stdlib.h>

int extract(char *inputFile, char *outputFile, int bufferSize){
   FILE * input = fopen(inputFile, "r");
   if(input == NULL){
       printf("Error opening file %s\n", inputFile);
       return 1;
   }

    node *root = readHeader(input);

    FILE *output = fopen(outputFile, "w");
    if(output == NULL){
        printf("Error opening file %s\n", outputFile);
        return 1;
    }

    char* buffer = (char*) malloc(sizeof(char) * bufferSize);
    size_t read = 0;
    char * linebuffer = (char*) malloc(sizeof(char) * bufferSize);
    int linebufferindex = 0;
    int linesize = 0;
    char currentByte =0;
    char lastbit = 0;
    int currentByteIndex = 0;
    node* currentNode = root;

    while((read = fread(buffer, sizeof(char), bufferSize, input)) > 0){
        for(size_t i=0;i<read;i++){
            if(currentByteIndex == 0){
                linesize = buffer[i]<<16 | buffer[i+1]<<8 | buffer[i+2];
                //last 3 bits of linesize is lastbit
                lastbit = linesize & 7;
                linesize = linesize >> 3;
                i+=3;
            }
            if(currentByteIndex == (linesize-1)){
                //last byte
                currentByte = buffer[i];
                int bitIndex = 7;

                while(bitIndex>= (8-lastbit)){
                    if((currentByte>>bitIndex) & 1){
                        currentNode = currentNode->right;
                    }else{
                        currentNode = currentNode->left;
                    }
                    if(currentNode->value != 0){
                        linebuffer[linebufferindex] = currentNode->value;
                        linebufferindex++;
                        currentNode = root;
                    }
                    bitIndex--;
                }

                linesize = 0;
                currentByteIndex = 0;
                lastbit = 0;
                fwrite(linebuffer, sizeof(char),  linebufferindex, output);
                fwrite("\n", sizeof(char), 1, output);
                currentNode = root;
                for(int j=0;j<linebufferindex;j++){
                    linebuffer[j] = 0;
                }
                linebufferindex = 0;
            } else {
            currentByte=buffer[i];

            int bitIndex = 7;
            while(bitIndex>= 0){
                if((currentByte>>bitIndex) & 1){
                    currentNode = currentNode->right;
                }else{
                    currentNode = currentNode->left;
                }
                if(currentNode->value != 0){
                    linebuffer[linebufferindex] = currentNode->value;
                    linebufferindex++;
                    currentNode = root;
                }
                bitIndex--;
            }
                currentByteIndex++;
            }

        }
    }

    fclose(input);
    fclose(output);
    free(buffer);
    free(linebuffer);
    return 0;
}

node* readHeader(FILE *input) {
    node *root =(node*) malloc( sizeof(node));
    root->value = 0;
    root->left = NULL;
    root->right = NULL;
    root->depth = 0;
    root->frequency = 0;
    root->prefix = NULL;

    int headerSize =0;
    fread(&headerSize, sizeof(char), 2, input);
    printf("headerSize %d\n", headerSize);
    while(headerSize > 0){
        char token=0;
        fread(&token, sizeof(char), 1, input);
        char prefixlength = 0;
        fread(&prefixlength, sizeof(char), 1, input);
        int prefixbytes = (prefixlength+7) / 8;
        long prefix=0;
        printf("prefixbytes %d\n", prefixbytes);
        fread(&prefix, sizeof(char), prefixbytes, input);
        printf("char %c prefix %ld\n", token,prefix);

        int bitindex = (prefixbytes*8)-1;
        node* current = root;

        while(prefixlength >0){
            node* newNode = (node*) malloc(sizeof(node));

            if(prefix>>bitindex & 1){
                if(current->right != NULL){
                    current = current->right;
                    bitindex--;
                    prefixlength--;
                    free(newNode);
                    continue;
                }
                current->right = newNode;
            }else{
                if(current->left != NULL){
                    current = current->left;
                    bitindex--;
                    prefixlength--;
                    free(newNode);
                    continue;
                }
                current->left = newNode;
            }
            if(prefixlength==1){
                newNode->value = token;
            }else{
                newNode->value = 0;
            }
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->depth = current->depth+1;
            newNode->frequency = 0;
            newNode->prefix = NULL;

            current = newNode;
            bitindex--;
            prefixlength--;
        }
        //printf("token %c %d length %d\n", token,prefix, prefixlength);
        headerSize--;
    }
    return root;
}
