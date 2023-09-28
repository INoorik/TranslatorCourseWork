#ifndef CODE_GENERATOR
#define CODE_GENERATOR

#include <stdio.h>

#include "ast.h"

void write_program(FILE *file, Ast_node *ast, int identifiers_num);

#endif
