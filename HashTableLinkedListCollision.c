#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "HashTableLinkedListCollision.h"


void addSequence(Table* t, Sequence* seq){
    if(contains(t, seq)){
        increment(t, seq);
    }else{
        addSeq(t, seq);
    }
}

void increment(Table* t, Sequence* seq){
    int index = hashCode(seq);
    Sequence* head = t->table[index];
    while(head != NULL){
        if(head->id == seq->id){
            head->count++;
            free(seq);
            seq = NULL;
            break;
        }else{
            head = head->nextTable;
        }
    }
}

bool contains(Table* t, Sequence* seq){
    int index = hashCode(seq);
    if(t->table[index] != NULL){
        Sequence* head = t->table[index];
        while(head != NULL){
            if(head->id == seq->id){
                return true;
            }
            head = head->nextTable;
        }
    }
    return false;
}

unsigned int hashCode(Sequence* seq){
    return seq->id % TABLE_SIZE;
}

void addSeq(Table* t, Sequence* seq){
    t->numElements++;
    addTable(t, seq);
    addList(t, seq);
}

void addList(Table* t, Sequence* seq){
    int index = hashCode(seq);
    if(t->uniquesHead == NULL){
        t->uniquesHead = seq;
        t->uniquesTail = seq;
    }else{
        t->uniquesTail->nextList = seq;
        t->uniquesTail = seq;
    }
}

void addTable(Table* t, Sequence* seq){
    int index = hashCode(seq);
    if(t->table[index] == NULL){
        t->table[index] = seq;
    }else{
        Sequence* head = t->table[index];
        while(head->nextTable != NULL){
            head = head->nextTable;
        }
        head->nextTable = seq;
    }
}

Sequence* parseSequence(char* it, unsigned int lastID){
    Sequence* seq = (Sequence*)malloc(sizeof(Sequence));
    //printf("adding *it: %c...", *it);
    seq->id = getNextID(*(it+(SEQUENCE_SIZE-1)), lastID);
    seq->count = 1;
    seq->sequence[SEQUENCE_SIZE] = '\0';
    strncpy(seq->sequence, it, SEQUENCE_SIZE);
    seq->nextTable = NULL;
    seq->nextList = NULL;
    return seq;
}

void printTable(Table* t){
    printHashTable(t);
    printf("\n");
    printLinkedList(t);
}

void printHashTable(Table* t){
    for(int i=0; i<TABLE_SIZE; i++){
        Sequence* head = t->table[i];
        printf("Index %d: ", i);
        while(head != NULL){
            printf("Sequence: %s, ID: %d, Count: %d -> ", head->sequence, head->id, head->count);
            head = head->nextTable;
        }
        printf("\n");
    }
}

void printLinkedList(Table* t){
    Sequence* head = t->uniquesHead;
    if(t->uniquesTail == NULL){
        printf("tail null\n");
    }
    while(head != t->uniquesTail){
        printf("Sequence: %s, ID: %d, Count: %d ->\n", head->sequence, head->id, head->count);
        head = head->nextList;
    }
}

void unlinkTableSequence(Sequence* seq){
    if(seq->nextTable != NULL){
        unlinkTableSequence(seq->nextTable);
        seq->nextTable = NULL;
    }
    return;
}

void deleteTable(Table* t){
    for(int i=0; i< TABLE_SIZE; i++){
        if(t->table[i] != NULL){
            unlinkTableSequence(t->table[i]);
            t->table[i] = NULL;
        }
    }
}

void deleteLinkedList(Table* t){
    while(t->uniquesHead != t->uniquesTail){
        t->numElements--;
        Sequence* temp = t->uniquesHead;
        t->uniquesHead = t->uniquesHead->nextList;
        temp->nextTable = NULL;
        temp->nextList = NULL;
        temp->count = 0;
        temp->id = 0;
        free(temp);
    }
}

void delete(Table* t){
    deleteTable(t);
    deleteLinkedList(t);
    t->numElements = 0;
    t->numElements = 0;
    t->uniquesTail = NULL;
    t->uniquesHead = NULL;
}

int loadSequences(Table* t, char** ret){
    Sequence* head = t->uniquesHead;
    int size = 0;
    while(head != NULL){
        if(head->count > 1){
            ret[size] = (char*)malloc(sizeof(char)*(SEQUENCE_SIZE+1));
            ret[size][SEQUENCE_SIZE] = '\0';
            strncpy(ret[size], head->sequence, SEQUENCE_SIZE);
            size++;
        }
        head = head->nextList;
    }
    return size;
}

unsigned int encode(char* strToEncode){
    unsigned int id = 0;
    unsigned int letterCode = 0;
    unsigned int mask = (1 << 20) - 1;
    for(int i=0; i < SEQUENCE_SIZE; i++){
        if(*(strToEncode+i) == 'A'){
            letterCode = 0;
        }else if(strToEncode[i] == 'C'){
            letterCode = 1;
        }else if(strToEncode[i] == 'G'){
            letterCode = 2;
        }else if(strToEncode[i] == 'T'){
            letterCode = 3;
        }
        id = (id << 2) | letterCode;
    }
    return id & mask;
}

unsigned int getNextID(char s, unsigned int lastID){
    unsigned int newID = 0;
    unsigned int mask = (1 << 20) - 1;
    if(s == 'A'){
        newID = 0;
    }else if(s == 'C'){
        newID = 1;
    }else if(s == 'G'){
        newID = 2;
    }else if(s == 'T'){
        newID = 3;
    }
    unsigned int ret = ((lastID << 2) | newID) & mask;
    return ret;
}

void decode(unsigned int id, char* ret){
    ret[SEQUENCE_SIZE] = '\0';
    for(int i = 0; i<SEQUENCE_SIZE; i++){
        unsigned int mask = 3;
        unsigned int letter = mask & id;
        if(letter == 0){
            ret[SEQUENCE_SIZE - i - 1] = 'A';
        }else if(letter == 1){
            ret[SEQUENCE_SIZE - i - 1] = 'C';
        }else if(letter == 2){
            ret[SEQUENCE_SIZE - i - 1] = 'G';
        }else if(letter == 3){
            ret[SEQUENCE_SIZE - i - 1] = 'T';
        }
        id = id>>2;
    }
    return;
}