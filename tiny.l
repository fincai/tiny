%{
#include "globals.h"
#include "util.h"
#include "scan.h"

static int comtLine;

char tokenString[MAXTOKENLEN+1];

%}

digit [0-9]
number {digit}+
letter [a-zA-Z]
identifier {letter}+
newline \n
whitespace [ \t]+
quote {

%x comment
%%

"{" { comtLine = lineno;  BEGIN(comment); }
<comment>[^}\n]* 
<comment>\n  ++lineno;
<comment>"}" BEGIN(INITIAL);
<comment><<EOF>> { fprintf(yyout, "unterminated comment at line %d.\n", comtLine); return ENDFILE; }


"if" { return IF; }
"then" { return THEN; }
"else" { return ELSE; }
"end" { return END; }
"repeat" { return REPEAT; }
"until" { return UNTIL; }
"read" { return READ; }
"write" { return WRITE; }
":=" { return ASSIGN; }

"=" { return EQ; }
"<" { return LT; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return TIMES; }
"/" { return OVER; }
"(" { return LPAREN; }
")" { return RPAREN; }
";" { return SEMI; }

{number} { return NUM; }
{identifier} { return ID; }
{newline} { ++lineno; }
{whitespace} {/* skip whitespaces */}


<<EOF>> { return ENDFILE; }

. { return ERROR; }

%%

TokenType getToken()
{
    static int firstTime = TRUE;
    TokenType currentToken;
    if (firstTime) {
        yyin = source;
        yyout = listing;
        firstTime = FALSE;
        ++lineno;
    }
    currentToken = yylex();
    if (currentToken == ENDFILE) return ENDFILE;
    strncpy(tokenString, yytext, MAXTOKENLEN);
    if (TraceScan) {
        fprintf(yyout, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }
    return currentToken; 
}
