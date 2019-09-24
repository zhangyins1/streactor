#ifndef __MISC_H_H__
#define __MISC_H_H__
#include <string.h>

#define st_memzero(buf, n)       (void) memset(buf, 0, n)
#define st_free free
#define st_delete delete
#define is_int_in_vec(n,vec) (std::find((vec).begin(), (vec).end(), (n)) != (vec).end())
#define SAFE_DEL(p) {if((p)) {delete (p), (p) = NULL;} }

void *st_alloc(size_t size);

extern std::string& Ltrim(std::string& s);
extern std::string& Rtrim(std::string& s);

inline std::string& Trim(std::string& s)
{
	return Rtrim(Ltrim(s));
}

//É¾³ý×Ö·û´®×ó±ßµÄ¿Õ¸ñ
extern char* Ltrim(char* p);

//É¾³ý×Ö·û´®ÓÒ±ßµÄ¿Õ¸ñ
extern char* Rtrim(char* p);

//É¾³ý×Ö·û´®Á½±ßµÄ¿Õ¸ñ
inline char* Trim(char* s)
{
	return Rtrim(Ltrim(s));
}


#endif  // !__MISC_H_H__