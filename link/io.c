#include <stdio.h>

void write_cr()
{
	printf("\n");
}

void write(int ident, char *array)
{
	int byte = ident/8;
	int bit = ident % 8;
	if(array[byte] & (1 << bit))
	{
		printf("1");
	}else{
		printf("0");
	}
}

void read(int ident, char *array)
{
	int byte = ident/8;
	int bit = ident % 8;
	int value;
	printf("? ");
	if(scanf("%d", &value)==0)
	{
		while(getchar()!='\n');
	}
	if(value == 0)
	{
		array[byte] &= (1 << bit) ^ 0xff;
	}else if(value == 1){
		array[byte] |= (1 << bit);
	}else{
		printf("Value must be 1 or 0\n");
		read(ident, array);
	}
}
