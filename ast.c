#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

typedef struct Identifiers_list{
	char name[MAX_IDENTIFIER_LENGTH+1];
	struct Identifiers_list *next;
} Identifiers_list;

void clear_identifiers_list(Identifiers_list *list)
{
	while(list)
	{
		Identifiers_list *next = list->next;
		free(list);
		list = next;
	}
}

Identifiers_list *push_identifiers(Identifiers_list *list, const char *name)
{
	Identifiers_list *new_node = malloc(sizeof(Identifiers_list));
	new_node -> next = list;
	strcpy(new_node->name, name);
	return new_node;
}

int count_identifiers(Identifiers_list *list)
{
	int retval = 0;
	for(; list; list = list->next)
		++retval;
	return retval;
}

int identifier_to_int(Identifiers_list *list, const char *identifier)
{
	int retval = 0;
	while(list)
	{
		if(strcmp(identifier, list->name) == 0)
			return retval;
		++retval;
		list = list->next;
	}
	return -1;
}

Identifiers_list *extract_identifiers(Parsing_tree_node *node)
{
	if(node->symbol != VARS_LIST)
	{
		printf("extract_identifiers function require vars list parsing tree as a parameter\n");
		exit(2);
	}
	Identifiers_list *list = NULL;
	while(1)	
	{
		Lexem *identifier_lexem = node->childs[0];
		const char *identifier_name = identifier_lexem->identifier_name;
		if(identifier_to_int(list, identifier_name) != -1)
		{
			printf("Identifier %s was declared twice\n", identifier_name);
			exit(2);
		}
		push_identifiers(list, identifier_name);
		if(((Parsing_tree_node*)(node->childs[1]))->childs_types[0] == NONE)
			break;
		node = ((Parsing_tree_node*)(node->childs[1]))->childs[0];
	}
}

Ast_node *build_expression_ast(Parsing_tree_node *tree, Identifiers_list *identifiers);

Ast_node *build_assignments_list_ast(Parsing_tree_node *tree, Identifiers_list *identifiers)
{
	Ast_node *result = malloc(sizeof(Ast_node));

	Ast_node *current_ast_node = result;
	Parsing_tree_node *current_parsing_node = tree;

	current_ast_node->type = AST_NOP;
	current_parsing_node = tree->childs[6];
	while(current_parsing_node->childs_types[0]!=NONE)
	{
		Lexem *first_lexem = current_parsing_node->childs[0];
		int is_write = 0;
		switch(first_lexem->type)
		{
			Ast_node *new_node;
			Parsing_tree_node *node;
			case IDENTIFIER:
				new_node = malloc(sizeof(Ast_node));
				new_node->type = AST_ASSIGNMENT;
				int ident_idx = identifier_to_int(identifiers, first_lexem -> identifier_name);
				if(ident_idx == -1)
				{
					printf("No such identifier: %s\n", first_lexem->identifier_name);
					exit(2);
				}
				new_node -> data = ident_idx;
				new_node -> childs[0] = build_expression_ast(current_parsing_node->childs[2], identifiers);
				current_parsing_node = current_parsing_node->childs[4];
				current_ast_node -> next = new_node;
				current_ast_node =  new_node;
			break;
			case WRITE:
				is_write = 1;
			case READ:	
				node = current_parsing_node -> childs[2];
				while(1)	
				{
					Lexem *identifier_lexem = node->childs[0];
					const char *identifier_name = identifier_lexem->identifier_name;
					int ident_idx = identifier_to_int(identifiers, identifier_name);
					if(ident_idx == -1)
					{
						printf("No such identifier: %s\n", identifier_name);
						exit(2);
					}
					Ast_node *new_node = malloc(sizeof(Ast_node));
					new_node -> type = is_write ? AST_WRITE : AST_READ;
					new_node -> data = ident_idx;
					new_node -> childs[0] = current_parsing_node -> childs[2];
					current_ast_node -> next = new_node;
					current_ast_node = new_node;
					if(((Parsing_tree_node*)(node->childs[1]))->childs_types[0] == NONE)
						break;
					node = ((Parsing_tree_node*)(node->childs[1]))->childs[0];
				}
				if(is_write)
				{
					Ast_node *new_node = malloc(sizeof(Ast_node));
					new_node -> type = AST_WRITE_CR;
					current_ast_node -> next = new_node;
					current_ast_node = new_node;
				}
				current_parsing_node = current_parsing_node->childs[5];
			break;
			case IF:
				new_node = malloc(sizeof(Ast_node));
				new_node->type = AST_BRANCH;
				new_node->childs[0] = build_expression_ast(current_parsing_node->childs[1], identifiers);
				new_node->childs[1] = build_assignments_list_ast(current_parsing_node->childs[3], identifiers);
				new_node->childs[2] = build_assignments_list_ast(current_parsing_node->childs[5], identifiers);
				current_ast_node -> next = new_node;
				current_ast_node = new_node;
			break;
		}
	}
	current_ast_node->next = NULL;

	return result;
}

Ast_node *build_ast(Parsing_tree_node *tree, int *identifiers_count)
{
	if(tree->symbol != PROGRAM)
	{
		printf("buils_ast function require program parsing tree as a parameter\n");
		exit(2);
	}

	Identifiers_list *identifiers = extract_identifiers(tree->childs[1]);

	*identifiers_count = count_identifiers(identifiers);

	Ast_node *result = build_assignments_list_ast(tree, identifiers);
	
	clear_identifiers_list(identifiers);
	return result;
}

int get_term_size(Parsing_tree_node *tree)
{
	Lexem *first_lexem = tree->childs[0];
	switch (first_lexem->type) {
		case ZERO_CONSTANT:
		case ONE_CONSTANT:
		case IDENTIFIER:
			return 1;
		case NOT:
			return 2;
		case OPEN_BRACKET:
			return 3;
	}
}

Ast_node *build_disjunction_ast(Parsing_tree_node *tree, Identifiers_list *identifiers);
Ast_node *build_conjunction_ast(Parsing_tree_node *tree, Identifiers_list *identifiers);
Ast_node *build_term_ast(Parsing_tree_node *tree, Identifiers_list *identifiers);

Ast_node *build_expression_ast(Parsing_tree_node *tree, Identifiers_list *identifiers)
{
	Parsing_tree_node *expression_c = tree->childs[get_term_size(tree)+2];
	if(expression_c->childs_types[0] == NONE)
	{
		return build_disjunction_ast(tree, identifiers);
	}else{
		Ast_node *result = malloc(sizeof(Ast_node));
		result -> type = AST_EQU;
		result -> childs[0] = build_disjunction_ast(tree, identifiers);
		result -> childs[1] = build_expression_ast(expression_c->childs[1], identifiers);
		return result;
	}
}

Ast_node *build_disjunction_ast(Parsing_tree_node *tree, Identifiers_list *identifiers)
{
	Parsing_tree_node *disjunction_c = tree->childs[get_term_size(tree)+1];
	if(disjunction_c->childs_types[0] == NONE)
	{
		return build_conjunction_ast(tree, identifiers);
	}else{
		Ast_node *result = malloc(sizeof(Ast_node));
		result -> type = AST_OR;
		result -> childs[0] = build_conjunction_ast(tree, identifiers);
		result -> childs[1] = build_disjunction_ast(disjunction_c->childs[1], identifiers);
		return result;
	}
}

Ast_node *build_conjunction_ast(Parsing_tree_node *tree, Identifiers_list *identifiers)
{
	Parsing_tree_node *conjunction_c = tree->childs[get_term_size(tree)];
	if(conjunction_c->childs_types[0] == NONE)
	{
		return build_term_ast(tree, identifiers);
	}else{
		Ast_node *result = malloc(sizeof(Ast_node));
		result -> type = AST_AND;
		result -> childs[0] = build_term_ast(tree, identifiers);
		result -> childs[1] = build_conjunction_ast(conjunction_c->childs[1], identifiers);
		return result;
	}
}

Ast_node *build_term_ast(Parsing_tree_node *tree, Identifiers_list *identifiers)
{
	Lexem *first_lexem = tree->childs[0];
	int constant_value = 1;
	Ast_node *result = malloc(sizeof(Ast_node));
	int ident_idx;
	switch (first_lexem->type) {
		case ZERO_CONSTANT:
			constant_value = 0;
		case ONE_CONSTANT:
			result -> type = AST_CONSTANT;
			result -> data = constant_value;
		break;
		case IDENTIFIER:
			ident_idx = identifier_to_int(identifiers, first_lexem->identifier_name);
			if(ident_idx==-1)
			{
				printf("No such identifier: %s\n", first_lexem->identifier_name);
				exit(2);
			}
			result -> type = AST_VARIABLE;
			result -> data = ident_idx;
		break;
		case OPEN_BRACKET:
			free(result);
			result = build_expression_ast(tree->childs[1], identifiers);
		break;
		case NOT:
			result -> type = AST_NOT;
			result -> childs[0] = build_expression_ast(tree->childs[1], identifiers);
		break;
	}
	return result;
}
