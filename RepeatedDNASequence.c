#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "HashTableLinkedListCollision.h"

char ** findRepeatedDnaSequences(char * s, int* returnSize);

int main(){
    int returnSize = 0;
    char* DNAString = {"AAAAAAGTAATTTTTAAATTTTTCCCCCGTAAAAGTAATTTTTTCCCTTTTT"};
    char** arrs = findRepeatedDnaSequences(DNAString, &returnSize);
    printf("[");
    for(int i=0; i<returnSize; i++){
        printf("%s", arrs[i]);
        free(arrs[i]);
        arrs[i] = NULL;
        if(i+1 < returnSize){
            printf("], [");
        }
    }
    printf("]\n");
    return 0;
}

char ** findRepeatedDnaSequences(char * s, int* returnSize){
    if(strlen(s) < SEQUENCE_SIZE){
        *returnSize = 0;
        return NULL;
    }
    unsigned int lastID = encode(s) >> 2;
    char* it = s;
    Table t = {.table = {NULL},.uniquesHead = NULL,.uniquesTail = NULL,.numElements = 0};
    while(*(it+(SEQUENCE_SIZE-1)) != '\0'){
        Sequence* seq = parseSequence(it, lastID);
        lastID = seq->id;
        addSequence(&t, seq);
        it++;
    }
    
    if(t.numElements == 0){
        delete(&t);
        *returnSize = 0;
        return NULL;
    }else{
        char** ret = (char**)malloc(sizeof(char*)*t.numElements);
        *returnSize = loadSequences(&t, ret);
        delete(&t);
        return ret;
    }
}