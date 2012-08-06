#include <iostream>
#include <typeinfo>

namespace ch15
{
    class A
    {
    public:
        virtual ~A() {}
    };

    class B : public A
    {
    };

    class A1
    {
    public:
        virtual ~A1() {}
    };

    class C : public A1
    {
    };

    template<class T, class Tfrom> T ptr_cast(Tfrom p)
    {
        T r = dynamic_cast<T>(p);
        if(!r) throw std::bad_cast();
        return r;
    }
}

int main()
{
    using namespace std;
    using namespace ch15;

    A* a = new B;
    A1* c = new C;

    B* b = ptr_cast<B*>(a);
    try
    {
        b = ptr_cast<B*>(c);
    }
    catch(bad_cast)
    {
        cout << "caught bad_cast" << endl;
    }

    delete a;
    delete c;

    return 0;
}
