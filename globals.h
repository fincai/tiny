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

typedef enum {
    ENDFILE, ERROR,
    /* reserved words */
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,
    /* multicharacter tokens */
    ID, NUM,
    /* special symbols */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI
} TokenType;

extern FILE* source;
extern FILE* listing;

extern int lineno;


/******************************************/
/******** Syntax tree for parsing *********/
/******************************************/












/******************************************/
/******** Flags for tracing *********/
/******************************************/

extern int EchoSource;

extern int TraceScan;

extern int Error;

#endif
