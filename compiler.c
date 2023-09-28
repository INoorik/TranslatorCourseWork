#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "code_generator.h"

int main(int argc, char **argv)
{
	FILE *input_file;
	FILE *output_file;
	if(argc == 3)
	{
		input_file = fopen(argv[1], "r");
		output_file = fopen(argv[2], "w");
	}else if(argc == 2)
	{
		input_file = fopen(argv[1], "r");
		output_file = stdout;
	}else if(argc == 1)
	{
		input_file = stdin;
		output_file = stdout;
	}else{
		printf("Usage: %s\n or %s <filename>\n or %s <filename> <filename>\n", argv[0], argv[0], argv[0]);
		exit(1);
	}
	if(!output_file)
	{
		printf("No output file specified\n");
		exit(1);
	}
	if(!input_file)
	{
		printf("No input file specified\n");
		exit(1);
	}
	Parsing_tree_node *tree = parse(input_file);
	int ident_count;
	Ast_node *ast = build_ast(tree, &ident_count);
	clear_parsing_tree(tree);
	write_program(output_file, ast, ident_count);
	fclose(output_file);
	fclose(input_file);
}
