#include <iostream>
#include <cctype>

bool is_palindrome(const char* start, const char* end) {
    while(end > start) {
        while(isspace(*end)) end--;
        while(isspace(*start)) start++;
        if(end <= start)
            return true;
        if (*(end--) != *(start++))
            return false;
    }

    return true;
}

bool is_palindrome(const char* s) {
    const char* end = s;
    while(*end)
        end++;
    end--;
    return is_palindrome(s, end);
}

bool is_palindrome_at_least_three_words(const char* s) {
    const char* end = s;
    while(*end)
        end++;
    end--;

    int word = 0;
    const char* i = s;
    while(*i && isspace(*(i++)));
    if(!*i++)
        return false;
    for(;;) {
        if(!*i || isspace(*i)) {
            if(++word >= 3 && is_palindrome(s, i-1))
                return true;
        }
        if(!*i++)
            break;
    }
    return false;
}

int main() {
    using namespace std;

    cout << is_palindrome("a bcdefedcba") << endl;
    cout << is_palindrome("abcdee dcba") << endl;
    cout << is_palindrome("abc") << endl;

    cout << is_palindrome_at_least_three_words("abc def fed cba") << endl;
    cout << is_palindrome_at_least_three_words("abcba") << endl;
    cout << is_palindrome_at_least_three_words("abc def frd cba") << endl;
    cout << is_palindrome_at_least_three_words("abc cba") << endl;
}
