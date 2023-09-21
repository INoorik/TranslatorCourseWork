#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "lexical_analyzer.h"

#define KEYWORD(word) if(strcmp(symbol, #word) == 0){ lexem->type = word; return; }

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

	if(c==-1)
	{
		lexem -> type = EOF_LEXEM;
		return;
	}
	if(c=='(')
	{
		lexem -> type = OPEN_BRACKET;
		return;
	}
	if(c==')')
	{
		lexem -> type = CLOSE_BRACKET;
		return;
	}
	if(c=='0')
	{
		lexem -> type = ZERO_CONSTANT;
		return;
	}
	if(c=='1')
	{
		lexem -> type = ONE_CONSTANT;
		return;
	}
	if(c==',')
	{
		lexem -> type = COMMA;
		return;
	}
	if(c==':')
	{
		lexem -> type = COLON;
		return;
	}
	if(c==';')
	{
		lexem -> type = SEMICOLON;
		return;
	}
	if(c=='=')
	{
		lexem -> type = ASSIGNMENT;
		return;
	}
	char symbol[MAX_IDENTIFIER_LENGTH + 1];
	symbol[0] = c;
	if(!isalpha(c))
	{
		printf("Invalid character: %c\n", c);
		exit(1);
	}
	int i = 1;
	c = getc(file);
	while(isalpha(c) && i<MAX_IDENTIFIER_LENGTH)
	{
		symbol[i] = c;
		++i;
		c = getc(file);
	}
	if(i==MAX_IDENTIFIER_LENGTH)
	{
		printf("Too long identifier: %s\n", symbol);
		exit(1);
	}
	ungetc(c, file);
	symbol[i] = '\0';
	KEYWORD(BEGIN)
	KEYWORD(END)
	KEYWORD(VAR)
	KEYWORD(LOGICAL)
	KEYWORD(NOT)
	KEYWORD(OR)
	KEYWORD(AND)
	lexem -> type = IDENTIFIER;
	strcpy(lexem->identifier_name, symbol);
}
