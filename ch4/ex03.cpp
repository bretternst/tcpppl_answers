#include <iostream>

int main()
{
	std::cout << "bool = " << sizeof(bool) << std::endl;
	std::cout << "signed char = " << sizeof(signed char) << std::endl;
	std::cout << "unsigned char = " << sizeof(unsigned char) << std::endl;
	std::cout << "signed short = " << sizeof(signed short) << std::endl;
	std::cout << "unsigned short = " << sizeof(unsigned short) << std::endl;
	std::cout << "signed int = " << sizeof(signed int) << std::endl;
	std::cout << "unsigned int = " << sizeof(unsigned int) << std::endl;
	std::cout << "signed long = " << sizeof(signed long) << std::endl;
	std::cout << "unsigned long = " << sizeof(unsigned long) << std::endl;
	std::cout << "signed long long = " << sizeof(signed long long) << std::endl;
	std::cout << "unsigned long long = " << sizeof(signed long long) << std::endl;
	std::cout << "float = " << sizeof(float) << std::endl;
	std::cout << "double = " << sizeof(double) << std::endl;
	std::cout << "long double = " << sizeof(long double) << std::endl;
	std::cout << "wchar_t = " << sizeof(wchar_t) << std::endl;
	std::cout << "char * = " << sizeof(char *) << std::endl;
	std::cout << "int * = " << sizeof(int *) << std::endl;

	enum Enum1 { a };
	enum Enum2 { b, c, d, e };

	std::cout << "Enum1 = " << sizeof(Enum1) << std::endl;
	std::cout << "Enum2 = " << sizeof(Enum2) << std::endl;
}
