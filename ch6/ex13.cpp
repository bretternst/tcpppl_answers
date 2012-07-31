#include <iostream>

using namespace std;

int strlen_(const char *s)
{
    const char *p = s;
    while(*p) p++;
    return p - s;
}

char* cat_(const char* s1, const char* s2)
{
	char* s = new char[strlen_(s1)+strlen_(s2)+1];
	int i = 0;
	while(*s1)
	{
		s[i++]=*s1++;
	}
	while(*s2)
	{
		s[i++]=*s2++;
	}
	s[i] = 0;
	return s;
}

int main(int argc, char* argv[])
{
	if(argc < 3) return -1;

	cout << cat_(argv[1],argv[2]) << endl;
}
