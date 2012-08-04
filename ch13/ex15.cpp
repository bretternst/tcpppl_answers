#include <iostream>
#include <map>
#include <string>

namespace ch13
{
    using std::cout;
    using std::endl;

#define SUM(x,y) (x + y + z)
#define MAX(x,y) (x > y ? x : y)

    template<class T> T sum(T x, T y)
    {
        int z = 0;
        return x + y + z;
    }

    template<> char sum(char x, char y)
    {
        return x + y - 'a';
    }

    template<class T> T max(T x, T y)
    {
        return x > y ? x : y;
    }

    // Difference 1: Macros are subject to the scope in which they are
    // invoked, not the scope in which they are defined. Thus, the same
    // macro may return a different result depending on where it is called
    // whereas a template does not have this problem.
    void callsum1()
    {
        int z = 4;
        cout << SUM(2,2) << endl;
        cout << sum(2,2) << endl;
    }

    // Difference 2: Macros cannot be specialized. Templates can take on
    // a different behavior depending on the supplied type arguments.
    //
    // Difference 3: Macros have no notion of type. Instead of outputting
    // a char, the SUM macro returns an integer, even though the inputs
    // are chars. This is because 'a' + 'b' results in an integer and the 
    // macro is just mindlessly substiting that expression.
    void callsum2()
    {
        char z = 0;
        cout << SUM('a','b') << endl;
        cout << sum('a','b') << endl;
    }

    // Difference 4: Macros can have unintended side effects if their arguments
    // are evaluated in an unexpected manner. In this case, one of the variables
    // will be incremented twice, once before the max is returned. Not only does
    // this produce an unexpected result, but it corrupts one of the inputs.
    void callsum3()
    {
        int x = 2, y = 2;
        cout << MAX(x++,y++) << endl;
        x = y = 2;
        cout << max(x++,y++) << endl;
    }

    // Other differences:
    // Template functions could be inlined, but don't have to be. Macros are alwyas 
    // inlined because they are not true functions.
    //
    // Debuggers cannot step into macros. You would have to use the preprocessed version of
    // a cpp file in order to set a breakpoint inside a macro.
    //
    // Since macros are simple inlined substitutions, you have to be careful that
    // operator precedence doesn't change the meaning of the macro. This often means putting
    // extra parentheses around a macro expression.
    //
    // Macros don't belong to namespaces, which can lead to confusion.
    //
    // Macros can be undefined or modified willy-nilly. Defining a template function
    // multiple times results in a compile time error.
}

int main()
{
    using namespace std;
    using namespace ch13;

    callsum1();
    callsum2();
    callsum3();

    return 0;
}
