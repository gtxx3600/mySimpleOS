#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "fs.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

int printf(const char *fmt, ...)
{
	int i;
	char buf[256];

	va_list arg = (va_list)((char*)(&fmt) + 4); /*4是参数fmt所占堆栈中的大小*/
	i = vsprintf(buf, fmt, arg);
	buf[i]=0;
	printx(buf);

	return i;
}

