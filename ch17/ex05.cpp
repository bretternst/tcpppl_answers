#include <iostream>
#include <vector>
#include <functional>
#include <cmath>

namespace Exercises
{
    // supports any bi-directional iterator
    template<class Iter, class Tequals>
    bool isPalindrome(Iter start, Iter end, Tequals eq)
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

    bool isPalindrome(const std::string& str, bool alphaNumericOnly = false, bool caseSensitive = true)
    {
        if(alphaNumericOnly)
        {
            std::string letters;
            letters.reserve(str.size());
            for(std::string::const_iterator i = str.begin(); i != str.end(); i++)
            {
                if(isalnum(*i)) letters.push_back(*i);
            }
            return isPalindrome(letters.begin(), letters.end(), CharComparer(caseSensitive));
        }
        else
        {
            return isPalindrome(str.begin(), str.end(), CharComparer(caseSensitive));
        }
    }

    bool isPalindrome(int n)
    {
        std::vector<char> digits;
        digits.reserve(static_cast<int>(log(static_cast<double>(n)) / log(10.0)) + 1);
        while(n > 0)
        {
            digits.push_back(n % 10);
            n /= 10;
        }
        return isPalindrome(digits.begin(),digits.end(),std::equal_to<char>());
    }
}

int main()
{
    using namespace std;
    using namespace Exercises;

    cout << isPalindrome("abba") << endl; // 1
    cout << isPalindrome("abbc") << endl; // 0
    cout << isPalindrome("abcba") << endl; // 1
    cout << isPalindrome("azcba") << endl; // 0

    cout << isPalindrome(1221) << endl; // 1
    cout << isPalindrome(1231) << endl; // 0
    cout << isPalindrome(12321) << endl; // 1
    cout << isPalindrome(13321) << endl; // 0

    cout << isPalindrome("A man, a plan, a canal -- Panama.", true, false) << endl; // 1
    cout << isPalindrome("A plan, a man, a canal -- Panama.", true, false) << endl; // 0

    return 0;
}
