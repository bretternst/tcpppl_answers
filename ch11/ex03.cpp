#include <iostream>

class INT
{
	int i;

public:
	INT(const int ii) : i(ii) {}
	operator int() {return i;}
	INT operator++(int){INT i2=*this;i++;return i2;}
	INT& operator++(){i++; return *this;}
	INT operator--(int){INT i2=*this;i--;return i2;}
	INT& operator--(){i--; return *this;}
	INT& operator+=(const int x){i+=x; return *this;}
	INT& operator-=(const int x){i-=x; return *this;}
	friend std::istream& operator>>(std::istream&, INT&);
};

std::istream& operator>>(std::istream& input, INT& x)
{
	input >> x.i;
	return input;
}

void main()
{
	using namespace std;

	INT x = 2;
	INT y = 3;
	cout << x + 1 << endl;
	cout << 1 + x << endl;
	cout << x + y << endl;
	cout << x * y << endl;
	cout << x / y << endl;
	cout << x % y << endl;
	cout << x << endl;
	x = 4 + 4;
	cout << x << endl;
	int z = 8;
	x = z;
	cout << x << endl;
	x++;
	z = x;
	cout << z << endl;
	cout << x++ << endl;
	cout << ++x << endl;
	cout << x-- << endl;
	cout << --x << endl;
	x += 1;
	cout << x << endl;
	x -= 1;
	cout << x << endl;
	cout << -x << endl;
	cin >> y; // Reference parameters must be exact, so the overload is required!
	cout << y + 1 << endl;
}
