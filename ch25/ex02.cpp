#include <iostream>

namespace ch25 {
    template<class X>
    class Handle {
        X* rep;
        int* pcount;
    public:
        X* operator->() { return rep; }

        Handle(X* pp) : rep(pp), pcount(new int(1)) { }

        template<class T> friend class Handle;

        template<class T>
        Handle(const Handle<T>& r) : rep(r.rep), pcount(r.pcount) { (*pcount)++; }

        template<class T>
        Handle& operator=(const Handle<T>& r) {
            if (rep == r.rep) return *this;
            if(--(*pcount) == 0) {
                delete rep;
                delete pcount;
            }
            rep = r.rep;
            pcount = r.pcount;
            (*pcount)++;
            return *this;
        }

        ~Handle() { if(--(*pcount) == 0) { delete rep; delete pcount; } }

        X* get_rep() { return rep; }

        void bind(X* pp) {
            if(pp != rep) {
                if(--(*pcount) == 0) {
                    delete rep;
                    *pcount = 1;
                }
                else
                    pcount = new int(1);
                rep = pp;
            }
        }
    };

    class Shape {
    public:
        virtual void f() { std::cout << "I'm a shape!" << std::endl; }
    };
    class Circle : public Shape {
        virtual void f() { std::cout << "I'm a circle!" << std::endl; }
    };
}

void f(ch25::Handle<ch25::Shape> hs) {
    hs->f();
}

int main() {
    using namespace ch25;

    Circle* c = new Circle();
    Shape* s = new Shape();

    Handle<Circle> hc(c);
    Handle<Shape> hs(s);
    f(hs); // I'm a shape!
    f(hc); // I'm a circle!
    hs = hc;
    hs->f(); // I'm a circle!
}
