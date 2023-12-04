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

    FILE *output = fopen(outputFile, "w+");
    if(output == NULL){
        printf("Error opening file %s\n", outputFile);
        return 1;
    }

    //longest possible line is 45354+3 bytes
    char* buffer = (char*) malloc(sizeof(char)*45354);
    char * linebuffer = (char*) malloc(sizeof(char) * 45354);

    char* linesizeBuffer = (char*) malloc(sizeof(char)*3);
    unsigned char lastbit;
    char inFile =1;

    while(inFile ==1 ){

        if(fread(linesizeBuffer,sizeof(char),3,input)<3){
            //end of file
            inFile=0;
        }else{
            //succes
            unsigned char linesize=0;
            linesize |= linesizeBuffer[0];
            linesize <<= 8;
            linesize |= linesizeBuffer[1];
            linesize <<= 8;
            linesize |= linesizeBuffer[2];
            lastbit=(linesize&0b111);
            linesize>>=3;

            //read all bytes of line
            fread(buffer,sizeof (char),linesize,input);
            //read all normal characters out of normal bytes
            node* current = root;
            int linebufferIndex = 0;
            for(int i=0; i<linesize-1;i++){
                char currentbyte = buffer[i];
                for(int j=7;j>=0;j--){
                    if(currentbyte>>j & 1){
                        current = current->right;
                    }else{
                        current = current->left;
                    }
                    if(current->value != 0){
                        linebuffer[linebufferIndex] = current->value;
                        linebufferIndex++;
                        current = root;
                    }
                }
            }
            //read last byte with lastbit
            char currentbyte = buffer[linesize-1];
            if(lastbit == 0){
                lastbit = 8;
            }
            for(int j=7;j>=8-lastbit;j--){
                if(currentbyte>>j & 1){
                    current = current->right;
                }else{
                    current = current->left;
                }
                if(current->value != 0){
                    linebuffer[linebufferIndex] = current->value;
                    linebufferIndex++;
                    current = root;
                }
            }
            fwrite(linebuffer, sizeof(char), linebufferIndex, output);
            fwrite("\n", sizeof(char), 1, output);
            for(int j = 0; j < linebufferIndex; j++){
                linebuffer[j] = 0;
            }
            for(int j = 0; j<linesize; j++){
                buffer[j] = 0;
            }
        }
    }

    free(linesizeBuffer);
    fclose(input);
    fclose(output);
    free(buffer);
    free(linebuffer);
    freeTree(root);
    return 0;
}

int freeTree(node *root) {
    if(root->left != NULL){
        freeTree(root->left);
    }
    if(root->right != NULL){
        freeTree(root->right);
    }
    free(root);
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
