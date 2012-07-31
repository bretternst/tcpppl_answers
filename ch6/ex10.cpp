#include <iostream>

using namespace std;

int strlen_(const char *s)
{
    const char *p = s;
    while(*p) p++;
    return p - s;
}

void strcpy_(char *dest, const char* src)
{
    while (*src) {
        *dest++ = *src++;
    }
}

int strcmp_(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        if (int d = *s1++ - *s2++)
            return d;
    }
    return *s1 - *s2;
}

int main()
{
	cout << strlen_("") << endl;
	cout << strlen_("hello") << endl;

	char dest[10];
	strcpy_(dest, "");
	cout << dest << endl;

	strcpy_(dest,"hello");
	cout << dest << endl;

	cout << strcmp_("", "hello") << endl;
	cout << strcmp_("hello", "") << endl;
	cout << strcmp_("abc", "abc") << endl;
	cout << strcmp_("abc","abd") << endl;
}
