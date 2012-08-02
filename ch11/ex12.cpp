#include <iostream>
#include <string>

using std::string;

bool is_match(const string& str, const string& pattern, string::const_iterator pPos, string::const_iterator sPos)
{
    if(pPos == pattern.end() ||
        (pPos + 1 == pattern.end() && *pPos == '*')) return true;
    if(*pPos == '*')
    {
        for(int idx = 0; idx < str.end() - sPos; idx++)
        {
            string::const_iterator p = pPos+1;
            string::const_iterator i = sPos+idx;
            bool match = true;
            while(i != str.end() && p != pattern.end())
            {
                if(*p == '*') return is_match(str, pattern, p, i);
                if(*i != *p) {match = false; break;}
                i++; p++;
            }
            if(match)
                return i == str.end() && (p == pattern.end() || (p + 1 == pattern.end() && *p=='*'));
        }
        return false;
    }
    else
    {
        string::const_iterator p = pPos;
        string::const_iterator i = sPos;
        while(i != str.end() && p != pattern.end())
        {
            if(*p == '*') return is_match(str, pattern, p, i);
            if(*i != *p) return false;
            i++; p++;
        }
        return i == str.end() && (p == pattern.end() || (p+1 == pattern.end() && *p=='*'));
    }
}

bool is_match(const string& str, const string& pattern)
{
    return is_match(str, pattern, pattern.begin(), str.begin());
}

int main()
{
    using namespace std;

    string ex11("Hello world!");
    cout << "Should be true: " << is_match(ex11,"*") << endl;
    cout << "Should be true: " << is_match(ex11,"Hello*") << endl;
    cout << "Should be true: " << is_match(ex11,"Hello world!") << endl;
    cout << "Should be true: " << is_match(ex11,"*lo world!") << endl;
    cout << "Should be true: " << is_match(ex11,"Hel*orld!") << endl;
    cout << "Should be true: " << is_match(ex11,"Hel*o*d!") << endl;
    cout << "Should be true: " << is_match(ex11,"*ello worl*") << endl;
    cout << "Should be true: " << is_match(ex11,"*Hello world!") << endl;
    cout << "Should be true: " << is_match(ex11,"Hello world!*") << endl;
    cout << "Should be true: " << is_match(ex11,"*") << endl;

    cout << "Should be false: " << is_match(ex11,"Hello world") << endl;
    cout << "Should be false: " << is_match(ex11,"*Hello world") << endl;
    cout << "Should be false: " << is_match(ex11,"Hello* world") << endl;
    cout << "Should be false: " << is_match(ex11,"x*") << endl;
    cout << "Should be false: " << is_match(ex11,"*x") << endl;
    cout << "Should be false: " << is_match(ex11,"Hel*x*orld!") << endl;
}
