#include <iostream>
#include <limits>
#include <cstdlib>

using std::ostream;

// On some platforms, one could use "long long" but that would
// probably not be in the spirit of the exercise.
// In the real world, one would want to consider using bitset<T>
// or something like that.
// I certainly don't make any claims about the efficiency of this
// implementation...
class LINT
{
    bool neg;
    unsigned short d[4];

    class OverflowError {};
    class divideByZeroError {};

    void clear()
    {
        for(int i = 0; i < 4; i++) d[i] = 0;
        neg = 0;
    }

    void bit_shift_left()
    {
        for(int i = 3; i >= 0; i--)
        {
            d[i] <<= 1;
            if(i > 0 && (d[i-1] & 0x8000))
                d[i] |= 1;
        }
    }

    void bit_shift_right()
    {
        for(int i = 0; i <= 3; i++)
        {
            d[i] >>= 1;
            if(i < 3 && (d[i+1] & 1))
                d[i] |= 0x8000;
        }
    }

    void invert()
    {
        for(int i = 0; i <= 3; i++)
            d[i] = ~d[i];
    }

    // we'll consider this sort of a 65k-based number system, so a "digit"
    // is a short.
    int digit_count() const
    {
        for(int i = 3; i >= 0; i--)
            if(d[i]!=0) return i+1;
        return 0;
    }

    unsigned short safe_git_digit(int idx) const
    {
        if(idx < 0 || idx > 3) return 0;
        return d[idx];
    }

    int basic_compare(const LINT& x) const
    {
        for(int i = 3; i >= 0; i--)
        {
            if(d[i] < x.d[i]) return -1;
            if(d[i] > x.d[i]) return 1;
        }
        return 0;
    }

    void basic_add(const LINT& x)
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

    void basic_subtract(const LINT& x)
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

    void add(const LINT& x)
    {
        if(neg == x.neg)
        {
            basic_add(x);
        }
        else if (basic_compare(x) < 0)
        {
            LINT li = *this;
            *this = x;
            basic_subtract(li);
            neg = !li.neg;
        }
        else if (basic_compare(x) > 0)
        {
            basic_subtract(x);
        }
        else
        {
            *this = 0; 
        }
        if(basic_compare(0)==0) neg = false;
    }

    void subtract(const LINT& x)
    {
        if (neg == x.neg)
        {
            if(basic_compare(x) < 0)
            {
                LINT li = *this;
                *this = x;
                basic_subtract(li);
                neg = !li.neg;
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
        if(basic_compare(0)==0) neg = false;
    }

    void multiply(const LINT& m)
    {
        unsigned int r = 0;
        LINT y(0), x(0);

        // Make sure the operand with the most digits is
        // processed in the outer loop.
        if(digit_count() > x.digit_count())
        {
            y = m;
            x = *this;
        }
        else
        {
            x = m;
            y = *this;
        }
        clear();
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
            basic_add(li);
            li = LINT(0);
        }
        neg = x.neg != y.neg;
        if(basic_compare(0)==0) neg = false;
    }

    // added note:
    // this algorithm came from google somewhere; I would credit
    // the site if I could remember...
    void divide(const LINT& x, LINT& quotient, LINT& remainder)
    {
        if(basic_compare(x) < 0)
        {
            remainder = x;
            return;
        }
        else if(basic_compare(x) == 0)
        {
            quotient = 1;
            quotient.neg = neg != x.neg;
            return;
        }
        else if (x == 0)
        {
            throw divideByZeroError();
        }

        LINT dividend(*this);
        LINT divisor(x);
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

        quotient.neg = neg != x.neg;
        if(quotient.basic_compare(0)==0) quotient.neg = false;
        if(remainder.basic_compare(0)==0) remainder.neg = false;
    }

    void divide(const LINT& x)
    {
        LINT quotient(0), remainder(0);
        divide(x, quotient, remainder);
        *this = quotient;
    }

    void modulus(const LINT& x)
    {
        LINT quotient(0), remainder(0);
        divide(x, quotient, remainder);
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
        li.subtract(x);
        return li;
    }

    LINT operator+(const LINT& x) 
    {
        LINT li = *this;
        li.add(x);
        return li;
    }

    LINT operator*(const LINT& x)
    {
        LINT li = *this;
        li.multiply(x);
        return li;
    }

    bool operator<(const LINT& x)
    {
        if(neg==x.neg) return basic_compare(x) < 0;
        else if (neg && !x.neg) return true;
        else return false;
    }

    bool operator>(const LINT& x)
    {
        if(neg==x.neg) return basic_compare(x) > 0;
        else if (!neg && x.neg) return true;
        else return false;
    }

    bool operator==(const LINT& x) const
    {
        if(neg != x.neg) return false;
        return basic_compare(x) == 0;
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
        li.divide(x);
        return li;
    }

    LINT operator%(const LINT& x) {
        LINT li = *this;
        li.modulus(x);
        return li;
    }

    friend ostream& operator<<(ostream& out, const LINT& x);
};

ostream& operator<<(ostream& out, const LINT& x)
{
    LINT li = x;
    if (li.neg)
    {
        li.invert();
        li.basic_add(1);
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

int main()
{
    using namespace std;

    // some of these operations can't be compared directly on a 32-bit
    // system without using something like "long long" (nonstandard)

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
    cout << (LINT(0xffe) * LINT(0xfffff)) << endl; 
    cout << (LINT(0x8fff) * LINT(0x7ffff)) << endl;
    cout << (LINT(0x10000) * LINT(0xffff)) << endl;

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
    cout << (LINT(-0xffe) * LINT(-0xfffff)) << endl;
    cout << (LINT(-0x8fff) * LINT(-0x7ffff)) << endl;
    cout << (LINT(-0x10000) * LINT(-0xffff)) << endl;

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
    cout << (LINT(-0xffe) * LINT(0xfffff)) << endl;
    cout << (LINT(-0x8fff) * LINT(0x7ffff)) << endl;
    cout << (LINT(-0x10000) * LINT(0xffff)) << endl;

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
    cout << (LINT(0xffe) * LINT(-0xfffff)) << endl;
    cout << (LINT(0x8fff) * LINT(-0x7ffff)) << endl;
    cout << (LINT(0x10000) * LINT(-0xffff)) << endl;

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
