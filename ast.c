#include <stdio.h>
#include <string.h>

#include "ast.h"

typedef struct Identifiers_list{
	char name[MAX_IDENTIFIER_LENGTH+1];
	struct Identifiers_list *next;
} Identifiers_list;

void clear_identifiers_list(Identifiers_list *list)
{
	while(list)
	{
		Identifiers_list *next = list->next;
		free(list);
		list = next;
	}
}

int count_identifiers(Identifiers_list *list)
{
	int retval = 0;
	for(; list; list = list->next)
		++retval;
	return retval;
}

int identifier_to_int(Identifiers_list *list, const char *identifier)
{
	int retval = 0;
	while(list)
	{
		if(strcmp(identifier, list->name) == 0)
			return retval;
		++retval;
		list = list->next;
	}
	return -1;
}


