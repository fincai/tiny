/*******************************************/
/* File: util.h                            */
/* Utility functions for the tiny compiler */
/*******************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/* Procedure printToken prints a token */
/* and its lexeme to the listing file  */
void printToken(TokenType, const char*);

TreeNode* newStmtNode(StmtKind);
TreeNode* newExpNode(ExpKind);
char* copyString(char*);
void printTree(TreeNode*);

#endif
