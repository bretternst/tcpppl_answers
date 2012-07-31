#include <iostream>
#include <limits>

using namespace std;

template <class T> void printMax(const T max, const char *tName)
{
	cout << "max " << tName << " = " << max << hex << " 0x" << max << dec << endl;
}
template <> void printMax<char>(const char max, const char *tName)
{
	cout << "max " << tName << " = " << (short)max << hex << " 0x" << (short)max << dec << endl;
}

template <class T> void limit(const char* tName)
{
	printMax<T>(std::numeric_limits<T>::max(), tName);
};

int main()
{
	limit<char>("char");
	limit<short>("short");
	limit<int>("int");
	limit<long>("long");
	limit<float>("float");
	limit<double>("double");
	limit<long double>("long double");
	limit<unsigned>("unsigned");
}
