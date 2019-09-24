#include <stdlib.h>
#include <ctype.h>
#include <algorithm>   
#include <functional>
#include "Misc.h"

#include "Log.h"

using std::string;
using std::not1;
using std::ptr_fun;

void *st_alloc(size_t size)
{
	void  *p;

	p = malloc(size);
	if (p == NULL) {
		LOG_ERROR("error on malloc.", size);
	}

	return p;
}

string& Ltrim(string& s)
{
	int(*func)(int) = isspace;

	string::iterator iter;
	iter = find_if(s.begin(), s.end(), not1(ptr_fun(func)));
	s.erase(s.begin(), iter);

	return s;
}


string& Rtrim(string& s)
{
	int(*func)(int) = isspace;

	string::reverse_iterator iter;
	iter = find_if(s.rbegin(), s.rend(), not1(ptr_fun(func)));
	s.erase(iter.base(), s.end());

	return s;
}

//É¾³ý×Ö·û´®ÓÒ±ßµÄ¿Õ¸ñ
char* Rtrim(char* p)
{
	if (p == NULL)
	{
		return p;
	}

	size_t n = strlen(p);
	if (n == 0)
	{
		return p;
	}

	char* q = p + n - 1;

	while (isspace(*q))
	{
		--q;
	}

	*(q + 1) = '\0';

	return p;
}

//É¾³ý×Ö·û´®×ó±ßµÄ¿Õ¸ñ
char* Ltrim(char* p)
{
	if (p == NULL)
	{
		return p;
	}

	char* q = p;

	while (isspace(*q))
	{
		++q;
	}

	if (p != q)
	{
		while (*p++ = *q++) {}
	}

	return p;
}
