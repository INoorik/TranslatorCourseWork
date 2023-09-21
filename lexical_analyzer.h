#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER

#define MAX_IDENTIFIER_LENGTH 11

typedef enum 
{
	BEGIN,
	END,
	VAR,
	LOGICAL,
	ZERO_CONSTANT,
	ONE_CONSTANT,
	IDENTIFIER,
	NOT,
	OR,
	AND,
	EQU,
	COMMA,
	COLON,
	SEMICOLON,
	ASSIGNMENT,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	EOF_LEXEM
} Lexem_type;

typedef struct {
	Lexem_type type;
	char identifier_name[MAX_IDENTIFIER_LENGTH + 1];
} Lexem;

void read_lexem(Lexem *lexem, FILE *file);

#endif
