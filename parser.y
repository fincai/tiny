%{
#define IN_BISON_PARSER 

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode*
static char* savedName;
static int savedLineno;
static TreeNode* savedTree;
static TreeNode* lastStmtNode;

static int firstReduction = FALSE;

%}

%token IF THEN ELSE END REPEAT UNTIL READ WRITE
%token ID NUM
%token ASSIGN EQ LT PLUS MINUS TIMES OVER LPAREN RPAREN SEMI
%token ERROR

%%

program : stmt_seq { savedTree = $1; }
        ;

stmt_seq: stmt_seq SEMI stmt 
          {
            YYSTYPE t = $1;
            if (t != NULL) {
                while (t->sibling != NULL)
                    t = t->sibling;
                t->sibling = $3;
                $$ = $1;
            } else $$ = $3;
          }
        | stmt { $$ = $1; }
        ;

stmt : if_stmt { $$ = $1; }
     | repeat_stmt { $$ = $1; } 
     | assign_stmt { $$ = $1; }
     | read_stmt { $$ = $1; }
     | write_stmt  { $$ = $1; }
     | error { $$ = NULL; }
     ;

if_stmt : IF exp THEN stmt_seq END
          {
            $$ = newStmtNode(IfK);
            $$->child[0] = $2;
            $$->child[1] = $4;
          }
        | IF exp THEN stmt_seq ELSE stmt_seq END
          {
            $$ = newStmtNode(IfK);
            $$->child[0] = $2;
            $$->child[1] = $4;
            $$->child[2] = $6;
          }
        ;

repeat_stmt : REPEAT stmt_seq UNTIL exp
              {
                $$ = newStmtNode(RepeatK);
                $$->child[0] = $2;
                $$->child[1] = $4;
              }
            ;

assign_stmt : ID { savedName = copyString(tokenString);
                  savedLineno = lineno; }
             ASSIGN exp
             {
                $$ = newStmtNode(AssignK);
                $$->attr.name = savedName;
                $$->lineno = lineno;
                $$->child[0] = $4; 
             }
           ;

read_stmt : READ ID 
          {
            $$ = newStmtNode(ReadK);
            $$->attr.name = copyString(tokenString);
          }
          ;

write_stmt : WRITE exp
           {
             $$ = newStmtNode(WriteK);
             $$->child[0] = $2;
           }
           ;

exp : simple_exp EQ simple_exp
    {
        $$ = newExpNode(OpK);
        $$->attr.op = EQ;
        $$->child[0] = $1;
        $$->child[1] = $3;
    }
    | simple_exp LT simple_exp
    {
        $$ = newExpNode(OpK);
        $$->attr.op = LT;
        $$->child[0] = $1;
        $$->child[1] = $3;
    }
    | simple_exp { $$ = $1; }
    ;

simple_exp : simple_exp PLUS term 
           {
             $$ = newExpNode(OpK);
             $$->attr.op = PLUS;
             $$->child[0] = $1;
             $$->child[1] = $3;
           }
           | simple_exp MINUS term
           {
             $$ = newExpNode(OpK);
             $$->attr.op = MINUS;
             $$->child[0] = $1;
             $$->child[1] = $3;
           }
           | term { $$ = $1; }
           ;

term : term TIMES factor
     {
       $$ = newExpNode(OpK);
       $$->attr.op = TIMES;
       $$->child[0] = $1;
       $$->child[1] = $3;
     }
     | term OVER factor
     {
       $$ = newExpNode(OpK);
       $$->attr.op = OVER;
       $$->child[0] = $1;
       $$->child[1] = $3;
     }
     | factor { $$ = $1; }
     ;

factor : NUM 
       {
         $$ = newExpNode(ConstK);
         $$->attr.val = atoi(tokenString);
       }
       | ID
       {
         $$ = newExpNode(IdK);
         $$->attr.name = copyString(tokenString);
       }
       | LPAREN exp RPAREN { $$ = $2; }
       | error { $$ = NULL; }
       ;
%%

int yylex() {
    return getToken();
}

int yyerror(char* message) {
    fprintf(listing, "Syntax error at line %d: %s\n", lineno, message);
    fprintf(listing, "Current token: ");
    printToken(yychar, tokenString);
    Error = TRUE;
    return 0;
}

TreeNode* parse() {
    yyparse();
    return savedTree;
}
