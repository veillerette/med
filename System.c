#include "System.h"

extern void _mem(void *ptr, const char *str, const char *file, int line)
{
	if(NULL == ptr)
	{
		printf("Memory Allocation Error variable \"%s\", ", str);
		printf("at \"%s\":%d\n", file, line);
		exit(EXIT_FAILURE);
	}
}

extern int text_color(TerminalColor color)
{
	char strC[7];
	
	strC[0] = 27;
	strC[1] = '[';
	strC[2] = '0' + color / 10;
	strC[3] = '0' + color % 10;
	strC[4] = 'm';
	strC[5] = '\0';
	
	return printf("%s", strC) == 5;
}

extern int colorprintf(TerminalColor color, const char *format, ...)
{
	int r;
	va_list va;
	va_start(va, format);
	text_color(color);
	r = vprintf(format, va);
	text_color(WHITE);
	va_end(va);
	return r;
}
