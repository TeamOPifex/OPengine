#pragma once

#include "./Core/include/OPmemory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef OPIFEX_WINDOWS
	#include <windows.h>
	#include <conio.h>
#else
	#include <termios.h>
	#include <stdio.h>

	static struct termios old, _new;

	/* Initialize new terminal i/o settings */
	void initTermios(int echo) 
	{
	  tcgetattr(0, &old); /* grab old terminal i/o settings */
	  _new = old; /* make new settings same as old settings */
	  _new.c_lflag &= ~ICANON; /* disable buffered i/o */
	  _new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	  tcsetattr(0, TCSANOW, &_new); /* use these new terminal i/o settings now */
	}

	/* Restore old terminal i/o settings */
	void resetTermios(void) 
	{
	  tcsetattr(0, TCSANOW, &old);
	}

	/* Read 1 character - echo defines echo mode */
	char getch_(int echo) 
	{
	  char ch;
	  initTermios(echo);
	  ch = getchar();
	  resetTermios();
	  return ch;
	}

	/* Read 1 character without echo */
	char getch(void) 
	{
	  return getch_(0);
	}
#endif

void MenuOptions(i8* header, OPint* selections, ui16 children, ...) {
	va_list args;
	i8 chr = 0x00;
	i32 tmp;
	i32 opt = 1;

	do
	{
		
#ifdef OPIFEX_WINDOWS
		system("cls");
#else
		system("clear");
#endif
		OPlog(header);

		if (chr == 0x48 || chr == 0x4B || chr == 'w' || chr =='a')
			(opt>1 && opt != 1) ? opt-- : (opt = children);
		else if (chr == 0x50 || chr == 0x4D || chr == 's' || chr == 'd')
			(opt >= 1 && opt != children + 1) ? opt++ : (opt = 1);
		else if (chr == ' ') {
			selections[opt - 1] = !selections[opt - 1];
		}
		if (opt == (children + 1) && chr == ' ') {
			break;
		}

		va_start(args, children);


		for (tmp = 1; tmp <= children; tmp++)
		{

			i8* r = va_arg(args, i8*);

			i8* format = "%s [ ] %s";
			if (selections[tmp - 1]) {
				format = "%s [*] %s";
			}

			if (opt == tmp) {
				OPlog(format, "->", r);
			}
			else {
				OPlog(format, "  ", r);
			}

		}

		if (opt == (children + 1)) {
			OPlog("\n-> Begin Export");
		}
		else {
			OPlog("\n   Begin Export");
		}


		va_end(args);

		//Sleep(5);
	} while ((chr = getch()) != 0x0D);
}