#include <iostream>
#include <limits>

using std::ostream;

// I intentionally avoid using "long long" here, as it is not
// a standard type.
class LINT
{
	bool neg;
	unsigned short d[4];

	class OverflowError {};
	class DivideByZeroError {};

	void Clear()
	{
		for(int i = 0; i < 4; i++) d[i] = 0;
		neg = 0;
	}

	void BitShiftLeft()
	{
		for(int i = 3; i >= 0; i--)
		{
			d[i] <<= 1;
			if(i > 0 && (d[i-1] & 0x8000))
				d[i] |= 1;
		}
	}

	void BitShiftRight()
	{
		for(int i = 0; i <= 3; i++)
		{
			d[i] >>= 1;
			if(i < 3 && (d[i+1] & 1))
				d[i] |= 0x8000;
		}
	}

	void Invert()
	{
		for(int i = 0; i <= 3; i++)
			d[i] = ~d[i];
	}

	int DigitCount() const
	{
		for(int i = 3; i >= 0; i--)
			if(d[i]!=0) return i+1;
		return 0;
	}

	unsigned short SafeGetDigit(int idx) const
	{
		if(idx < 0 || idx > 3) return 0;
		return d[idx];
	}

	int BasicCompare(const LINT& x) const
	{
		for(int i = 3; i >= 0; i--)
		{
			if(d[i] < x.d[i]) return -1;
			if(d[i] > x.d[i]) return 1;
		}
		return 0;
	}

	void BasicAdd(const LINT& x)
	{
		int r = 0;
		for(int i = 0; i < 4; i++)
		{
			r = d[i] + x.d[i] + r;
			d[i] = r % 0x10000;
			r = r / 0x10000;
		}
		if(r > 0)
			throw OverflowError();
	}

	void BasicSubtract(const LINT& x)
	{
		int r = 0;
		for(int i = 0; i < 4; i++)
		{
			r = d[i] - x.d[i] - r;
			if (r < 0)
			{
				d[i] = 0x10000 + r;
				r = 1;
			}
			else
			{
				d[i] = r;
				r = 0;
			}
		}
		if(r > 0)
			throw OverflowError();
	}

	void Add(const LINT& x)
	{
		if(neg == x.neg)
		{
			BasicAdd(x);
		}
		else if (BasicCompare(x) < 0)
		{
			LINT li = *this;
			*this = x;
			BasicSubtract(li);
			neg = !li.neg;
		}
		else if (BasicCompare(x) > 0)
		{
			BasicSubtract(x);
		}
		else
		{
			*this = 0; 
		}
		if(BasicCompare(0)==0) neg = false;
	}

	void Subtract(const LINT& x)
	{
		if (neg == x.neg)
		{
			if(BasicCompare(x) < 0)
			{
				LINT li = *this;
				*this = x;
				BasicSubtract(li);
				neg = !li.neg;
			}
			else if (BasicCompare(x) > 0)
			{
				BasicSubtract(x);
			}
			else
			{
				*this = 0;
			}
		}
		else
		{
			BasicAdd(x);
		}
		if(BasicCompare(0)==0) neg = false;
	}

	void Multiply(const LINT& m)
	{
		unsigned int r = 0;
		LINT y(0), x(0);

		// Make sure the operand with the most digits is
		// processed in the outer loop.
		if(DigitCount() > x.DigitCount())
		{
			y = m;
			x = *this;
		}
		else
		{
			x = m;
			y = *this;
		}
		Clear();
		LINT li(0);

		for(int i = 0; i < 4; i++)
		{
			r = 0;
			for(int j = 0; j < 4; j++)
			{
				r = x.d[i] * y.d[j] + r;

				if(r==0) break;
				if(i+j > 3) throw OverflowError();

				li.d[j+i] = r % 0x10000;
				r = r / 0x10000;
			}
			BasicAdd(li);
			li = LINT(0);
		}
		neg = x.neg != y.neg;
		if(BasicCompare(0)==0) neg = false;
	}

	// The general algorithm employed here was inspired by:

	void Divide(const LINT& x, LINT& quotient, LINT& remainder)
	{
		if(BasicCompare(x) < 0)
		{
			remainder = x;
			return;
		}
		else if(BasicCompare(x) == 0)
		{
			quotient = 1;
			quotient.neg = neg != x.neg;
			return;
		}
		else if (x == 0)
		{
			throw DivideByZeroError();
		}

		LINT dividend(*this);
		LINT divisor(x);
		quotient = remainder = 0;
		int zeroCount = 0;

		while(divisor.BasicCompare(dividend) < 0)
		{
			divisor.BitShiftLeft();
			zeroCount++;
		}
		if(divisor.BasicCompare(dividend) > 0)
		{
			divisor.BitShiftRight();
			zeroCount--;
		}

		if(zeroCount >= 0)
		{
			for(int i = 0; i <= zeroCount; i++)
			{
				if(divisor.BasicCompare(dividend) <= 0)
				{
					dividend.BasicSubtract(divisor);
					divisor.BitShiftRight();
					quotient.BitShiftLeft();
					quotient.BasicAdd(1);
				}
				else
				{
					divisor.BitShiftRight();
					quotient.BitShiftLeft();
				}
			}
		}

		remainder = dividend;

		quotient.neg = neg != x.neg;
		if(quotient.BasicCompare(0)==0) quotient.neg = false;
		if(remainder.BasicCompare(0)==0) remainder.neg = false;
	}

	void Divide(const LINT& x)
	{
		LINT quotient(0), remainder(0);
		Divide(x, quotient, remainder);
		*this = quotient;
	}

	void Modulus(const LINT& x)
	{
		LINT quotient(0), remainder(0);
		Divide(x, quotient, remainder);
		*this = remainder;
	}

public:
	LINT(int ii) {
		d[0] = abs(ii) & 0xffff;
		d[1] = abs(ii) >> 16;
		d[2] = 0;
		d[3] = 0;
		neg = ii < 0;
	}

	LINT(const LINT& li)
	{
		for(int i=0;i<4;i++)
			d[i]=li.d[i];
		neg = li.neg;
	}

	LINT operator+() { return *this; }

	LINT operator-(const LINT& x)
	{
		LINT li = *this;
		li.Subtract(x);
		return li;
	}

	LINT operator+(const LINT& x) 
	{
		LINT li = *this;
		li.Add(x);
		return li;
	}

	LINT operator*(const LINT& x)
	{
		LINT li = *this;
		li.Multiply(x);
		return li;
	}

	bool operator<(const LINT& x)
	{
		if(neg==x.neg) return BasicCompare(x) < 0;
		else if (neg && !x.neg) return true;
		else return false;
	}

	bool operator>(const LINT& x)
	{
		if(neg==x.neg) return BasicCompare(x) > 0;
		else if (!neg && x.neg) return true;
		else return false;
	}

	bool operator==(const LINT& x) const
	{
		if(neg != x.neg) return false;
		return BasicCompare(x) == 0;
	}

	bool operator==(int x) const
	{
		return *this == LINT(x);
	}

	LINT& operator=(const LINT& x)
	{
		for(int i = 0; i < 4; i++)
		{
			d[i] = x.d[i];
		}
		neg = x.neg;
		return *this;
	}

	LINT& operator=(const int i)
	{
		*this = LINT(i);
		return *this;
	}

	LINT operator/(const LINT& x) {
		LINT li = *this;
		li.Divide(x);
		return li;
	}

	LINT operator%(const LINT& x) {
		LINT li = *this;
		li.Modulus(x);
		return li;
	}

	friend ostream& operator<<(ostream& out, const LINT& x)
	{
		LINT li = x;
		if (li.neg)
		{
			li.Invert();
			li.BasicAdd(1);
		}

		out.fill('0');
		out << std::hex;
		for(int i = 3; i >= 0; i--)
		{
			out.width(4);
			out << li.d[i];
		}
		return out;
	}
};

void main()
{
	using namespace std;

	cout << "All positives:" << endl;
	cout << (LINT(0x100) + LINT(0xffff)) << ' ' << (0x100 + 0xffff) << endl;
	cout << (LINT(0xffff) + LINT(0xffff)) << ' ' << (0xffff + 0xffff) << endl;
	cout << (LINT(0x1ffff) + LINT(0xfffe)) << ' ' << (0x1ffff + 0xfffe) << endl;
	cout << (LINT(0xfffff) + LINT(0xfffff)) << ' ' << (0xfffff + 0xfffff) << endl;
	cout << (LINT(0x7ffff) + LINT(0x8ffff)) << ' ' << (0x7ffff + 0x8ffff) << endl;

	cout << (LINT(0xffff) - LINT(0x100)) << ' ' << (0xffff - 0x100) << endl;
	cout << (LINT(0xffff) - LINT(0xffff)) << ' ' << (0xffff - 0xffff) << endl;
	cout << (LINT(0x1ffff) - LINT(0xfffe)) << ' ' << (0x1ffff - 0xfffe) << endl;
	cout << (LINT(0xfffff) - LINT(0xfffff)) << ' ' << (0xfffff - 0xfffff) << endl;
	cout << (LINT(0x8ffff) - LINT(0x7ffff)) << ' ' << (0x8ffff - 0x7ffff) << endl;

	cout << (LINT(0xff) * LINT(0x100)) << ' ' << (0xff * 0x100) << endl;
	cout << (LINT(0xff) * LINT(0xffff)) << ' ' << (0xff * 0xffff) << endl;
	cout << (LINT(0x1ff) * LINT(0xfffe)) << ' ' << (0x1ff * 0xfffe) << endl;
	cout << (LINT(0xffe) * LINT(0xfffff)) << ' ' << (0xffe * 0xfffff) << endl;
	cout << (LINT(0x8fff) * LINT(0x7ffff)) << ' ' << (0x8fff * 0x7ffff) << endl;
	cout << (LINT(0x10000) * LINT(0xffff)) << ' ' << (0x10000 * 0xffff) << endl;

	cout << (LINT(2) / LINT(1)) << ' ' << (0x2 / 0x1) << endl;
	cout << (LINT(0x20) / LINT(0x5)) << ' ' << (0x20 / 0x5) << endl;
	cout << (LINT(0x10000) / LINT(0x2)) << ' ' << (0x10000 / 0x2) << endl;
	cout << (LINT(0x20000) / LINT(0x3)) << ' ' << (0x20000 / 0x3) << endl;
	cout << (LINT(0x20000) / LINT(0xffff)) << ' ' << (0x20000 / 0xffff) << endl;
	cout << (LINT(0x20000) / LINT(0x10000)) << ' ' << (0x20000 / 0x10000) << endl;
	cout << (LINT(16) / LINT(5)) << ' ' << (16 / 5) << endl;

	cout << (LINT(2) % LINT(1)) << ' ' << (0x2 % 0x1) << endl;
	cout << (LINT(0x20) % LINT(0x5)) << ' ' << (0x20 % 0x5) << endl;
	cout << (LINT(0x10000) % LINT(0x2)) << ' ' << (0x10000 % 0x2) << endl;
	cout << (LINT(0x20000) % LINT(0x3)) << ' ' << (0x20000 % 0x3) << endl;
	cout << (LINT(0x20000) % LINT(0xffff)) << ' ' << (0x20000 % 0xffff) << endl;
	cout << (LINT(0x20000) % LINT(0x10000)) << ' ' << (0x20000 % 0x10000) << endl;
	cout << (LINT(16) % LINT(5)) << ' ' << (16 % 5) << endl;

	cout << "All negatives:" << endl;
	cout << (LINT(-0x100) + LINT(-0xffff)) << ' ' << (-0x100 + -0xffff) << endl;
	cout << (LINT(-0xffff) + LINT(-0xffff)) << ' ' << (-0xffff + -0xffff) << endl;
	cout << (LINT(-0x1ffff) + LINT(-0xfffe)) << ' ' << (-0x1ffff + -0xfffe) << endl;
	cout << (LINT(-0xfffff) + LINT(-0xfffff)) << ' ' << (-0xfffff + -0xfffff) << endl;
	cout << (LINT(-0x7ffff) + LINT(-0x8ffff)) << ' ' << (-0x7ffff + -0x8ffff) << endl;

	cout << (LINT(-0xffff) - LINT(-0x100)) << ' ' << (-0xffff - -0x100) << endl;
	cout << (LINT(-0xffff) - LINT(-0xffff)) << ' ' << (-0xffff - -0xffff) << endl;
	cout << (LINT(-0x1ffff) - LINT(-0xfffe)) << ' ' << (-0x1ffff - -0xfffe) << endl;
	cout << (LINT(-0xfffff) - LINT(-0xfffff)) << ' ' << (-0xfffff - -0xfffff) << endl;
	cout << (LINT(-0x8ffff) - LINT(-0x7ffff)) << ' ' << (-0x8ffff - -0x7ffff) << endl;

	cout << (LINT(-0xff) * LINT(-0x100)) << ' ' << (-0xff * -0x100) << endl;
	cout << (LINT(-0xff) * LINT(-0xffff)) << ' ' << (-0xff * -0xffff) << endl;
	cout << (LINT(-0x1ff) * LINT(-0xfffe)) << ' ' << (-0x1ff * -0xfffe) << endl;
	cout << (LINT(-0xffe) * LINT(-0xfffff)) << ' ' << (-0xffe * -0xfffff) << endl;
	cout << (LINT(-0x8fff) * LINT(-0x7ffff)) << ' ' << (-0x8fff * -0x7ffff) << endl;
	cout << (LINT(-0x10000) * LINT(-0xffff)) << ' ' << (-0x10000 * -0xffff) << endl;

	cout << (LINT(-2) / LINT(-1)) << ' ' << (-0x2 / -0x1) << endl;
	cout << (LINT(-0x20) / LINT(-0x5)) << ' ' << (-0x20 / -0x5) << endl;
	cout << (LINT(-0x10000) / LINT(-0x2)) << ' ' << (-0x10000 / -0x2) << endl;
	cout << (LINT(-0x20000) / LINT(-0x3)) << ' ' << (-0x20000 / -0x3) << endl;
	cout << (LINT(-0x20000) / LINT(-0xffff)) << ' ' << (-0x20000 / -0xffff) << endl;
	cout << (LINT(-0x20000) / LINT(-0x10000)) << ' ' << (-0x20000 / -0x10000) << endl;
	cout << (LINT(-16) / LINT(-5)) << ' ' << (-16 / -5) << endl;

	cout << (LINT(-2) % LINT(-1)) << ' ' << (-0x2 % -0x1) << endl;
	cout << (LINT(-0x20) % LINT(-0x5)) << ' ' << (-0x20 % -0x5) << endl;
	cout << (LINT(-0x10000) % LINT(-0x2)) << ' ' << (-0x10000 % -0x2) << endl;
	cout << (LINT(-0x20000) % LINT(-0x3)) << ' ' << (-0x20000 % -0x3) << endl;
	cout << (LINT(-0x20000) % LINT(-0xffff)) << ' ' << (-0x20000 % -0xffff) << endl;
	cout << (LINT(-0x20000) % LINT(-0x10000)) << ' ' << (-0x20000 % -0x10000) << endl;
	cout << (LINT(-16) % LINT(-5)) << ' ' << (-16 % -5) << endl;

	cout << "Left negative:" << endl;
	cout << (LINT(-0x100) + LINT(0xffff)) << ' ' << (-0x100 + 0xffff) << endl;
	cout << (LINT(-0xffff) + LINT(0xffff)) << ' ' << (-0xffff + 0xffff) << endl;
	cout << (LINT(-0x1ffff) + LINT(0xfffe)) << ' ' << (-0x1ffff + 0xfffe) << endl;
	cout << (LINT(-0xfffff) + LINT(0xfffff)) << ' ' << (-0xfffff + 0xfffff) << endl;
	cout << (LINT(-0x7ffff) + LINT(0x8ffff)) << ' ' << (-0x7ffff + 0x8ffff) << endl;

	cout << (LINT(-0xffff) - LINT(0x100)) << ' ' << (-0xffff - 0x100) << endl;
	cout << (LINT(-0xffff) - LINT(0xffff)) << ' ' << (-0xffff - 0xffff) << endl;
	cout << (LINT(-0x1ffff) - LINT(0xfffe)) << ' ' << (-0x1ffff - 0xfffe) << endl;
	cout << (LINT(-0xfffff) - LINT(0xfffff)) << ' ' << (-0xfffff - 0xfffff) << endl;
	cout << (LINT(-0x8ffff) - LINT(0x7ffff)) << ' ' << (-0x8ffff - 0x7ffff) << endl;

	cout << (LINT(-0xff) * LINT(0x100)) << ' ' << (-0xff * 0x100) << endl;
	cout << (LINT(-0xff) * LINT(0xffff)) << ' ' << (-0xff * 0xffff) << endl;
	cout << (LINT(-0x1ff) * LINT(0xfffe)) << ' ' << (-0x1ff * 0xfffe) << endl;
	cout << (LINT(-0xffe) * LINT(0xfffff)) << ' ' << (-0xffe * 0xfffff) << endl;
	cout << (LINT(-0x8fff) * LINT(0x7ffff)) << ' ' << (-0x8fff * 0x7ffff) << endl;
	cout << (LINT(-0x10000) * LINT(0xffff)) << ' ' << (-0x10000 * 0xffff) << endl;

	cout << (LINT(-2) / LINT(1)) << ' ' << (-0x2 / 0x1) << endl;
	cout << (LINT(-0x20) / LINT(0x5)) << ' ' << (-0x20 / 0x5) << endl;
	cout << (LINT(-0x10000) / LINT(0x2)) << ' ' << (-0x10000 / 0x2) << endl;
	cout << (LINT(-0x20000) / LINT(0x3)) << ' ' << (-0x20000 / 0x3) << endl;
	cout << (LINT(-0x20000) / LINT(0xffff)) << ' ' << (-0x20000 / 0xffff) << endl;
	cout << (LINT(-0x20000) / LINT(0x10000)) << ' ' << (-0x20000 / 0x10000) << endl;
	cout << (LINT(-16) / LINT(5)) << ' ' << (-16 / 5) << endl;

	cout << (LINT(-2) % LINT(1)) << ' ' << (-0x2 % 0x1) << endl;
	cout << (LINT(-0x20) % LINT(0x5)) << ' ' << (-0x20 % 0x5) << endl;
	cout << (LINT(-0x10000) % LINT(0x2)) << ' ' << (-0x10000 % 0x2) << endl;
	cout << (LINT(-0x20000) % LINT(0x3)) << ' ' << (-0x20000 % 0x3) << endl;
	cout << (LINT(-0x20000) % LINT(0xffff)) << ' ' << (-0x20000 % 0xffff) << endl;
	cout << (LINT(-0x20000) % LINT(0x10000)) << ' ' << (-0x20000 % 0x10000) << endl;
	cout << (LINT(-16) % LINT(5)) << ' ' << (-16 % 5) << endl;

	cout << "Right negative:" << endl;
	cout << (LINT(0x100) + LINT(-0xffff)) << ' ' << (0x100 + -0xffff) << endl;
	cout << (LINT(0xffff) + LINT(-0xffff)) << ' ' << (0xffff + -0xffff) << endl;
	cout << (LINT(0x1ffff) + LINT(-0xfffe)) << ' ' << (0x1ffff + -0xfffe) << endl;
	cout << (LINT(0xfffff) + LINT(-0xfffff)) << ' ' << (0xfffff + -0xfffff) << endl;
	cout << (LINT(0x7ffff) + LINT(-0x8ffff)) << ' ' << (0x7ffff + -0x8ffff) << endl;

	cout << (LINT(0xffff) - LINT(-0x100)) << ' ' << (0xffff - -0x100) << endl;
	cout << (LINT(0xffff) - LINT(-0xffff)) << ' ' << (0xffff - -0xffff) << endl;
	cout << (LINT(0x1ffff) - LINT(-0xfffe)) << ' ' << (0x1ffff - -0xfffe) << endl;
	cout << (LINT(0xfffff) - LINT(-0xfffff)) << ' ' << (0xfffff - -0xfffff) << endl;
	cout << (LINT(0x8ffff) - LINT(-0x7ffff)) << ' ' << (0x8ffff - -0x7ffff) << endl;

	cout << (LINT(0xff) * LINT(-0x100)) << ' ' << (0xff * -0x100) << endl;
	cout << (LINT(0xff) * LINT(-0xffff)) << ' ' << (0xff * -0xffff) << endl;
	cout << (LINT(0x1ff) * LINT(-0xfffe)) << ' ' << (0x1ff * -0xfffe) << endl;
	cout << (LINT(0xffe) * LINT(-0xfffff)) << ' ' << (0xffe * -0xfffff) << endl;
	cout << (LINT(0x8fff) * LINT(-0x7ffff)) << ' ' << (0x8fff * -0x7ffff) << endl;
	cout << (LINT(0x10000) * LINT(-0xffff)) << ' ' << (0x10000 * -0xffff) << endl;

	cout << (LINT(2) / LINT(-1)) << ' ' << (0x2 / -0x1) << endl;
	cout << (LINT(0x20) / LINT(-0x5)) << ' ' << (0x20 / -0x5) << endl;
	cout << (LINT(0x10000) / LINT(-0x2)) << ' ' << (0x10000 / -0x2) << endl;
	cout << (LINT(0x20000) / LINT(-0x3)) << ' ' << (0x20000 / -0x3) << endl;
	cout << (LINT(0x20000) / LINT(-0xffff)) << ' ' << (0x20000 / -0xffff) << endl;
	cout << (LINT(0x20000) / LINT(-0x10000)) << ' ' << (0x20000 / -0x10000) << endl;
	cout << (LINT(16) / LINT(-5)) << ' ' << (16 / -5) << endl;

	cout << (LINT(2) % LINT(-1)) << ' ' << (0x2 % -0x1) << endl;
	cout << (LINT(0x20) % LINT(-0x5)) << ' ' << (0x20 % -0x5) << endl;
	cout << (LINT(0x10000) % LINT(-0x2)) << ' ' << (0x10000 % -0x2) << endl;
	cout << (LINT(0x20000) % LINT(-0x3)) << ' ' << (0x20000 % -0x3) << endl;
	cout << (LINT(0x20000) % LINT(-0xffff)) << ' ' << (0x20000 % -0xffff) << endl;
	cout << (LINT(0x20000) % LINT(-0x10000)) << ' ' << (0x20000 % -0x10000) << endl;
	cout << (LINT(16) % LINT(-5)) << ' ' << (16 % -5) << endl;
}
