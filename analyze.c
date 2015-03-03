/******************************************************/
/* File: analyze.c                                    */
/* Semantic analyzer implementation for tiny compiler */
/******************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"

static int location;

static void traverse(TreeNode* t, 
              void (* preProc) (TreeNode*),
              void (* postProc) (TreeNode*))
{
    if (t != NULL) {
        preProc(t);
        int i;
        for (i = 0; i < MAXCHILDREN; ++i)
            traverse(t->child[i], preProc, postProc);
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

static void nullProc(TreeNode* t) { }

void insertNode(TreeNode* t)
{
    switch (t->nodekind) {
        case StmtK:
            switch(t->kind.stmt) {
                case AssignK:
                case ReadK:
                    if (st_lookup(t->attr.name) == -1)
                        st_insert(t->attr.name, t->lineno, location++);
                    else  // Already in symtab, ignore the location
                        st_insert(t->attr.name, t->lineno, 0);
                    break;
                default:
                    break;
            }
            break;
        case ExpK:
            switch(t->kind.exp) {
                case IdK:
                    if (st_lookup(t->attr.name) == -1)
                        st_insert(t->attr.name, t->lineno, location++);
                    else
                        st_insert(t->attr.name, t->lineno, 0);
                    break;
                default:
                    break;
            }
            break;
    }
}

void buildSymtab(TreeNode* syntaxTree)
{
    traverse(syntaxTree, insertNode, nullProc);
    if (TraceAnalyze) {
        fprintf(listing, "\nSymbol Table:\n\n");
        printSymtab(listing);
    }
}

static void typeError(TreeNode* t, char* message)
{
    fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
    Error = TRUE;
}


void checkNode(TreeNode* t)
{
    switch(t->nodekind) {
        case ExpK:
            switch(t->kind.exp) {
                case OpK:
                    // Type checking
                    if (t->child[0]->type != Integer ||
                        t->child[1]->type != Integer) 
                        typeError(t, "Operator applied to non-Integer");
                    // Type inference
                    if (t->attr.op == EQ || t->attr.op == LT)
                        t->type = Boolean;
                    else
                        t->type = Integer;
                    break;
                case ConstK:
                case IdK:
                    t->type = Integer;
                    break;
                default:
                    break;
            }
            break;
        case StmtK:
            switch(t->kind.stmt) {
                case IfK:
                    if (t->child[0]->type != Boolean)
                        typeError(t->child[0], "If test-expr must be boolean");
                    break;
                case RepeatK:
                    if (t->child[1]->type != Boolean)
                        typeError(t->child[1], "Repeat test-expr must be boolean");
                    break;
                case AssignK:
                    if (t->child[0]->type != Integer)
                        typeError(t->child[0], "Assignment of non-Integer value");
                    break;
                case WriteK:
                    if (t->child[0]->type != Integer)
                        typeError(t->child[0], "Write to non-Integer value");
                    break;
                default:
                    break;
            }
            break;
    }
}
void typeCheck(TreeNode* syntaxTree)
{
    traverse(syntaxTree, nullProc, checkNode); 
}


