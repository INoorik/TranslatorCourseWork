#ifndef GRAMMATIC_RULE
#define GRAMMATIC_RULE

#include "parser.h"

#define MAX_SYMBOLS_IN_RULE 15

typedef struct{
	Non_terminal left_part;
	Lexem_type first_lexem_of_right_part;
	United_dict_symbol right_part[MAX_SYMBOLS_IN_RULE];
} Grammatic_rule;

#endif
