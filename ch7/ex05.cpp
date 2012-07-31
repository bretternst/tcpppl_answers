/* 
#include <stdlib.h>
#include <stdio.h>

#define HELLO_WORLD "Hello world!"

int main(void)
{
	void *p = malloc(13);

    printf("%s\n", HELLO_WORLD);
    return 0;

	free(p);
}
*/

#include <iostream>
#include <string>

using namespace std;

const string HelloWorld("Hello, world!");

int main(void)
{
	char *p = new char[13];

	cout << HelloWorld << endl;

	delete[] p;
}
