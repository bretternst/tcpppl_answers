#include <iostream>
#include <sstream>
#include <cstdarg>

using namespace std;

void error(string s, ...)
{
    istringstream in(s);
    char ch;
    int i = 0;
    va_list args;
    va_start(args, s);

    char *s_arg;
    double d_arg;
    char c_arg;

    while (in.get(ch))
    {
        if(ch=='%')
        {
            switch(in.peek())
            {
            case '%':
                cout << in.get();
                break;
            case 's':
                s_arg = va_arg(args,char *);
                cout << s_arg;
                in.get();
                break;
            case 'd':
                d_arg = va_arg(args,int);
                cout << d_arg;
                in.get();
                break;
            case 'c':
                c_arg = static_cast<char>(va_arg(args, int));
                cout << c_arg;
                in.get();
                break;
            default:
                cout << ch;
                break;
            }
        }
        else
            cout << ch;
    }

    cout << endl;
    va_end(args);
}

int main()
{
    error("Hello %s", "world");
    error("Hello %d %s", 5, "world");
    error("Hello %c %d %s", 'a', 5, "world");
}
