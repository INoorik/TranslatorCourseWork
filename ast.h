#ifndef AST
#define AST

#include "parser.h"

#define MAX_AST_NODE_CHILDS 5

typedef enum{
	AST_NOT, 
	AST_AND,
	AST_OR,
	AST_EQU,
	AST_CONSTANT,
	AST_VARIABLE,
	AST_ASSIGNMENT,
	AST_READ,
	AST_WRITE,
	AST_WRITE_CR,
	AST_BRANCH,
	AST_NOP
} Ast_node_type;

typedef struct Ast_node{
	Ast_node_type type;
	int data;
	struct Ast_node *childs[MAX_AST_NODE_CHILDS];
	struct Ast_node *next;
} Ast_node;

Ast_node *build_ast(Parsing_tree_node *tree, int *identifiers_cnt);

#endif
