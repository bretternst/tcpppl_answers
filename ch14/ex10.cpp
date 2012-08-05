#include <iostream>

namespace ch14
{
    template<class Targ>
    class Callback
    {
    public:
        typedef void (CallbackFunc)(Targ);

    private:
        CallbackFunc* func;

    public:
        class FunctionNullError {};

        Callback() : func(0) {}
        explicit Callback(CallbackFunc* funcPtr) : func(funcPtr) {}

        void invoke(Targ) const;
    };

    template<>
    class Callback<void>
    {
    public:
        typedef void (CallbackFunc)();

    private:
        CallbackFunc* func;

    public:
        class FunctionNullError {};

        Callback() : func(0) {}
        explicit Callback(CallbackFunc* funcPtr) : func(funcPtr) {}

        void invoke() const;
    };

    template<class Targ>
    void Callback<Targ>::invoke(Targ arg) const
    {
        if(!func) throw FunctionNullError();
        // Let the invoked function throw any exception; don't trap it
        func(arg);
    }

    void Callback<void>::invoke() const
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
    using namespace ch14;

    Callback<void> c(&f);
    c.invoke();

    Callback<int> c2(&g);
    c2.invoke(1);

    return 0;
}
