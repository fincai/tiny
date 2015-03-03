/********************************************/
/* File: symtab.h                           */
/* Symbol table interface for tiny compiler */
/********************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

void st_insert(char* name, int lineno, int loc);

int st_lookup(char* name);

void printSymtab(FILE *listing);


#endif
