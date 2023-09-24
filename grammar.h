#include "grammatic_rule.h"

Grammatic_rule grammar[] = {
	{VARS_LIST, IDENTIFIER, {non_terminal2united_dict_symbol(VARS_LIST_C), -1}},
	{VARS_LIST_C, COMMA, {non_terminal2united_dict_symbol(VARS_LIST), -1}},
	{VARS_LIST_C, -1, {}},
	{ASSIGNMENTS_LIST, IDENTIFIER, {
					       lexem2united_dict_symbol(SEMICOLON), 
					       non_terminal2united_dict_symbol(EXPRESSION),
					       lexem2united_dict_symbol(ASSIGNMENT),
					       -1
				       }},
	// to be continued ... 
}
