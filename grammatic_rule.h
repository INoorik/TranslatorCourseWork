#ifdef GRAMTIC_RULE
#define GRAMTIC_RULE

#include "parser.h"

#define MAX_SYMBOLS_IN_RULE 15

typedef struct{
	Lexem_type left_part;
	Lexem_type first_lexem_of_right_part;
	United_dict_symbol right_part[MAX_SYMBOLS_IN_RULE];
}Grammatic_rule

#endif
