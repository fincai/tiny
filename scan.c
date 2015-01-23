/****************************************************/
/* File: scan.c                                     */
/* The scanner implemenation for the tiny compiler  */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"

typedef enum {
    START, INNUM, INID, INASSIGN, INCOMMENT, DONE
} StateType;

char tokenString[MAXTOKENLEN+1];

static int buflen = 256;

static char *lineBuf = NULL;
static int linepos = 0;
static int bufsize = 0;

static char getNextChar() 
{
    if (! (linepos < bufsize)) {
        char c;
        if (lineBuf) {
            free(lineBuf);
            lineBuf = NULL;
            bufsize = 0;
        }
        while ((c = fgetc(source)) != '\n' && c != EOF) {
           lineBuf = realloc(lineBuf, bufsize + 1);
           lineBuf[bufsize] = c;
           ++bufsize;
        }
        if (c == '\n') {
            lineBuf = realloc(lineBuf, bufsize+2);
            lineBuf[bufsize] = '\n';
            lineBuf[++bufsize] = '\0';
            linepos = 0;
            ++lineno;
            if (EchoSource) fprintf(listing, "%d: %s", lineno, lineBuf);
            return lineBuf[linepos++];
        } else {   /* c == EOF */
            if (lineBuf) free(lineBuf);
            return EOF;
        }
    } else
        return lineBuf[linepos++];
}

static void ungetNextChar() 
{ --linepos; }

/* lookup table of reserved words */
static struct {
    char* str;
    TokenType tok;
} reservedWords[MAXRESERVED] = {
    {"if", IF}, {"then",THEN}, {"else", ELSE},
    {"end", END}, {"repeat", REPEAT}, {"until", UNTIL},
    {"read", READ}, {"write", WRITE}
};

TokenType reservedLookup(char* s)
{
    int i;
    for (i = 0; i < MAXRESERVED; ++i)
        if (!strcmp(s, reservedWords[i].str))
            return reservedWords[i].tok;
    return ID;
}

/* returns next token in the source file */
TokenType getToken() {
    int tokenStringIndex = 0;
    TokenType currentToken;
    int save, comtLine;
    StateType state = START;
    while (state != DONE) {
        save = TRUE;
        char c = getNextChar();
        switch (state) {
            case START:
                if (isdigit(c)) 
                    state = INNUM;
                else if (isalpha(c)) 
                    state = INID;
                else if (c == ':')
                    state = INASSIGN;
                else if (c == ' ' || c == '\t' || c == '\n')
                    save = FALSE;
                else if (c == '{') {
                    comtLine = lineno;
                    save = FALSE; 
                    state = INCOMMENT;
                } else {
                    state = DONE;
                    switch (c) {
                        case EOF:
                            return ENDFILE;
                        case '=':
                            currentToken = EQ;
                            break;
                        case '<':
                            currentToken = LT;
                            break;
                        case '+':
                            currentToken = PLUS;
                            break;
                        case '-':
                            currentToken = MINUS;
                            break;
                        case '*':
                            currentToken = TIMES;
                            break;
                        case '/':
                            currentToken = OVER;
                            break;
                        case '(':
                            currentToken = LPAREN;
                            break;
                        case ')':
                            currentToken = RPAREN;
                            break;
                        case ';':
                            currentToken = SEMI;
                            break;
                        default:
                            currentToken = ERROR;
                            break;
                    }
                }
                break;  // break START
            case INNUM:
                if (!isdigit(c)) {
                   save = FALSE;
                   ungetNextChar();
                   state = DONE;
                   currentToken = NUM;
                }
                break;
            case INID:
                if (!isalpha(c)) {
                    save = FALSE;
                    ungetNextChar();
                    state = DONE;
                    currentToken = ID;
                }
                break;
            case INASSIGN:
                state = DONE;
                if (c == '=') 
                    currentToken = ASSIGN;
                else {
                    save = FALSE; 
                    ungetNextChar();
                    currentToken = ERROR;
                }
                break;
            case INCOMMENT:
                save = FALSE;
                if (c == '}')
                    state = START;
                if (c == EOF) {
                    fprintf(listing, "Unterminated comment at line %d\n", comtLine);
                    return ENDFILE;
                }
                break;
            case DONE:
            default:  // should never happen
                fprintf(listing, "Scanner Bug: state = %d\n", state);
                state = DONE;
                currentToken = ERROR;
                break;
        }
        if (save && tokenStringIndex < MAXRESERVED)
            tokenString[tokenStringIndex++] = c;
        if (state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    } // end while
    if (TraceScan) {
        fprintf(listing, "\t%d:  ", lineno);
        printToken(currentToken, tokenString);
    }
    return currentToken;
}
