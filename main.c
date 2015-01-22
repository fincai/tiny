/********************************************/
/* File: main.c                             */
/* Main program for the tiny compiler       */
/********************************************/

#include "globals.h"

#define NO_PARSE TRUE
#define NO_ANALYZE TRUE
#define NO_CODEGEN TRUE 
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODEGEN
#include "codegen.h"
#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE *source;
FILE *listing;


/* allocate tracing flags */
int EchoSource = TRUE;
int TraceScan = TRUE;

int Error = FALSE;

int main(int argc, char *argv[ ])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]); 
        exit(1);
    }
    char *pgm = malloc((strlen(argv[1]) + 5) * sizeof(char));
    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".tny");
    source = fopen(pgm, "r");
    if (source == NULL) {
        fprintf(stderr, "File %s not found!\n", pgm);
        exit(1);
    }
    listing = stdout;   /* send listing to screen */
    fprintf(listing, "\n TINY COMPILATION: %s\n", pgm);
    free(pgm);

#if NO_PARSE
    while (getToken() != ENDFILE) ;
#endif
    
    return 0;    
}



