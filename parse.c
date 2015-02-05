/************************************************/
/* File: parse.c                                */
/* The recursive-descent parser implementation  */
/* for the tiny compuler                        */
/************************************************/ 

#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token;  /* holds current token */

/* function declarations for recursive calls */
static TreeNode* stmt_sequence();
static TreeNode* statement();
static TreeNode* if_stmt();
static TreeNode* repeat_stmt();
static TreeNode* assign_stmt();
static TreeNode* read_stmt();
static TreeNode* write_stmt();
static TreeNode* exp();
static TreeNode* simple_exp();
static TreeNode* term();
static TreeNode* factor();

static void syntaxError(char* message)
{
    fprintf(listing, "\n>>> ");
    fprintf(listing, "Syntax error at line %d: %s", lineno, message);
    Error = TRUE;
}

static void match(TokenType expected)
{
    if (token == expected) token = getToken();
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        fprintf(listing, "          ");
    }
}

TreeNode* stmt_sequence()
{
    TreeNode *t = statement();
    TreeNode *p = t;
    while (token != ENDFILE && token != END 
         && token != ELSE && token != UNTIL) {
        match(SEMI);
        TreeNode *q = statement();
        if (q != NULL) {
            if (t == NULL) t = p = q;
            else {  // p and q are not null connect them
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}


TreeNode* statement() 
{ 
    TreeNode *t = NULL;
    switch(token) {
        case IF: t = if_stmt(); break;
        case REPEAT: t = repeat_stmt(); break;
        case ID: t = assign_stmt(); break;
        case READ: t = read_stmt(); break;
        case WRITE: t = write_stmt(); break;
        default: syntaxError("unexpected token -> ");
                 printToken(token, tokenString);
                 token = getToken();
                 break;
    }
    return t;
}

TreeNode* if_stmt() 
{ 
    TreeNode *t = newStmtNode(IfK);
    match(IF);
    if (t != NULL) t->child[0] = exp();
    match(THEN);
    if (t != NULL) t->child[1] = stmt_sequence();
    if (token == ELSE) {
        match(ELSE);
        t->child[2] = stmt_sequence();
    }
    match(END);
    return t;
}

TreeNode* repeat_stmt() 
{ 
    TreeNode *t = newStmtNode(RepeatK);
    match(REPEAT);
    if (t != NULL) t->child[0] = stmt_sequence();
    match(UNTIL);
    if (t != NULL) t->child[1] = exp();
    return t;
}

TreeNode* assign_stmt() 
{ 
    TreeNode *t = newStmtNode(AssignK);
    if (t != NULL && token == ID) 
        t->attr.name = copyString(tokenString);
    match(ID);
    match(ASSIGN);
    if (t != NULL) t->child[0] = exp();
    return t; 
}

TreeNode* read_stmt() 
{ 
   TreeNode *t = newStmtNode(ReadK);
   match(READ);
   if (t != NULL && token == ID)
       t->attr.name = copyString(tokenString);
   match(ID);
   return t;
}

TreeNode* write_stmt() 
{ 
    TreeNode *t = newStmtNode(WriteK);
    match(WRITE);
    if (t != NULL) t->child[0] = exp();
    return t;
}

TreeNode* exp() 
{ 
    TreeNode *t = simple_exp();
    if (token == LT || token == EQ) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->attr.op = token;
            p->child[0] = t;
            t = p;
        }
        match(token);
        if (t != NULL) 
            t->child[1] = simple_exp();
    }
    return t;
}

TreeNode* simple_exp() 
{ 
    TreeNode *t = term();
    while (token == PLUS || token == MINUS) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->attr.op = token;
            p->child[0] = t;
            t = p;
            match(token);
            t->child[1] = term();
        }
    } 
    return t;
} 

TreeNode* term() 
{ 
    TreeNode *t = factor();
    while (token == TIMES || token == OVER) {
        TreeNode *p = newExpNode(OpK);
        if (p != NULL) {
            p->attr.op = token;
            p->child[0] = t;
            t = p;
            match(token);
            t->child[1] = factor();
        }
    }
    return t;
}

TreeNode* factor() 
{ 
    TreeNode *t = NULL;
    switch (token) {
        case LPAREN:
            match(LPAREN);
            t = exp();
            match(RPAREN);
            break;
        case ID:
            t = newExpNode(IdK);
            if (t != NULL)
                t->attr.name = copyString(tokenString);
            match(ID);
            break;
        case NUM:
            t = newExpNode(ConstK);
            if (t != NULL)
                t->attr.val = atoi(tokenString);
            match(NUM);
            break;
        default: 
            syntaxError("unexpected token -> ");
            printToken(token, tokenString);
            token = getToken();
            break;
    }
    return t;
}

TreeNode* parse() 
{
    token = getToken();
    TreeNode *t = stmt_sequence();
    if (token != ENDFILE)
        syntaxError("Code ends before file\n");
    return t;
}
