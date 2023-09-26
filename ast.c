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
	if(tree->type != VARS_LIST)
	{
		printf("extract_identifiers function require vars list parsing tree as a parameter\n");
		exit(2);
	}
	Identifiers_list *list = NULL;
	while(1)	
	{
		Lexem *identifier_lexem = node->childs[0];
		const char *identifier_name = identifier_lexem->idenntifier_name;
		if(identifier_to_int(list, identifier_name) != -1)
		{
			printf("Identifier %s was declared twice\n", identifier_name);
			exit(2);
		}
		push_identifier(list, identifier_name);
		if(node->childs[1]->childs_types[0] == NONE)
			break;
		node = node->childs[1]->childs[0];
	}
}

Ast_node *build_expression_ast(Parsing_tree_node *tree, Identifiers_list *identifiers);

Ast_node *build_ast(Parsing_tree_node *tree)
{
	if(tree->type != PROGRAM)
	{
		printf("buils_ast function require program parsing tree as a parameter\n");
		return 2;
	}

	Identifiers_list *identifiers = extract_identifiers(tree->childs[1]);

	Ast_node *result = malloc(sizeof(Ast_node));

	Ast_node *current_ast_node = result;

	current_ast_node->type = AST_NOP;
	current_parsing_node = tree->childs[6];
	while(current_parsing_node->childs_types[0]!=NONE)
	{
		Lexem *first_lexem = current_parsing_node->childs[0];
		int is_write = 0;
		switch(first_lexem->type)
		{
			case IDENTIFIER:
				Ast_node *new_node = malloc(sizeof(Ast_node));
				new_node->type = AST_ASSIGNMENT;
				int ident_idx = identifier_to_int(identifiers, first_lexem -> identifier_name);
				if(ident_idx == -1)
				{
					printf("No such identifier: %s\n", first_lexem->identifier_name);
					exit(2);
				}
				new_node -> data = ident_idx;
				new_node -> childs[0] = build_expression_ast(current_parsing_tree->childs[2], identifiers);
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
					const char *identifier_name = identifier_lexem->idenntifier_name;
					int ident_idx = identifier_to_int(identifiers, identifier_name);
					if(ident_idx == -1)
					{
						printf("No such identifier: %s\n", identifier_name);
						exit(2);
					}
					Ast_node *new_node = malloc(sizeof(Ast_node));
					new_node -> type = is_write ? AST_WRITE : AST_READ;
					new_node -> data = ident_idx;
					new_node -> childs[0] = current_pasing_node -> childs[2];
					current_ast_node -> next = new_node;
					current_ast_node = new_node;
					if(node->childs[1]->childs_types[0] == NONE)
						break;
					node = node->childs[1]->childs[0];
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
				Ast_node *new_node = malloc(sizeof(Ast_node));
				new_node->type = AST_BRANCH;
				new_node->childs[0] = build_expression_ast(current_parsing_node->childs[1], identifiers);
				new_node->childs[1] = build_ast(current_parsing_node->childs[3]);
				new_node->childs[2] = build_ast(current_parsing_node->childs[5]);
				current_ast_node -> next = new_node;
				current_ast_node = new_node;
			break;
		}
	}
	current_ast_node->next = NULL;
	
	clear_identifiers_list(identifiers);
	return result;
}

Ast_node *build_expression_ast(Parsing_tree_node *tree, Identifiers_list *identifiers)
{
}
