#include <stdlib.h>

#include "parser.h"
#include "grammar.h"
#include "lexem_to_a.h"

const size_t RULES_CNT =  sizeof(grammar) / sizeof(grammar[0]);

United_dict_symbol lexem2united_dict_symbol(Lexem_type symbol)
{
	return symbol*2;
}

United_dict_symbol non_terminal2united_dict_symbol(Non_terminal symbol)
{
	return symbol*2 + 1;
}

Symbol_type get_symbol_type(United_dict_symbol symbol)
{
	if(symbol&1)
		return NON_TERMINAL;
	return TERMINAL;
}

void clear_parsing_tree(Parsing_tree_node *tree)
{
	for(int i=0; i<MAX_CHILDS_IN_PARSING_TREE; i++)
	{
		if(tree->childs_types[i] == NONE)
			break;

		if(tree->childs_types[i] == TERMINAL)
			free(tree->childs[i]);
		else
			clear_parsing_tree(tree->childs[i]);
	}
	free(tree);
}

Symbol_stack *push_symbol_to_stack(Symbol_stack *stack, United_dict_symbol symbol)
{
	Symbol_stack *node = malloc(sizeof(Symbol_stack));
	node->symbol = symbol;
	node->next = stack;
	return node;
}

United_dict_symbol symbol_stack_top(Symbol_stack *stack)
{
	return stack->symbol;
}

Symbol_stack *pop_symbol_from_stack(Symbol_stack *stack)
{
	Symbol_stack *retval = stack->next;
	free(stack);
	return retval;
}

Parsing_tree_node *new_parsing_tree_node(Non_terminal symbol, Parsing_tree_node *parent)
{
	Parsing_tree_node *retval = malloc(sizeof(Parsing_tree_node));
	retval->symbol = symbol;
	retval->parent = parent;
	for (int i = 0; i < MAX_CHILDS_IN_PARSING_TREE; ++i)
	{
		retval -> childs[i] = NULL;
		retval -> childs_types[i] = NONE;
	}
	return retval;
}

int get_first_none_idx(Parsing_tree_node *node)
{
	int retval = 0;
	while (node -> childs_types[retval] != NONE)
		++retval;
	return retval;
}

Parsing_tree_node *parse(FILE *file)
{
	Symbol_stack *stack = NULL;
	stack = push_symbol_to_stack(stack, lexem2united_dict_symbol(END));
	stack = push_symbol_to_stack(stack, non_terminal2united_dict_symbol(ASSIGNMENTS_LIST));
	stack = push_symbol_to_stack(stack, lexem2united_dict_symbol(BEGIN));
	stack = push_symbol_to_stack(stack, lexem2united_dict_symbol(SEMICOLON));
	stack = push_symbol_to_stack(stack, lexem2united_dict_symbol(LOGICAL));
	stack = push_symbol_to_stack(stack, lexem2united_dict_symbol(COLON));
	stack = push_symbol_to_stack(stack, non_terminal2united_dict_symbol(VARS_LIST));
	stack = push_symbol_to_stack(stack, lexem2united_dict_symbol(VAR));

	Parsing_tree_node *root = new_parsing_tree_node(PROGRAM, NULL);

	Parsing_tree_node *current = root;

	Lexem lexem;

	read_lexem(&lexem, file);

	while (stack)
	{
		if(symbol_stack_top(stack) == -1)
		{
			current = current->parent;
			stack = pop_symbol_from_stack(stack);
			continue;
		}

		United_dict_symbol top = symbol_stack_top(stack);

		int success = 0;
		if(get_symbol_type(top) == TERMINAL)
		{
			if(lexem2united_dict_symbol(lexem.type)==top)
			{
				stack = pop_symbol_from_stack(stack);
				int idx = get_first_none_idx(current);
				current -> childs_types[idx] = TERMINAL;
				current -> childs[idx] = malloc(sizeof(Lexem));
				*(Lexem*)(current -> childs[idx]) = lexem;
				read_lexem(&lexem, file);
			}else{
				printf("Expected %s, got %s\n", lexem_to_a(top/2), lexem_to_a(lexem.type));
				exit(1);
			}
		}else{
			for(int i = 0; i<RULES_CNT; ++i)
			{
				if(non_terminal2united_dict_symbol(grammar[i].left_part) != top)
					continue;
				if(grammar[i].first_lexem_of_right_part == -1)
				{
					int idx = get_first_none_idx(current);
					current -> childs_types[idx] = NON_TERMINAL;
					current -> childs[idx] = new_parsing_tree_node(grammar[i].left_part, current);
					current = current -> childs[idx];
					stack = pop_symbol_from_stack(stack);
					stack = push_symbol_to_stack(stack, -1);
					success = 1;
					break;
				}
				if(grammar[i].first_lexem_of_right_part != lexem.type)
					continue;
				stack = pop_symbol_from_stack(stack);
				stack = push_symbol_to_stack(stack, -1);
				for(int s = 0; grammar[i].right_part[s] != -1; ++s)
				{
					stack = push_symbol_to_stack(stack, grammar[i].right_part[s]);
				}
				int idx = get_first_none_idx(current);
				current -> childs_types[idx] = NON_TERMINAL;
				current -> childs[idx] = new_parsing_tree_node(grammar[i].left_part, current);
				current = current -> childs[idx];
				current -> childs_types[0] = TERMINAL;
				current -> childs[0] = malloc(sizeof(Lexem));
				*(Lexem*)(current -> childs[0]) = lexem;
				read_lexem(&lexem, file);
				success = 1;
				break;
			}
			if(success)
				continue;
			printf("Unexpected %s\n", lexem_to_a(lexem.type));
			exit(1);
		}
	}
		
	if(lexem.type != EOF_LEXEM)
	{
		printf("Expected EOF, got %s\n", lexem_to_a(lexem.type));
		exit(1);
	}

	return root;
}

