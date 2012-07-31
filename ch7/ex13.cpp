#include <iostream>

using namespace std;

// The Google style guide located at:
// http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml#Naming
// has some good naming convention rules for C++. 
// In reality, a number of different styles are used.

struct MixedCase
{
	int lowercase;
	int pascalCase;
	int multi_word;
};

const int ALL_CAPS_CONST = 5;
const int kMyConstant = 5;

enum MyEnum
{
	ALL_CAPS_ENUM_VALUE
};

void SomeFunction()
{
}

void set_variable()
{
}

void someFunction()
{
}

int main(int argc, char* argv[])
{
	char* szHungarianNotation;
}
