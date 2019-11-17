#ifndef HASHTABLELINKEDLISTCOLLISION_H
#define HASHTABLELINKEDLISTCOLLISION_H

#define SEQUENCE_SIZE 10
#define TABLE_SIZE 50000


typedef struct Sequence Sequence;
struct Sequence{
    unsigned int id:20;
    unsigned int count;
    char sequence[SEQUENCE_SIZE + 1];
    Sequence* nextTable;
    Sequence* nextList;
};

typedef struct Table{
    Sequence* table[TABLE_SIZE];
    Sequence* uniquesHead;
    Sequence* uniquesTail;
    int numElements;
}Table;

unsigned int getNextID(char s, unsigned int lastID);
unsigned int hashCode(Sequence* seq);
unsigned int encode(char* strToEncode);
void decode(unsigned int id, char* ret);
void addSequence(Table* t, Sequence* seq);
void printHashTable(Table* t);
void increment(Table* t, Sequence* seq);
void addSeq(Table* t, Sequence* seq);
void addList(Table* t, Sequence* seq);
void addTable(Table* t, Sequence* seq);
void printLinkedList(Table* t);
void printHashTable(Table* t);
Sequence* parseSequence(char *it, unsigned int lastID);
_Bool contains(Table* t, Sequence* seq);
void delete(Table* t);
int loadSequences(Table* t, char** ret);

#endif