#ifndef PARSER
#define PARSER

#include "lexical_analyzer.h"

#define MAX_CHILDS_IN_PARSING_TREE 10

typedef enum{
	PROGRAM,
	VARS_LIST,
	VAR_LIST_C,
	ASSIGNMENTS_LIST,
	EXPRESSION,
	EXPRESSION_C,
	DISJUNCTION,
	DISJUNCTION_C,
	CONJUNCTION,
	CONJUNCTION_C,
	TERM
} Non_terminal;

typedef enum{
	TERMINAL,
	NON_TERMINAL,
	NONE
} Symbol_type;

typedef int United_dict_symbol;

United_dict_symbol lexem2united_dict_symbol(Lexem_type symbol);
United_dict_symbol non_terminal2united_dict_symbol(Non_terminal symbol);
Symbol_type get_symbol_type(United_dict_symbol symbol);

typedef struct Parsing_tree_node{
	Non_terminal symbol;
	void *childs[MAX_CHILDS_IN_PARSING_TREE];
	Symbol_type childs_types[MAX_CHILDS_IN_PARSING_TREE];
	struct Parsing_tree_node *parent;
} Parsing_tree_node;

typedef struct Symbol_stack{
	United_dict_symbol symbol;
	struct Symbols_stack *next;
} Symbol_stack;

void clear_parsing_tree(Parsing_tree_node *tree);

Parsing_tree_node *parse(FILE *file);

#endif
