/*************************************************/
/* File: analyze.h                               */
/* Semantic analyzer interface for tiny compiler */
/*************************************************/

#ifndef _ANALYZE_H_
#define _ANALYZE_H_

/* Function buildSymtab constructs symbol table
 * by preorder traversal of the AST 
 */
void buildSymtab(TreeNode*);

/* Function typeCheck performs type checking
 * by postorder traversal of the AST
 */
void typeCheck(TreeNode*);

#endif
