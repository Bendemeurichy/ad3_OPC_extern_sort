//
// Created by bendm on 21/10/23.
//

#include "sort.h"
#include <string.h>
#include <stdlib.h>
#include "sorting_linked_list.h"

#define MAXLINELENGTH 45357

int sort(char *inputFile, char *outputFile, int bufferSize) {
    FILE *input = fopen(inputFile, "rb");
    if (input == NULL) {
        printf("Error opening file %s\n", inputFile);
        return 1;
    }

    FILE *output = fopen(outputFile, "wb");
    if (output == NULL) {
        printf("Error opening file %s\n", outputFile);
        return 1;
    }
    //write header to output file and skip it for sorting
    uint16_t headersize = 0;

    fread(&headersize, sizeof(uint16_t), 1, input);
    fwrite(&headersize, sizeof(uint16_t), 1, output);
    while(headersize > 0){
        uint8_t header[10];
        uint8_t token;
        fread(&token, sizeof(uint8_t), 1, input);
        header[0] = token;
        uint8_t size;
        fread(&size, sizeof(uint8_t), 1, input);
        header[1] = size;
        uint8_t bytesUsed = (size+7)/8;
        fread(header+2, sizeof(uint8_t), bytesUsed, input);
        fwrite(header, sizeof(uint8_t), bytesUsed+2, output);
        headersize--;
    }

    //basestep: write blocks to temp files
    int tempfiles = 0;
    tempfiles = blockSort(input, bufferSize);

    //merge sorted blocks
    while (tempfiles>1){
        tempfiles = mergeFiles(tempfiles, bufferSize);
    }

    //write sorted file to output
    FILE *lasttemp = fopen("temp0.txt", "rb");
    if(lasttemp == NULL){
        printf("Error opening temp file\n");
        return 1;
    }

    uint8_t linesizeBuffer[3];
    uint16_t linesize=0;
    while (fread(linesizeBuffer, sizeof(uint8_t), 3, lasttemp) == 3) {
        linesize |= linesizeBuffer[1];
        linesize <<= 8;
        linesize |= linesizeBuffer[2];
        linesize >>= 3;
        fwrite(linesizeBuffer, sizeof(uint8_t), 3, output);
        uint8_t line[linesize];
        fread(line, sizeof(uint8_t), linesize, lasttemp);
        fwrite(line, sizeof(uint8_t), linesize, output);
        linesize = 0;
        for (int i = 0; i < 3; i++) {
            linesizeBuffer[i] = 0;
        }
    }


    fclose(lasttemp);
    fclose(input);
    fclose(output);
    remove("temp0.txt");

    return 0;
}

int freeLines(uint8_t **lines, long size) {
    for (long i = 0; i < size; i++) {
        free(lines[i]);
    }
    free(lines);
    return 0;
}

int blockSort(FILE *input, int bufferSize){
    int tempcount = 0;
    uint8_t linesizeBuffer[3];
    uint16_t linesize=0;
    uint8_t **lines= (uint8_t **) malloc(sizeof(uint8_t *));
    if(lines == NULL){
        printf("Error allocating memory\n");
        return 1;
    }
    long bufferIndex = 0;
    long buffersizeUsed = 0;
    //lastbit is not necessary for this function but we need to write it to the output file for extraction so it goes in the linebuffer
    while (fread(linesizeBuffer, sizeof(uint8_t), 3, input) == 3) {
        linesize |= linesizeBuffer[1];
        linesize <<= 8;
        linesize |= linesizeBuffer[2];
        linesize >>= 3;

        if ((buffersizeUsed + linesize + 3) < bufferSize) {
            //add line to buffer
            lines[bufferIndex] = (uint8_t *) malloc(sizeof(uint8_t) * (linesize + 3));
            if(lines[bufferIndex] == NULL){
                printf("Error allocating memory\n");
                return 1;
            }

            lines[bufferIndex][0] = linesizeBuffer[0];
            lines[bufferIndex][1] = linesizeBuffer[1];
            lines[bufferIndex][2] = linesizeBuffer[2];
            fread(lines[bufferIndex]+3, sizeof(uint8_t), linesize, input);

            bufferIndex++;
            uint8_t **temp = realloc(lines, sizeof(uint8_t *) * (bufferIndex+1));
            if (temp == NULL) {
                printf("Error reallocating memory\n");
                freeLines(lines, bufferIndex);
                return 1;
            }else {
                lines = temp;
            }
            //free(temp);
            buffersizeUsed += linesize + 3;

        } else {
            //sort and write buffer to temp file
            qsort(lines, bufferIndex, sizeof(uint8_t *), compareLines);
            char tempname[20];
            snprintf(tempname,20,"temp%d.txt", tempcount);
            FILE *temp = fopen(tempname, "wb");
            if(temp == NULL){
                printf("Error opening temp file\n");
                return 1;
            }
            for (int i = 0; i < bufferIndex; i++) {

                fwrite(lines[i], sizeof(uint8_t), (linesize>>3)+2, temp);
            }
            fclose(temp);
            bufferIndex = 0;
            buffersizeUsed = 0;
            tempcount++;
            freeLines(lines, bufferIndex);
            lines = (uint8_t **) malloc(sizeof(uint8_t *));

            lines[bufferIndex] = (uint8_t *) malloc(sizeof(uint8_t) * (linesize + 3));
            if(lines[bufferIndex] == NULL){
                printf("Error allocating memory\n");
                return 1;
            }
            fread(lines[bufferIndex]+3, sizeof(uint8_t), linesize, input);
            lines[bufferIndex][0] = linesizeBuffer[0];
            lines[bufferIndex][1] = linesizeBuffer[1];
            lines[bufferIndex][2] = linesizeBuffer[2];
            bufferIndex++;

            uint8_t **temp_lines = realloc(lines, sizeof(uint8_t *) * (bufferIndex+1));
            if (temp_lines == NULL) {
                printf("Error reallocating memory\n");
                freeLines(lines, bufferIndex);
                return 1;
            }else {
                lines = temp_lines;
            }
            buffersizeUsed += linesize + 3;
        }
        linesize = 0;
        for (int i = 0; i < 3; i++) {
            linesizeBuffer[i] = 0;
        }

    }
    //write last buffer to temp file
    if(bufferIndex >0){
        qsort(lines, bufferIndex, sizeof(uint8_t *), compareLines);
        char tempname[20];
        snprintf(tempname,20,"temp%d.txt", tempcount);
        FILE *temp = fopen(tempname, "wb");
        if(temp == NULL){
            printf("Error opening temp file\n");
            return 1;
        }
        for (int i = 0; i < bufferIndex; i++) {
            int currentlinesize = lines[i][1] << 8;
            currentlinesize |= lines[i][2];
            fwrite(lines[i], sizeof(uint8_t), (currentlinesize>>3)+3, temp);
        }
        fclose(temp);
        buffersizeUsed = 0;
    }
    freeLines(lines, bufferIndex);
    return tempcount;
}



int compareLines(const void *a, const void *b){
    uint8_t *pa = *(uint8_t **) a;
    uint8_t *pb = *(uint8_t **) b;
    uint32_t sizea = pa[0] << 8;
    sizea |= pa[1];
    sizea<<=8;
    sizea |= pa[2];
    sizea >>= 3;
    uint32_t sizeb = pb[0] << 8;
    sizeb |= pb[1];
    sizeb<<=8;
    sizeb |= pb[2];
    sizeb >>= 3;

    return memcmp(pa + 3,pb + 3,sizea < sizeb ? sizea : sizeb);
}

int mergeFiles(int tempCount,int bufferSize){
    int tempfiles = 0;
    int tempfilesOneStep = bufferSize/MAXLINELENGTH;
    tempfiles = tempCount/tempfilesOneStep;
    for(int i=0;i<tempfiles;i++){
        int endCondition = 0;
        if(i == tempfiles-1){
            endCondition = tempCount%tempfilesOneStep;
        } else {
            endCondition = tempfilesOneStep;
        }

        temp_file_list * temp_list = create_temp_list();

        for(int j=i*tempfilesOneStep;j<(i*tempfilesOneStep)+endCondition;j++) {
            char tempname[20];
            snprintf(tempname, 20, "temp%d.txt", tempfiles);
            FILE *temp = fopen(tempname, "rb");
            if (temp == NULL) {
                printf("Error opening temp file\n");
                return 1;
            }
            add_temp_file(temp_list, tempname, temp);
        }

        char largerTempName[20];
        snprintf(largerTempName,20,"temp%d.txt", i);
        FILE *largerTemp = fopen(largerTempName, "wb");
        if(largerTemp == NULL){
            printf("Error opening temp file\n");
            return 1;
        }

        while(temp_list->size > 1){
            int bufferIndex = 0;
            uint8_t ** lines =(uint8_t **) malloc(sizeof(uint8_t *)*temp_list->size);
            temp_file * current = temp_list->firstNode;
            while(current->next!=NULL){
                uint8_t linesizeBuffer[3];
                uint16_t linesize=0;
                if(fread(linesizeBuffer, sizeof(uint8_t), 3, current->file)<3){
                    remove_temp_file(temp_list, current->name);
                } else {
                    linesize |= linesizeBuffer[1];
                    linesize <<= 8;
                    linesize |= linesizeBuffer[2];
                    linesize >>= 3;
                    lines[bufferIndex] = (uint8_t *) malloc(sizeof(uint8_t) * linesize + 3);
                    lines[bufferIndex][0] = linesizeBuffer[0];
                    lines[bufferIndex][1] = linesizeBuffer[1];
                    lines[bufferIndex][2] = linesizeBuffer[2];
                    fread(lines[bufferIndex] + 3, sizeof(uint8_t), linesize, current->file);
                    current = current->next;
                    bufferIndex++;
                }
            }
            qsort(lines, bufferIndex, sizeof(uint8_t *), compareLines);

            for (int j = 0; j < bufferIndex; j++) {
                fwrite(lines[j], sizeof(uint8_t), ((lines[j][1]<<8|lines[j][2])>>3)+3, largerTemp);
            }

            freeLines(lines, bufferIndex);

            bufferIndex = 0;


        }
        remove_temp_file_list(temp_list);

        char tempname[20];
        snprintf(tempname,20,"temp%d.txt", i);
        fclose(largerTemp);
        rename(largerTempName, tempname);


    }




    return tempfiles;
}