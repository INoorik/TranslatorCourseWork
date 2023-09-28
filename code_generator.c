#include "code_generator.h"

int get_unique_number()
{
	static int number = 0;
	return number++;
}

void write_expression(FILE *file, Ast_node *ast)
{
	switch (ast->type)
	{
		int label;
		int byte, bit;
		case AST_NOT:
			write_expression(file, ast->childs[0]);
			fprintf(file, "sete %%al\n");
			fprintf(file, "test %%al, %%al\n");
		break;
		case AST_AND:
			label = get_unique_number();
			write_expression(file, ast->childs[0]);
			fprintf(file, "jz label_%d\n", label);
			write_expression(file, ast->childs[1]);
			fprintf(file, "label_%d:\n", label);
		break;
		case AST_OR:
			label = get_unique_number();
			write_expression(file, ast->childs[0]);
			fprintf(file, "jnz label_%d\n", label);
			write_expression(file, ast->childs[1]);
			fprintf(file, "label_%d:\n", label);
		break;
		case AST_EQU:
			label = get_unique_number();
			write_expression(file, ast->childs[0]);
			fprintf(file, "push %%eax\n");
			write_expression(file, ast->childs[1]);
			fprintf(file, "pop %%eax\n");
			fprintf(file, "jnz label_%d\n", label);
			fprintf(file, "test %%al, %%al\n");
			fprintf(file, "sete %%al\n");
			fprintf(file, "label_%d:\n", label);
			fprintf(file, "test %%al, %%al\n");
		break;
		case AST_CONSTANT:
			if(ast->data)
				fprintf(file, "or $1, %%al\n");
			else
				fprintf(file, "and $0, %%al\n");
		break;
		case AST_VARIABLE:
			byte = (ast->data)/8;
			bit = (ast->data)%8;
			fprintf(file, "mov %d(%%ebx), %%al\n", byte);
			fprintf(file, "and $%d, %%al\n", 1<<bit);
		break;
	}
}

void write_assignments_list(FILE *file, Ast_node *ast)
{
	for(; ast; ast = ast->next)
	{
		int is_read = 0;
		int else_label, endif_label;
		int assignment_label;
		int byte, bit;
		switch(ast->type)
		{
			case AST_ASSIGNMENT:
				assignment_label = get_unique_number();
				byte = (ast->data)/8;
				bit = (ast->data)%8;
				fprintf(file, "mov %d(%%ebx), %%ah\n", byte);
				fprintf(file, "and $%d, %%ah\n", 0xff ^ (1<<bit));
				write_expression(file, ast->childs[0]);
				fprintf(file, "jz label_%d\n", assignment_label);
				fprintf(file, "or $%d, %%ah\n", 1<<bit);
				fprintf(file, "label_%d:\n", assignment_label);
				fprintf(file, "mov %%ah, %d(%%ebx)\n", byte);
			break;
			case AST_READ:
				is_read = 1;
			case AST_WRITE:
				fprintf(file, "push %%ebx\n");
				fprintf(file, "push $%d\n", ast->data);
				fprintf(file, "call ");
				fprintf(file, is_read ? "read\n" : "write\n");
				fprintf(file, "add $8, %%esp\n");
				fprintf(file, "xor %%eax, %%eax\n");
			break;
			case AST_WRITE_CR:
				fprintf(file, "call write_cr\n");
				fprintf(file, "xor %%eax, %%eax\n");
			break;
			case AST_BRANCH:
				else_label = get_unique_number();
				endif_label = get_unique_number();
				write_expression(file, ast->childs[0]);
				fprintf(file, "jz label_%d\n", else_label);
				write_assignments_list(file, ast->childs[1]);
				fprintf(file, "jmp label_%d\n", endif_label);
				fprintf(file, "label_%d:\n", else_label);
				write_assignments_list(file, ast->childs[2]);
				fprintf(file, "label_%d:\n", endif_label);
			break;
			case AST_NOP:
			break;
		}
	}
}

void write_program(FILE *file, Ast_node *ast, int identifiers_num)
{
	fprintf(file, "variables_array_size = %d\n", (identifiers_num+7)/8);
	fprintf(file, ".include \"prologue.s\"\n");

	write_assignments_list(file, ast);

	fprintf(file, ".include \"epilogue.s\"\n");
}
