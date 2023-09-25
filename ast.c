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

Ast_node *build_ast(Parsing_tree_node *tree, int *identifier)
{
	if(tree->type != PROGRAM)
	{
		printf("buils_ast function require program parsing tree as a parameter\n");
		return 2;
	}

	Identifiers_list *identifiers = extract_identifiers(tree->childs[1]);
	
	clear_identifiers_list(identifiers);
}
