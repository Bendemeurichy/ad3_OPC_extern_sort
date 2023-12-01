//
// Created by bendm on 21/10/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

//compress every line without newlines, just save line length in begin of each lines first 3bytes
int compress(char *inputFile, char *outputFile, int bufferSize, char *prefixCodingFile){
    FILE* input = fopen(inputFile, "r");
    if(input == NULL){
        printf("Error opening file %s\n", inputFile);
        return 1;
    }

    FILE* output = fopen(outputFile, "w+b");
    if(output == NULL){
        printf("Error opening file %s\n", outputFile);
        return 1;
    }

    FILE* prefixCoding = fopen(prefixCodingFile, "r");
    if(prefixCoding == NULL){
        printf("Error opening file %s\n", prefixCodingFile);
        return 1;
    }

    // Read prefix coding file
    char* prefixes[127]={NULL};

    char *line = NULL;
    size_t len = 0;
    // Read the file line by line
    while (getline(&line, &len, prefixCoding) != -1) {
        char *token = strtok(line, " "); // Using space as the delimiter

        // get the character
        int value = atoi(token);

        // Skip the second value
        strtok(NULL, " ");
        token = strtok(NULL, " ");

        // Process the prefix (third value)
        char bitstring[64]; // the prefix will never be longer than 64 bits (opgave)
        if (token != NULL) {
            // Remove the  newline character on each line
            size_t prefixlength = strlen(token);
            if (prefixlength > 0 && token[prefixlength - 1] == '\n') {
                token[prefixlength - 1] = '\0'; // Remove the newline
            }
            strcpy(bitstring, token);
        }
        if (strcmp(bitstring,"")!=0) {
            prefixes[value] = strdup(bitstring);
        }
    }

    free(line);

    fclose(prefixCoding);

    // parse the input file and write encoding to output file
    char* buffer= (char*) malloc(bufferSize*sizeof(char));
    char byte=0;
    char bitcount=0;
    size_t bytesRead;
    long lineLength = 0;
    
    //make header by just assigning 4 bytes max per prefix
    char charAmount = 0;
    char* headerBuffer = (char*) malloc(((10*127)+2)*sizeof(char));
    int headerIndex = 2;

    for (char i = 0; i < 127; i++) {
        if (prefixes[i] != NULL) {
            headerBuffer[headerIndex] = i;
            headerIndex++;
            headerBuffer[headerIndex] = strlen(prefixes[i]);
            printf("prefix %d %d %s\n", i, strlen(prefixes[i]), prefixes[i]);
            headerIndex++;
            charAmount++;
            for (int j = 0; j < strlen(prefixes[i]); j++) {
                if (prefixes[i][j] == '1') {
                    byte |= (1 << (7 - bitcount));
                } else {
                    byte &= ~(1 << (7 - bitcount));
                }
                bitcount++;
                if (bitcount == 8) {
                    headerBuffer[headerIndex] = byte;
                    headerIndex++;
                    byte = 0;
                    bitcount = 0;
                }
            }
            if (bitcount != 0) {
                //byte = byte << (7 - bitcount);
                headerBuffer[headerIndex] = byte;
                headerIndex++;
                byte = 0;
                bitcount = 0;
            }
        }
    }
    headerBuffer[0] = charAmount&0xFF;
    charAmount = charAmount >> 8;
    headerBuffer[1]= charAmount&0xFF;
    //write header to file
    fwrite(headerBuffer, sizeof(char), headerIndex, output);
    free(headerBuffer);
    //works till here, header is written to file

    byte=0;
    bitcount=0;
    unsigned char* linebuffer = (char*) malloc(45357*sizeof(char));
    unsigned int linebufferIndex = 3;
    //read input file and write to output file
    while((bytesRead = fread(buffer, sizeof(char), bufferSize, input)) > 0){
        for(size_t i = 0; i < bytesRead; i++){
            if(buffer[i]=='\n'){
                if(bitcount != 0){
                    byte = byte;
                    linebuffer[linebufferIndex] = byte;
                    linebufferIndex++;
                    lineLength++;
                    byte = 0;
                }
                //write line length to file with first 19bits the size in bytes and the 3 bits after the remaining bits in the next byte
                //write in front of line 22 bits of 24 actually used
                unsigned int first3BytesOfLine = (lineLength& 0x7FFFF) <<3;
                first3BytesOfLine |= (bitcount & 0x7);


                linebuffer[0] = (first3BytesOfLine >> 16) & 0xFF;
                linebuffer[1] = (first3BytesOfLine >> 8) & 0xFF;
                linebuffer[2] = first3BytesOfLine & 0xFF;

                fwrite(linebuffer, sizeof (char),linebufferIndex, output);
                for(int j = 0; j < linebufferIndex; j++){
                    linebuffer[j] = 0;
                }
                linebufferIndex = 3;
                lineLength = 0;
                bitcount=0;
                byte=0;
            }
            char* prefix = prefixes[buffer[i]];
            if(prefix != NULL) {
                for (size_t j = 0; j < strlen(prefix); j++) {
                    if (prefix[j] == '1') {
                        byte |= (1 << (7 - bitcount));
                    } else {
                        byte &= ~(1 << (7 - bitcount));
                    }
                    bitcount++;
                    if (bitcount == 8) {
                        linebuffer[linebufferIndex] = byte;
                        byte = 0;
                        bitcount = 0;
                        lineLength++;
                        linebufferIndex++;
                    }
                }
            }
        }

    }

    if(bitcount != 0){
        byte = byte << (7 - bitcount);
        linebuffer[linebufferIndex] = byte;
        linebufferIndex++;
        lineLength++;
        int first3BytesOfLine = (lineLength& 0x7FFFF) <<3;
        first3BytesOfLine |= (bitcount & 0x7);

        linebuffer[0] = (first3BytesOfLine >> 16) & 0xFF;
        linebuffer[1] = (first3BytesOfLine >> 8) & 0xFF;
        linebuffer[2] = first3BytesOfLine & 0xFF;

        fwrite(linebuffer, sizeof (char),linebufferIndex, output);
    }
    //write last line length to file


    free(linebuffer);
    free(buffer);
    fclose(output);
    fclose(input);

    freePrefixes(prefixes, 127);
    return 0;
}

void freePrefixes(char* prefixes[], int size) {
    for(int i = 0; i < size; i++) {
        if(prefixes[i] != NULL) {
            free(prefixes[i]);
            prefixes[i] = NULL;
        }
    }
}
