#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "lexical_analyzer.h"

#define KEYWORD(word) if(strcmp(symbol, #word) == 0){ lexem->type = word; return; }
#define ONE_SYMBOL_LEXEM(ch, lexem_name) if(c==ch) { lexem->type = lexem_name; return; }

int is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
		return 1;
	return 0;
}

void read_lexem(Lexem *lexem, FILE *file)
{
	int c = getc(file);
	while(is_space(c))
		c = getc(file);

	ONE_SYMBOL_LEXEM(-1, EOF_LEXEM);
	ONE_SYMBOL_LEXEM('(', OPEN_BRACKET)
	ONE_SYMBOL_LEXEM(')', CLOSE_BRACKET);
	ONE_SYMBOL_LEXEM('0', ZERO_CONSTANT);
	ONE_SYMBOL_LEXEM('1', ONE_CONSTANT);
	ONE_SYMBOL_LEXEM(',', COMMA);
	ONE_SYMBOL_LEXEM(':', COLON);
	ONE_SYMBOL_LEXEM(';', SEMICOLON);
	ONE_SYMBOL_LEXEM('=', ASSIGNMENT);
	char symbol[MAX_IDENTIFIER_LENGTH + 1];
	symbol[0] = c;
	if(!isalpha(c))
	{
		printf("Invalid character: %c\n", c);
		exit(1);
	}
	int i = 1;
	c = getc(file);
	while((isalpha(c) || c=='_' )&& i<MAX_IDENTIFIER_LENGTH)
	{
		symbol[i] = c;
		++i;
		c = getc(file);
	}
	symbol[i] = '\0';
	if(i==MAX_IDENTIFIER_LENGTH)
	{
		printf("Too long identifier: %s\n", symbol);
		exit(1);
	}
	ungetc(c, file);
	KEYWORD(BEGIN);
	KEYWORD(END);
	KEYWORD(VAR);
	KEYWORD(LOGICAL);
	KEYWORD(NOT);
	KEYWORD(OR);
	KEYWORD(AND);
	KEYWORD(READ);
	KEYWORD(WRITE);
	KEYWORD(IF);
	KEYWORD(ELSE);
	KEYWORD(THEN);
	KEYWORD(END_IF);
	KEYWORD(EQU);
	for(char *ptr = symbol; *ptr; ptr++)
	{
		if(*ptr == '_')
		{
			printf("Invalid character: _\n");
			exit(1);
		}
	}
	lexem -> type = IDENTIFIER;
	strcpy(lexem->identifier_name, symbol);
}
