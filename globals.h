/*******************************************/
/* File: globals.h                         */
/* Global types and vars for tiny compiler */
/* must come before other include files    */
/*******************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* The number of reserved words */
#define MAXRESERVED 8

#define NO_BISON TRUE 

#if NO_BISON
typedef enum {
    ERROR, ENDFILE,
    /* reserved words */
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    /* multicharacter tokens */
    ID, NUM,
    /* special symbols */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
} TokenType;
#else
typedef int TokenType;
#ifndef IN_BISON_PARSER
#include "parser.tab.h"
#define ENDFILE 0
#endif
#endif




extern FILE* source;
extern FILE* listing;

extern int lineno;


/******************************************/
/******** Syntax tree for parsing *********/
/******************************************/

typedef enum { StmtK, ExpK } NodeKind;
typedef enum { IfK, RepeatK, AssignK, ReadK, WriteK } StmtKind;
typedef enum { OpK, ConstK, IdK } ExpKind;

/* ExpType is used for type checking */
typedef enum { Void, Integer, Boolean } ExpType;

#define MAXCHILDREN 3

typedef struct treeNode {
    struct treeNode* child[MAXCHILDREN];
    struct treeNode* sibling;
    int lineno;
    NodeKind nodekind;
    union { StmtKind stmt; ExpKind exp; } kind;
    union { TokenType op;
            int val;
            char* name;
          } attr;
    ExpType type;
} TreeNode;


/******************************************/
/******** Flags for tracing *********/
/******************************************/

extern int EchoSource;

extern int TraceScan;

extern int TraceParse;

extern int Error;

#endif
