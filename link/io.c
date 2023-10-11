#define PUT_STRING(str) write_syscall(1, str, sizeof(str));

void write_syscall(int fd, char *buf, int len)
{
	asm volatile (
			"int $0x80\n"
			::"a"(4), "b"(fd), "c"(buf), "d"(len)
		     );
}

void read_syscall(int fd, char *buf, int len)
{
	asm volatile (
			"int $0x80\n"
			::"a"(3), "b"(fd), "c"(buf), "d"(len)
		     );
}

void put_char(char c)
{
	write_syscall(1, &c, 1);
}

char get_char()
{
	char result;
	read_syscall(0, &result, 1);
	return result;
}

void write_cr()
{
	PUT_STRING("\n");
}

void write(int ident, char *array)
{
	int byte = ident/8;
	int bit = ident % 8;
	if(array[byte] & (1 << bit))
	{
		PUT_STRING("1");
	}else{
		PUT_STRING("0");
	}
}

#define READ_ERROR_ALERT "Value must be 0 or 1\n"

void read(int ident, char *array)
{
	int success = 0;
	int byte = ident/8;
	int bit = ident % 8;

	while(!success)
	{
		PUT_STRING("? ");
		char c1 = get_char();
		if(c1 == '\n')
		{
			PUT_STRING(READ_ERROR_ALERT);
			continue;
		}
		char c2 = get_char();
		if(c2 != '\n')
		{
			while(get_char() != '\n');
			PUT_STRING(READ_ERROR_ALERT);
			continue;
		}
		if(c1 == '1')
		{
			array[byte] |= 1 << bit;
			success = 1;
		}else if(c1 == '0'){
			array[byte] &= (1 << bit) ^ 0xff;
			success = 1;
		}else{
			PUT_STRING(READ_ERROR_ALERT);
		}
	}
}
