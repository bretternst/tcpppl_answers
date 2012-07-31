#include <iostream>
#include <limits>
#include <vector>

using std::ostream;

namespace Exercises
{
	class BIGINT
	{
		class IntRep;
		IntRep* rep;

		void Clear();
		void BitShiftLeft();
		void BitShiftRight();
		void Invert();
		unsigned short SafeGetDigit(unsigned int idx) const;
		int BasicCompare(const BIGINT& x) const;
		void BasicAdd(const BIGINT& x);
		void BasicSubtract(const BIGINT& x);
		void Add(const BIGINT& x);
		void Subtract(const BIGINT& x);
		void Multiply(const BIGINT& m);
		void Divide(const BIGINT& x, BIGINT& quotient, BIGINT& remainder);
		void Divide(const BIGINT& x);
		void Modulus(const BIGINT& x);

	public:
		class OverflowError {};
		class DivideByZeroError {};

		BIGINT(int ii);
		BIGINT(const BIGINT& li);
		~BIGINT();
		BIGINT operator+();
		BIGINT operator-();
		BIGINT operator-(const BIGINT& x);
		BIGINT operator+(const BIGINT& x);
		BIGINT operator*(const BIGINT& x);
		bool operator<(const BIGINT& x);
		bool operator>(const BIGINT& x);
		bool operator==(const BIGINT& x) const;
		bool operator==(int x) const;
		BIGINT& operator=(const BIGINT& x);
		BIGINT& operator=(const int i);
		BIGINT operator/(const BIGINT& x);
		BIGINT operator%(const BIGINT& x);
		friend ostream& operator<<(ostream& out, const BIGINT& x);
	};

	class BIGINT::IntRep
	{
		friend class BIGINT; 
		friend ostream& operator<<(ostream& out, const BIGINT& x);

		int n;
		bool neg;

		typedef std::vector<unsigned short> DigitList;
		DigitList d;

		IntRep() : n(1), neg(false)
		{
		}

		IntRep(bool neg, const DigitList& x) : n(1), neg(neg)
		{
			d.insert(d.begin(), x.begin(), x.end());
		}

		IntRep* Copy()
		{
			if(n==1) return this;
			n--;
			return new IntRep(neg, d);
		}

		void Normalize()
		{
			while(d.size() > 0 && *(d.end()-1) == 0) d.pop_back();
		}

	private:
		IntRep(const IntRep&);
		IntRep& operator=(const IntRep&);
	};

	void BIGINT::Clear()
	{
		rep->d.clear();
		rep->neg = 0;
	}

	void BIGINT::BitShiftLeft()
	{
		rep->d.push_back(0);
		for(int i = rep->d.size(); i > 0; i--)
		{
			rep->d[i-1] <<= 1;
			if(i > 1 && (rep->d[i-2] & 0x8000))
				rep->d[i-1] |= 1;
		}
		rep->Normalize();
	}

	void BIGINT::BitShiftRight()
	{
		int sz = rep->d.size() - 1;

		for(int i = 0; i <= sz; i++)
		{
			rep->d[i] >>= 1;
			if(i < sz && (rep->d[i+1] & 1))
				rep->d[i] |= 0x8000;
		}
		rep->Normalize();
	}

	void BIGINT::Invert()
	{
		for(unsigned int i = 0; i <= rep->d.size()-1; i++)
			rep->d[i] = ~rep->d[i];
		rep->Normalize();
	}

	unsigned short BIGINT::SafeGetDigit(unsigned int idx) const
	{
		if(idx < 0 || idx+1 > rep->d.size()) return 0;
		return rep->d[idx];
	}

	int BIGINT::BasicCompare(const BIGINT& x) const
	{
		if(rep->d.size() > x.rep->d.size()) return 1;
		else if (rep->d.size() < x.rep->d.size()) return -1;

		for(int i = rep->d.size(); i > 0; i--)
		{
			if(rep->d[i-1] < x.rep->d[i-1]) return -1;
			if(rep->d[i-1] > x.rep->d[i-1]) return 1;
		}
		return 0;
	}

	void BIGINT::BasicAdd(const BIGINT& x)
	{
		int r = 0;
		while(rep->d.size() < x.rep->d.size()+1) rep->d.push_back(0);
		for(unsigned int i = 0; i < rep->d.size(); i++)
		{
			r = rep->d[i] + x.SafeGetDigit(i) + r;
			rep->d[i] = r % 0x10000;
			r = r / 0x10000;
		}
		if(r > 0)
			throw OverflowError();
		rep->Normalize();
	}

	void BIGINT::BasicSubtract(const BIGINT& x)
	{
		int r = 0;
		while(rep->d.size() < x.rep->d.size()) rep->d.push_back(0);
		for(unsigned int i = 0; i < rep->d.size(); i++)
		{
			r = rep->d[i] - x.SafeGetDigit(i) - r;
			if (r < 0)
			{
				rep->d[i] = static_cast<unsigned short>(0x10000 + r);
				r = 1;
			}
			else
			{
				rep->d[i] = static_cast<unsigned short>(r);
				r = 0;
			}
		}
		if(r > 0)
			throw OverflowError();
		rep->Normalize();
	}

	void BIGINT::Add(const BIGINT& x)
	{
		if(rep->neg == x.rep->neg)
		{
			BasicAdd(x);
		}
		else if (BasicCompare(x) < 0)
		{
			BIGINT li = *this;
			*this = x;
			rep = rep->Copy();
			BasicSubtract(li);
			rep->neg = !li.rep->neg;
		}
		else if (BasicCompare(x) > 0)
		{
			BasicSubtract(x);
		}
		else
		{
			*this = 0; 
		}
		if(BasicCompare(0)==0) rep->neg = false;
	}

	void BIGINT::Subtract(const BIGINT& x)
	{
		if (rep->neg == x.rep->neg)
		{
			if(BasicCompare(x) < 0)
			{
				BIGINT li = *this;
				*this = x;
				rep = rep->Copy();
				BasicSubtract(li);
				rep->neg = !li.rep->neg;
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
		if(BasicCompare(0)==0) rep->neg = false;
	}

	void BIGINT::Multiply(const BIGINT& m)
	{
		unsigned int r = 0;
		BIGINT y(0), x(0);

		// Make sure the operand with the most digits is
		// processed in the outer loop.
		if(rep->d.size() > m.rep->d.size())
		{
			y = m;
			x = *this;
		}
		else
		{
			x = m;
			y = *this;
		}
		rep = rep->Copy();
		Clear();

		BIGINT li(0);
		for(unsigned int i = 0; i < x.rep->d.size(); i++)
		{
			r = 0;
			li.Clear();
			for(int k = 0; k < i; k++) li.rep->d.push_back(0);
			for(unsigned int j = 0; j < y.rep->d.size(); j++)
			{
				r = x.rep->d[i] * y.rep->d[j] + r;

				li.rep->d.push_back(r % 0x10000);
				r = r / 0x10000;
			}
			if(r > 0) li.rep->d.push_back(r);
			BasicAdd(li);
		}
		rep->neg = x.rep->neg != y.rep->neg;
		if(BasicCompare(0)==0) rep->neg = false;
		rep->Normalize();
	}

	void BIGINT::Divide(const BIGINT& x, BIGINT& quotient, BIGINT& remainder)
	{
		if(BasicCompare(x) < 0)
		{
			remainder = x;
			return;
		}
		else if(BasicCompare(x) == 0)
		{
			quotient = 1;
			quotient.rep->neg = rep->neg != x.rep->neg;
			return;
		}
		else if (x == 0)
		{
			throw DivideByZeroError();
		}

		BIGINT dividend(*this);
		BIGINT divisor(x);
		dividend.rep = dividend.rep->Copy();
		divisor.rep = divisor.rep->Copy();

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

		quotient.rep->neg = rep->neg != x.rep->neg;
		if(quotient.BasicCompare(0)==0) quotient.rep->neg = false;
		if(remainder.BasicCompare(0)==0) remainder.rep->neg = false;
	}

	void BIGINT::Divide(const BIGINT& x)
	{
		BIGINT quotient(0), remainder(0);
		Divide(x, quotient, remainder);
		*this = quotient;
	}

	void BIGINT::Modulus(const BIGINT& x)
	{
		BIGINT quotient(0), remainder(0);
		Divide(x, quotient, remainder);
		*this = remainder;
	}

	BIGINT::BIGINT(int ii) : rep(new IntRep) {
		rep->d.push_back(abs(ii) & 0xffff);
		rep->d.push_back(abs(ii) >> 16);
		rep->neg = ii < 0;
		rep->Normalize();
	}

	BIGINT::BIGINT(const BIGINT& li)
	{
		rep = li.rep;
		rep->n++;
	}

	BIGINT::~BIGINT()
	{
		if(rep->n == 1)
			delete rep;
	}

	BIGINT BIGINT::operator+() { return *this; }

	BIGINT BIGINT::operator-()
	{
		BIGINT li = *this;
		li.rep = rep->Copy();
		li.rep->neg = !li.rep->neg;
		return li;
	}

	BIGINT BIGINT::operator-(const BIGINT& x)
	{
		BIGINT li = *this;
		li.rep = li.rep->Copy();
		li.Subtract(x);
		return li;
	}

	BIGINT BIGINT::operator+(const BIGINT& x) 
	{
		BIGINT li = *this;
		li.rep = li.rep->Copy();
		li.Add(x);
		return li;
	}

	BIGINT BIGINT::operator*(const BIGINT& x)
	{
		BIGINT li = *this;
		li.rep = li.rep->Copy();
		li.Multiply(x);
		return li;
	}

	bool BIGINT::operator<(const BIGINT& x)
	{
		if(rep->neg==x.rep->neg) return BasicCompare(x) < 0;
		else if (rep->neg && !x.rep->neg) return true;
		else return false;
	}

	bool BIGINT::operator>(const BIGINT& x)
	{
		if(rep->neg==x.rep->neg) return BasicCompare(x) > 0;
		else if (!rep->neg && x.rep->neg) return true;
		else return false;
	}

	bool BIGINT::operator==(const BIGINT& x) const
	{
		if(rep->neg != x.rep->neg) return false;
		return BasicCompare(x) == 0;
	}

	bool BIGINT::operator==(int x) const
	{
		return *this == BIGINT(x);
	}

	BIGINT& BIGINT::operator=(const BIGINT& x)
	{
		if(rep->n == 1) delete rep;
		rep = x.rep;
		rep->n++;
		return *this;
	}

	BIGINT& BIGINT::operator=(const int i)
	{
		*this = BIGINT(i);
		return *this;
	}

	BIGINT BIGINT::operator/(const BIGINT& x) {
		BIGINT li = *this;
		li.rep = li.rep->Copy();
		li.Divide(x);
		return li;
	}

	BIGINT BIGINT::operator%(const BIGINT& x) {
		BIGINT li = *this;
		li.rep = li.rep->Copy();
		li.Modulus(x);
		return li;
	}

	ostream& operator<<(ostream& out, const BIGINT& x)
	{
		if(x.rep->neg) out << '-';
		out.fill('0');
		out << std::hex;
		if(x.rep->d.size() < 1) out << 0;
		for(int i = x.rep->d.size(); i > 0; i--)
		{
			out.width(4);
			out << x.rep->d[i-1];
		}
		return out;
	}
}

void main()
{
	using namespace Exercises;
	using namespace std;

	cout << "All positives:" << endl;
	cout << (BIGINT(0x100) + BIGINT(0xffff)) << ' ' << (0x100 + 0xffff) << endl;
	cout << (BIGINT(0xffff) + BIGINT(0xffff)) << ' ' << (0xffff + 0xffff) << endl;
	cout << (BIGINT(0x1ffff) + BIGINT(0xfffe)) << ' ' << (0x1ffff + 0xfffe) << endl;
	cout << (BIGINT(0xfffff) + BIGINT(0xfffff)) << ' ' << (0xfffff + 0xfffff) << endl;
	cout << (BIGINT(0x7ffff) + BIGINT(0x8ffff)) << ' ' << (0x7ffff + 0x8ffff) << endl;

	cout << (BIGINT(0xffff) - BIGINT(0x100)) << ' ' << (0xffff - 0x100) << endl;
	cout << (BIGINT(0xffff) - BIGINT(0xffff)) << ' ' << (0xffff - 0xffff) << endl;
	cout << (BIGINT(0x1ffff) - BIGINT(0xfffe)) << ' ' << (0x1ffff - 0xfffe) << endl;
	cout << (BIGINT(0xfffff) - BIGINT(0xfffff)) << ' ' << (0xfffff - 0xfffff) << endl;
	cout << (BIGINT(0x8ffff) - BIGINT(0x7ffff)) << ' ' << (0x8ffff - 0x7ffff) << endl;

	cout << (BIGINT(0xff) * BIGINT(0x100)) << ' ' << (0xff * 0x100) << endl;
	cout << (BIGINT(0xff) * BIGINT(0xffff)) << ' ' << (0xff * 0xffff) << endl;
	cout << (BIGINT(0x1ff) * BIGINT(0xfffe)) << ' ' << (0x1ff * 0xfffe) << endl;
	cout << (BIGINT(0xffe) * BIGINT(0xfffff)) << ' ' << (0xffe * 0xfffff) << endl;
	cout << (BIGINT(0x8fff) * BIGINT(0x7ffff)) << ' ' << (0x8fff * 0x7ffff) << endl;
	cout << (BIGINT(0x10000) * BIGINT(0xffff)) << ' ' << (0x10000 * 0xffff) << endl;

	cout << (BIGINT(2) / BIGINT(1)) << ' ' << (0x2 / 0x1) << endl;
	cout << (BIGINT(0x20) / BIGINT(0x5)) << ' ' << (0x20 / 0x5) << endl;
	cout << (BIGINT(0x10000) / BIGINT(0x2)) << ' ' << (0x10000 / 0x2) << endl;
	cout << (BIGINT(0x20000) / BIGINT(0x3)) << ' ' << (0x20000 / 0x3) << endl;
	cout << (BIGINT(0x20000) / BIGINT(0xffff)) << ' ' << (0x20000 / 0xffff) << endl;
	cout << (BIGINT(0x20000) / BIGINT(0x10000)) << ' ' << (0x20000 / 0x10000) << endl;
	cout << (BIGINT(16) / BIGINT(5)) << ' ' << (16 / 5) << endl;

	cout << (BIGINT(2) % BIGINT(1)) << ' ' << (0x2 % 0x1) << endl;
	cout << (BIGINT(0x20) % BIGINT(0x5)) << ' ' << (0x20 % 0x5) << endl;
	cout << (BIGINT(0x10000) % BIGINT(0x2)) << ' ' << (0x10000 % 0x2) << endl;
	cout << (BIGINT(0x20000) % BIGINT(0x3)) << ' ' << (0x20000 % 0x3) << endl;
	cout << (BIGINT(0x20000) % BIGINT(0xffff)) << ' ' << (0x20000 % 0xffff) << endl;
	cout << (BIGINT(0x20000) % BIGINT(0x10000)) << ' ' << (0x20000 % 0x10000) << endl;
	cout << (BIGINT(16) % BIGINT(5)) << ' ' << (16 % 5) << endl;

	cout << "All negatives:" << endl;
	cout << (BIGINT(-0x100) + BIGINT(-0xffff)) << ' ' << (-0x100 + -0xffff) << endl;
	cout << (BIGINT(-0xffff) + BIGINT(-0xffff)) << ' ' << (-0xffff + -0xffff) << endl;
	cout << (BIGINT(-0x1ffff) + BIGINT(-0xfffe)) << ' ' << (-0x1ffff + -0xfffe) << endl;
	cout << (BIGINT(-0xfffff) + BIGINT(-0xfffff)) << ' ' << (-0xfffff + -0xfffff) << endl;
	cout << (BIGINT(-0x7ffff) + BIGINT(-0x8ffff)) << ' ' << (-0x7ffff + -0x8ffff) << endl;

	cout << (BIGINT(-0xffff) - BIGINT(-0x100)) << ' ' << (-0xffff - -0x100) << endl;
	cout << (BIGINT(-0xffff) - BIGINT(-0xffff)) << ' ' << (-0xffff - -0xffff) << endl;
	cout << (BIGINT(-0x1ffff) - BIGINT(-0xfffe)) << ' ' << (-0x1ffff - -0xfffe) << endl;
	cout << (BIGINT(-0xfffff) - BIGINT(-0xfffff)) << ' ' << (-0xfffff - -0xfffff) << endl;
	cout << (BIGINT(-0x8ffff) - BIGINT(-0x7ffff)) << ' ' << (-0x8ffff - -0x7ffff) << endl;

	cout << (BIGINT(-0xff) * BIGINT(-0x100)) << ' ' << (-0xff * -0x100) << endl;
	cout << (BIGINT(-0xff) * BIGINT(-0xffff)) << ' ' << (-0xff * -0xffff) << endl;
	cout << (BIGINT(-0x1ff) * BIGINT(-0xfffe)) << ' ' << (-0x1ff * -0xfffe) << endl;
	cout << (BIGINT(-0xffe) * BIGINT(-0xfffff)) << ' ' << (-0xffe * -0xfffff) << endl;
	cout << (BIGINT(-0x8fff) * BIGINT(-0x7ffff)) << ' ' << (-0x8fff * -0x7ffff) << endl;
	cout << (BIGINT(-0x10000) * BIGINT(-0xffff)) << ' ' << (-0x10000 * -0xffff) << endl;

	cout << (BIGINT(-2) / BIGINT(-1)) << ' ' << (-0x2 / -0x1) << endl;
	cout << (BIGINT(-0x20) / BIGINT(-0x5)) << ' ' << (-0x20 / -0x5) << endl;
	cout << (BIGINT(-0x10000) / BIGINT(-0x2)) << ' ' << (-0x10000 / -0x2) << endl;
	cout << (BIGINT(-0x20000) / BIGINT(-0x3)) << ' ' << (-0x20000 / -0x3) << endl;
	cout << (BIGINT(-0x20000) / BIGINT(-0xffff)) << ' ' << (-0x20000 / -0xffff) << endl;
	cout << (BIGINT(-0x20000) / BIGINT(-0x10000)) << ' ' << (-0x20000 / -0x10000) << endl;
	cout << (BIGINT(-16) / BIGINT(-5)) << ' ' << (-16 / -5) << endl;

	cout << (BIGINT(-2) % BIGINT(-1)) << ' ' << (-0x2 % -0x1) << endl;
	cout << (BIGINT(-0x20) % BIGINT(-0x5)) << ' ' << (-0x20 % -0x5) << endl;
	cout << (BIGINT(-0x10000) % BIGINT(-0x2)) << ' ' << (-0x10000 % -0x2) << endl;
	cout << (BIGINT(-0x20000) % BIGINT(-0x3)) << ' ' << (-0x20000 % -0x3) << endl;
	cout << (BIGINT(-0x20000) % BIGINT(-0xffff)) << ' ' << (-0x20000 % -0xffff) << endl;
	cout << (BIGINT(-0x20000) % BIGINT(-0x10000)) << ' ' << (-0x20000 % -0x10000) << endl;
	cout << (BIGINT(-16) % BIGINT(-5)) << ' ' << (-16 % -5) << endl;

	cout << "Left negative:" << endl;
	cout << (BIGINT(-0x100) + BIGINT(0xffff)) << ' ' << (-0x100 + 0xffff) << endl;
	cout << (BIGINT(-0xffff) + BIGINT(0xffff)) << ' ' << (-0xffff + 0xffff) << endl;
	cout << (BIGINT(-0x1ffff) + BIGINT(0xfffe)) << ' ' << (-0x1ffff + 0xfffe) << endl;
	cout << (BIGINT(-0xfffff) + BIGINT(0xfffff)) << ' ' << (-0xfffff + 0xfffff) << endl;
	cout << (BIGINT(-0x7ffff) + BIGINT(0x8ffff)) << ' ' << (-0x7ffff + 0x8ffff) << endl;

	cout << (BIGINT(-0xffff) - BIGINT(0x100)) << ' ' << (-0xffff - 0x100) << endl;
	cout << (BIGINT(-0xffff) - BIGINT(0xffff)) << ' ' << (-0xffff - 0xffff) << endl;
	cout << (BIGINT(-0x1ffff) - BIGINT(0xfffe)) << ' ' << (-0x1ffff - 0xfffe) << endl;
	cout << (BIGINT(-0xfffff) - BIGINT(0xfffff)) << ' ' << (-0xfffff - 0xfffff) << endl;
	cout << (BIGINT(-0x8ffff) - BIGINT(0x7ffff)) << ' ' << (-0x8ffff - 0x7ffff) << endl;

	cout << (BIGINT(-0xff) * BIGINT(0x100)) << ' ' << (-0xff * 0x100) << endl;
	cout << (BIGINT(-0xff) * BIGINT(0xffff)) << ' ' << (-0xff * 0xffff) << endl;
	cout << (BIGINT(-0x1ff) * BIGINT(0xfffe)) << ' ' << (-0x1ff * 0xfffe) << endl;
	cout << (BIGINT(-0xffe) * BIGINT(0xfffff)) << ' ' << (-0xffe * 0xfffff) << endl;
	cout << (BIGINT(-0x8fff) * BIGINT(0x7ffff)) << ' ' << (-0x8fff * 0x7ffff) << endl;
	cout << (BIGINT(-0x10000) * BIGINT(0xffff)) << ' ' << (-0x10000 * 0xffff) << endl;

	cout << (BIGINT(-2) / BIGINT(1)) << ' ' << (-0x2 / 0x1) << endl;
	cout << (BIGINT(-0x20) / BIGINT(0x5)) << ' ' << (-0x20 / 0x5) << endl;
	cout << (BIGINT(-0x10000) / BIGINT(0x2)) << ' ' << (-0x10000 / 0x2) << endl;
	cout << (BIGINT(-0x20000) / BIGINT(0x3)) << ' ' << (-0x20000 / 0x3) << endl;
	cout << (BIGINT(-0x20000) / BIGINT(0xffff)) << ' ' << (-0x20000 / 0xffff) << endl;
	cout << (BIGINT(-0x20000) / BIGINT(0x10000)) << ' ' << (-0x20000 / 0x10000) << endl;
	cout << (BIGINT(-16) / BIGINT(5)) << ' ' << (-16 / 5) << endl;

	cout << (BIGINT(-2) % BIGINT(1)) << ' ' << (-0x2 % 0x1) << endl;
	cout << (BIGINT(-0x20) % BIGINT(0x5)) << ' ' << (-0x20 % 0x5) << endl;
	cout << (BIGINT(-0x10000) % BIGINT(0x2)) << ' ' << (-0x10000 % 0x2) << endl;
	cout << (BIGINT(-0x20000) % BIGINT(0x3)) << ' ' << (-0x20000 % 0x3) << endl;
	cout << (BIGINT(-0x20000) % BIGINT(0xffff)) << ' ' << (-0x20000 % 0xffff) << endl;
	cout << (BIGINT(-0x20000) % BIGINT(0x10000)) << ' ' << (-0x20000 % 0x10000) << endl;
	cout << (BIGINT(-16) % BIGINT(5)) << ' ' << (-16 % 5) << endl;

	cout << "Right negative:" << endl;
	cout << (BIGINT(0x100) + BIGINT(-0xffff)) << ' ' << (0x100 + -0xffff) << endl;
	cout << (BIGINT(0xffff) + BIGINT(-0xffff)) << ' ' << (0xffff + -0xffff) << endl;
	cout << (BIGINT(0x1ffff) + BIGINT(-0xfffe)) << ' ' << (0x1ffff + -0xfffe) << endl;
	cout << (BIGINT(0xfffff) + BIGINT(-0xfffff)) << ' ' << (0xfffff + -0xfffff) << endl;
	cout << (BIGINT(0x7ffff) + BIGINT(-0x8ffff)) << ' ' << (0x7ffff + -0x8ffff) << endl;

	cout << (BIGINT(0xffff) - BIGINT(-0x100)) << ' ' << (0xffff - -0x100) << endl;
	cout << (BIGINT(0xffff) - BIGINT(-0xffff)) << ' ' << (0xffff - -0xffff) << endl;
	cout << (BIGINT(0x1ffff) - BIGINT(-0xfffe)) << ' ' << (0x1ffff - -0xfffe) << endl;
	cout << (BIGINT(0xfffff) - BIGINT(-0xfffff)) << ' ' << (0xfffff - -0xfffff) << endl;
	cout << (BIGINT(0x8ffff) - BIGINT(-0x7ffff)) << ' ' << (0x8ffff - -0x7ffff) << endl;

	cout << (BIGINT(0xff) * BIGINT(-0x100)) << ' ' << (0xff * -0x100) << endl;
	cout << (BIGINT(0xff) * BIGINT(-0xffff)) << ' ' << (0xff * -0xffff) << endl;
	cout << (BIGINT(0x1ff) * BIGINT(-0xfffe)) << ' ' << (0x1ff * -0xfffe) << endl;
	cout << (BIGINT(0xffe) * BIGINT(-0xfffff)) << ' ' << (0xffe * -0xfffff) << endl;
	cout << (BIGINT(0x8fff) * BIGINT(-0x7ffff)) << ' ' << (0x8fff * -0x7ffff) << endl;
	cout << (BIGINT(0x10000) * BIGINT(-0xffff)) << ' ' << (0x10000 * -0xffff) << endl;

	cout << (BIGINT(2) / BIGINT(-1)) << ' ' << (0x2 / -0x1) << endl;
	cout << (BIGINT(0x20) / BIGINT(-0x5)) << ' ' << (0x20 / -0x5) << endl;
	cout << (BIGINT(0x10000) / BIGINT(-0x2)) << ' ' << (0x10000 / -0x2) << endl;
	cout << (BIGINT(0x20000) / BIGINT(-0x3)) << ' ' << (0x20000 / -0x3) << endl;
	cout << (BIGINT(0x20000) / BIGINT(-0xffff)) << ' ' << (0x20000 / -0xffff) << endl;
	cout << (BIGINT(0x20000) / BIGINT(-0x10000)) << ' ' << (0x20000 / -0x10000) << endl;
	cout << (BIGINT(16) / BIGINT(-5)) << ' ' << (16 / -5) << endl;

	cout << (BIGINT(2) % BIGINT(-1)) << ' ' << (0x2 % -0x1) << endl;
	cout << (BIGINT(0x20) % BIGINT(-0x5)) << ' ' << (0x20 % -0x5) << endl;
	cout << (BIGINT(0x10000) % BIGINT(-0x2)) << ' ' << (0x10000 % -0x2) << endl;
	cout << (BIGINT(0x20000) % BIGINT(-0x3)) << ' ' << (0x20000 % -0x3) << endl;
	cout << (BIGINT(0x20000) % BIGINT(-0xffff)) << ' ' << (0x20000 % -0xffff) << endl;
	cout << (BIGINT(0x20000) % BIGINT(-0x10000)) << ' ' << (0x20000 % -0x10000) << endl;
	cout << (BIGINT(16) % BIGINT(-5)) << ' ' << (16 % -5) << endl;

	cout << "Factorial of 1000:" << endl;
	BIGINT bi(1000);
	for(int i = 999; i > 1; i--)
	{
		bi = bi * i;
	}
	cout << bi << endl;
}
