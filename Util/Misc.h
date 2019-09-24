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

//ɾ���ַ�����ߵĿո�
extern char* Ltrim(char* p);

//ɾ���ַ����ұߵĿո�
extern char* Rtrim(char* p);

//ɾ���ַ������ߵĿո�
inline char* Trim(char* s)
{
	return Rtrim(Ltrim(s));
}


#endif  // !__MISC_H_H__