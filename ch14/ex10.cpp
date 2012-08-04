#include <iostream>

namespace Exercises
{
    template<class Targ> class Callback
    {
    public:
        typedef void (CallbackFunc)(Targ);

    private:
        CallbackFunc* func;

    public:
        class FunctionNullError {};

        Callback() : func(0) {}
        Callback(CallbackFunc* funcPtr) : func(funcPtr) {}

        void Invoke(Targ) const;
    };

    template<class Targ>
    void Callback<Targ>::Invoke(Targ arg) const
    {
        if(!func) throw FunctionNullError();
        // Let the invoked function throw any exception; don't trap it
        func(arg);
    }

    template<>
    void Callback<void>::Invoke(void) const
    {
        if(!func) throw FunctionNullError();
        // Let the invoked function throw any exception; don't trap it
        func();
    }
}

void f()
{
    std::cout << "f was invoked" << std::endl;
}

void g(int x)
{
    std::cout << "g was invoked with " << x << std::endl;
}

int main()
{
    using namespace std;
    using namespace Exercises;

    Callback<void> c(&f);
    c.Invoke();

    Callback<int> c2(&g);
    c2.Invoke(1);

    return 0;
}
