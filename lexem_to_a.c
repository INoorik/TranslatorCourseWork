#include "lexem_to_a.h"

#define LEXEM(lex, str) if(lexem == lex) return str " (" #lex ")";
#define KEYWORD(lex) if(lexem == lex) return #lex;

char *lexem_to_a(Lexem_type lexem)
{
	KEYWORD(BEGIN);
	KEYWORD(END);
	KEYWORD(VAR);
	KEYWORD(LOGICAL);
	KEYWORD(IDENTIFIER);
	KEYWORD(NOT);
	KEYWORD(OR);
	KEYWORD(AND);
	KEYWORD(EQU);
	KEYWORD(IF);
	KEYWORD(THEN);
	KEYWORD(ELSE);
	KEYWORD(END_IF);
	KEYWORD(READ);
	KEYWORD(WRITE);
	KEYWORD(EOF_LEXEM);
	LEXEM(ZERO_CONSTANT, "0");
	LEXEM(ONE_CONSTANT, "1");
	LEXEM(COMMA, ",");
	LEXEM(COLON, ":");
	LEXEM(SEMICOLON, ";");
	LEXEM(ASSIGNMENT, "=");
	LEXEM(OPEN_BRACKET, "(");
	LEXEM(CLOSE_BRACKET, ")");
}
