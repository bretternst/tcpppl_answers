#include <iostream>

using namespace std;

void func01(const char *a1, int &a2);
typedef void (*pfunc01)(const char*,int&);
void func02(pfunc01 a1);
pfunc01 func03();
pfunc01 func04(pfunc01 p)
{
	int i = 0;
	p("", i);
    return p;
}

int main(int argc, char* argv[])
{
}
