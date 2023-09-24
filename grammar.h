#include "grammatic_rule.h"

#define NON_TERMINAL_ENTRY(s) s*2 + 1
#define TERMINAL_ENTRY(s) s*2 

Grammatic_rule grammar[] = {
	{VARS_LIST, IDENTIFIER, {NON_TERMINAL_ENTRY(VARS_LIST_C), -1}},
	{VARS_LIST_C, COMMA, {NON_TERMINAL_ENTRY(VARS_LIST), -1}},
	{VARS_LIST_C, -1, {}},
	{ASSIGNMENTS_LIST, IDENTIFIER, {
					       TERMINAL_ENTRY(SEMICOLON), 
					       NON_TERMINAL_ENTRY(EXPRESSION),
					       TERMINAL_ENTRY(ASSIGNMENT),
					       -1
				       }},
	// to be continued ... 
};
