#include <iostream>
#include <vector>
#include <functional>
#include <cmath>

namespace ch17
{
    // supports any bi-directional iterator
    template<class Iter, class Tequals>
    bool is_palindrome(Iter start, Iter end, Tequals eq)
    {
        while(start != end)
        {
            if(start==--end) break;
            if(!eq(*(start++),*end)) return false;
        }
        return true;
    }

    class CharComparer
    {
        bool cs;
    public:
        CharComparer(bool caseSensitive) : cs(caseSensitive) {}
        bool operator ()(char c1, char c2) { return cs ? c1 == c2 : toupper(c1) == toupper(c2); }
    };

    bool is_palindrome(const std::string& str, bool alpha_num_only = false, bool caseSensitive = true)
    {
        if(alpha_num_only)
        {
            std::string letters;
            letters.reserve(str.size());
            for(std::string::const_iterator i = str.begin(); i != str.end(); i++)
            {
                if(isalnum(*i)) letters.push_back(*i);
            }
            return is_palindrome(letters.begin(), letters.end(), CharComparer(caseSensitive));
        }
        else
        {
            return is_palindrome(str.begin(), str.end(), CharComparer(caseSensitive));
        }
    }

    bool is_palindrome(int n)
    {
        std::vector<char> digits;
        digits.reserve(static_cast<int>(log(static_cast<double>(n)) / log(10.0)) + 1);
        while(n > 0)
        {
            digits.push_back(n % 10);
            n /= 10;
        }
        return is_palindrome(digits.begin(),digits.end(),std::equal_to<char>());
    }
}

int main()
{
    using namespace std;
    using namespace ch17;

    cout << is_palindrome("abba") << endl; // 1
    cout << is_palindrome("abbc") << endl; // 0
    cout << is_palindrome("abcba") << endl; // 1
    cout << is_palindrome("azcba") << endl; // 0

    cout << is_palindrome(1221) << endl; // 1
    cout << is_palindrome(1231) << endl; // 0
    cout << is_palindrome(12321) << endl; // 1
    cout << is_palindrome(13321) << endl; // 0

    cout << is_palindrome("A man, a plan, a canal -- Panama.", true, false) << endl; // 1
    cout << is_palindrome("A plan, a man, a canal -- Panama.", true, false) << endl; // 0

    return 0;
}
