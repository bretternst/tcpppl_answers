#include <iostream>

class RINT
{
	int i;
public:
	explicit RINT(int ii) : i(ii) {}

	RINT operator-() { return RINT(-i); }
	RINT operator+() { return RINT(+i); }
	RINT operator-(const RINT& x) { return RINT(i-x.i); }
	RINT operator+(const RINT& x) { return RINT(i+x.i); }
	RINT operator/(const RINT& x) { return RINT(i/x.i); }
	RINT operator*(const RINT& x) { return RINT(i*x.i); }
	RINT operator%(const RINT& x) { return RINT(i%x.i); }

	int Value() { return i; }
};

void main()
{
	using namespace std;

	RINT x(2);
	RINT y(3);
	cout << (-x).Value() << endl;
	cout << (+y).Value() << endl;
	cout << (y-x).Value() << endl;
	cout << (y+x).Value() << endl;
	cout << (y/x).Value() << endl;
	cout << (y*x).Value() << endl;
	cout << (y%x).Value() << endl;
}
