#include <iostream>
#include <limits>
#include <vector>
#include <cstdlib>

using std::ostream;

namespace Exercises
{
    // Again, this is unlikely to be very efficient at all.
    class BIGINT
    {
        class IntRep;
        IntRep* rep;

        void clear();
        void bit_shift_left();
        void bit_shift_right();
        void invert();
        unsigned short safe_get_digit(unsigned int idx) const;
        int basic_compare(const BIGINT& x) const;
        void basic_add(const BIGINT& x);
        void basic_subtract(const BIGINT& x);
        void add(const BIGINT& x);
        void subtract(const BIGINT& x);
        void multiply(const BIGINT& m);
        void divide(const BIGINT& x, BIGINT& quotient, BIGINT& remainder);
        void divide(const BIGINT& x);
        void modulus(const BIGINT& x);

    public:
        class OverflowError {};
        class divideByZeroError {};

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

        IntRep* copy()
        {
            if(n==1) return this;
            n--;
            return new IntRep(neg, d);
        }

        void normalize()
        {
            while(d.size() > 0 && *(d.end()-1) == 0) d.pop_back();
        }

    private:
        IntRep(const IntRep&);
        IntRep& operator=(const IntRep&);
    };

    void BIGINT::clear()
    {
        rep->d.clear();
        rep->neg = 0;
    }

    void BIGINT::bit_shift_left()
    {
        rep->d.push_back(0);
        for(int i = rep->d.size(); i > 0; i--)
        {
            rep->d[i-1] <<= 1;
            if(i > 1 && (rep->d[i-2] & 0x8000))
                rep->d[i-1] |= 1;
        }
        rep->normalize();
    }

    void BIGINT::bit_shift_right()
    {
        int sz = rep->d.size() - 1;

        for(int i = 0; i <= sz; i++)
        {
            rep->d[i] >>= 1;
            if(i < sz && (rep->d[i+1] & 1))
                rep->d[i] |= 0x8000;
        }
        rep->normalize();
    }

    void BIGINT::invert()
    {
        for(unsigned int i = 0; i <= rep->d.size()-1; i++)
            rep->d[i] = ~rep->d[i];
        rep->normalize();
    }

    unsigned short BIGINT::safe_get_digit(unsigned int idx) const
    {
        if(idx < 0 || idx+1 > rep->d.size()) return 0;
        return rep->d[idx];
    }

    int BIGINT::basic_compare(const BIGINT& x) const
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

    void BIGINT::basic_add(const BIGINT& x)
    {
        int r = 0;
        while(rep->d.size() < x.rep->d.size()+1) rep->d.push_back(0);
        for(unsigned int i = 0; i < rep->d.size(); i++)
        {
            r = rep->d[i] + x.safe_get_digit(i) + r;
            rep->d[i] = r % 0x10000;
            r = r / 0x10000;
        }
        if(r > 0)
            throw OverflowError();
        rep->normalize();
    }

    void BIGINT::basic_subtract(const BIGINT& x)
    {
        int r = 0;
        while(rep->d.size() < x.rep->d.size()) rep->d.push_back(0);
        for(unsigned int i = 0; i < rep->d.size(); i++)
        {
            r = rep->d[i] - x.safe_get_digit(i) - r;
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
        rep->normalize();
    }

    void BIGINT::add(const BIGINT& x)
    {
        if(rep->neg == x.rep->neg)
        {
            basic_add(x);
        }
        else if (basic_compare(x) < 0)
        {
            BIGINT li = *this;
            *this = x;
            rep = rep->copy();
            basic_subtract(li);
            rep->neg = !li.rep->neg;
        }
        else if (basic_compare(x) > 0)
        {
            basic_subtract(x);
        }
        else
        {
            *this = 0; 
        }
        if(basic_compare(0)==0) rep->neg = false;
    }

    void BIGINT::subtract(const BIGINT& x)
    {
        if (rep->neg == x.rep->neg)
        {
            if(basic_compare(x) < 0)
            {
                BIGINT li = *this;
                *this = x;
                rep = rep->copy();
                basic_subtract(li);
                rep->neg = !li.rep->neg;
            }
            else if (basic_compare(x) > 0)
            {
                basic_subtract(x);
            }
            else
            {
                *this = 0;
            }
        }
        else
        {
            basic_add(x);
        }
        if(basic_compare(0)==0) rep->neg = false;
    }

    void BIGINT::multiply(const BIGINT& m)
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
        rep = rep->copy();
        clear();

        BIGINT li(0);
        for(unsigned int i = 0; i < x.rep->d.size(); i++)
        {
            r = 0;
            li.clear();
            for(int k = 0; k < i; k++) li.rep->d.push_back(0);
            for(unsigned int j = 0; j < y.rep->d.size(); j++)
            {
                r = x.rep->d[i] * y.rep->d[j] + r;

                li.rep->d.push_back(r % 0x10000);
                r = r / 0x10000;
            }
            if(r > 0) li.rep->d.push_back(r);
            basic_add(li);
        }
        rep->neg = x.rep->neg != y.rep->neg;
        if(basic_compare(0)==0) rep->neg = false;
        rep->normalize();
    }

    void BIGINT::divide(const BIGINT& x, BIGINT& quotient, BIGINT& remainder)
    {
        if(basic_compare(x) < 0)
        {
            remainder = x;
            return;
        }
        else if(basic_compare(x) == 0)
        {
            quotient = 1;
            quotient.rep->neg = rep->neg != x.rep->neg;
            return;
        }
        else if (x == 0)
        {
            throw divideByZeroError();
        }

        BIGINT dividend(*this);
        BIGINT divisor(x);
        dividend.rep = dividend.rep->copy();
        divisor.rep = divisor.rep->copy();

        quotient = remainder = 0;
        int zeroCount = 0;

        while(divisor.basic_compare(dividend) < 0)
        {
            divisor.bit_shift_left();
            zeroCount++;
        }
        if(divisor.basic_compare(dividend) > 0)
        {
            divisor.bit_shift_right();
            zeroCount--;
        }

        if(zeroCount >= 0)
        {
            for(int i = 0; i <= zeroCount; i++)
            {
                if(divisor.basic_compare(dividend) <= 0)
                {
                    dividend.basic_subtract(divisor);
                    divisor.bit_shift_right();
                    quotient.bit_shift_left();
                    quotient.basic_add(1);
                }
                else
                {
                    divisor.bit_shift_right();
                    quotient.bit_shift_left();
                }
            }
        }

        remainder = dividend;

        quotient.rep->neg = rep->neg != x.rep->neg;
        if(quotient.basic_compare(0)==0) quotient.rep->neg = false;
        if(remainder.basic_compare(0)==0) remainder.rep->neg = false;
    }

    void BIGINT::divide(const BIGINT& x)
    {
        BIGINT quotient(0), remainder(0);
        divide(x, quotient, remainder);
        *this = quotient;
    }

    void BIGINT::modulus(const BIGINT& x)
    {
        BIGINT quotient(0), remainder(0);
        divide(x, quotient, remainder);
        *this = remainder;
    }

    BIGINT::BIGINT(int ii) : rep(new IntRep) {
        rep->d.push_back(abs(ii) & 0xffff);
        rep->d.push_back(abs(ii) >> 16);
        rep->neg = ii < 0;
        rep->normalize();
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
        li.rep = rep->copy();
        li.rep->neg = !li.rep->neg;
        return li;
    }

    BIGINT BIGINT::operator-(const BIGINT& x)
    {
        BIGINT li = *this;
        li.rep = li.rep->copy();
        li.subtract(x);
        return li;
    }

    BIGINT BIGINT::operator+(const BIGINT& x) 
    {
        BIGINT li = *this;
        li.rep = li.rep->copy();
        li.add(x);
        return li;
    }

    BIGINT BIGINT::operator*(const BIGINT& x)
    {
        BIGINT li = *this;
        li.rep = li.rep->copy();
        li.multiply(x);
        return li;
    }

    bool BIGINT::operator<(const BIGINT& x)
    {
        if(rep->neg==x.rep->neg) return basic_compare(x) < 0;
        else if (rep->neg && !x.rep->neg) return true;
        else return false;
    }

    bool BIGINT::operator>(const BIGINT& x)
    {
        if(rep->neg==x.rep->neg) return basic_compare(x) > 0;
        else if (!rep->neg && x.rep->neg) return true;
        else return false;
    }

    bool BIGINT::operator==(const BIGINT& x) const
    {
        if(rep->neg != x.rep->neg) return false;
        return basic_compare(x) == 0;
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
        li.rep = li.rep->copy();
        li.divide(x);
        return li;
    }

    BIGINT BIGINT::operator%(const BIGINT& x) {
        BIGINT li = *this;
        li.rep = li.rep->copy();
        li.modulus(x);
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

int main()
{
    using namespace Exercises;
    using namespace std;

    BIGINT bi(1000);
    for(int i = 999; i > 1; i--)
    {
        bi = bi * i;
    }
    cout << bi << endl;
}
