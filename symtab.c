/*************************************************/
/* File: symtab.c                                */
/* Symbol table implementation for tiny compiler */
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

#define SIZE 211

#define SHIFT 4

/* The hash function */
int hash(char* key) 
{
    int temp = 0;
    int i = 0;
    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }
    return temp;
}

/* list of line numbers of the source file
 * in which a variable is referenced
 */

typedef struct LineListRec {
    int lineno;
    struct LineListRec *next;
    struct LineListRec *rear;
} * LineList;


typedef struct BucketListRec {
    char *name;
    LineList lines;
    int memloc;
    struct BucketListRec *next;
} * BucketList;
/* The hash table */
static BucketList hashTable[SIZE];


void st_insert(char* name, int lineno, int loc)
{
    int h = hash(name);
    BucketList l = hashTable[h];
    while (l != NULL && strcmp(l->name, name) != 0) 
        l = l->next;
    if (l == NULL) {  // Variable not in symbol table
        l = (BucketList) malloc(sizeof (struct BucketListRec));
        l->name = name;
        l->lines = (LineList) malloc(sizeof (struct LineListRec));
        l->lines->lineno = lineno;
        l->lines->next = NULL;
        //l->lines->rear = l->lines;
        l->memloc = loc;
        l->next = hashTable[h]; // Add new id record to the front of bucket list
        hashTable[h] = l;
    } else {
        /*
        LineList rear = l->lines->rear; 
        rear->next = (LineList) malloc(sizeof (struct LineListRec));
        rear->next->lineno = lineno;
        rear = rear->next;
        rear->next = NULL;
        */
        LineList t = l->lines;
        while (t->next != NULL) t = t->next;
        t->next = (LineList) malloc(sizeof (struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;
        
    }
}

int st_lookup(char* name)
{
    int h = hash(name);
    BucketList l = hashTable[h];
    while (l != NULL && strcmp(l->name, name) != 0)
        l = l->next;
    if (l == NULL) return -1;
    else return l->memloc;
}

void printSymtab(FILE* listing)
{
    int i;
    fprintf(listing, "Variable Name    Location    Line Numbers\n");
    fprintf(listing, "-------------    --------    ------------\n");
    for (i = 0; i < SIZE; ++i) {
        if (hashTable[i] != NULL) {
            BucketList l = hashTable[i];
            while (l != NULL) {
                LineList t = l->lines;
                fprintf(listing, "%-14s ", l->name);
                fprintf(listing, "%-8d ", l->memloc);
                while (t != NULL) {
                    fprintf(listing, "%4d ", t->lineno);
                    t = t->next;
                }
                fprintf(listing, "\n");
                l = l->next;
            }
        }
    }
}
