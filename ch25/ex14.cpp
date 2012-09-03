#include <iostream>
#include <vector>
#include <map>

namespace ch25 {
    class Func {
    public:
        virtual void operator()(void* p, void* arg) = 0;
        virtual ~Func() { }
    };

    class Oper {
        std::string m_id;
        Func* m_func;
    public:
        Oper() { }
        Oper(const std::string& id, Func* func) : m_id(id), m_func(func) { }
        std::string id() const { return m_id; }
        Func& func() { return *m_func; }
    };

    class Cat_object {
        std::map<std::string, Oper> m_opers;
        void* p;
    public:
        Cat_object(void* p) : p(p) { }

        void add_oper(const Oper& oper) {
            m_opers[oper.id()] = oper;
        }

        void remove_oper(const std::string& id) {
            m_opers.erase(id);
        }

        void operator()(const std::string& id, void* arg) {
            std::map<std::string, Oper>::iterator i = m_opers.find(id);
            if(i != m_opers.end()) {
                i->second.func()(p, arg);
            }
        }
    };

    class Cat {
        int x;
    public:
        Cat() : x(0) { }
        Cat(int x) : x(x) { }
        operator int() const { return x; }
    };

    typedef std::vector<Cat> Cat_vector;

    class Func_push : public Func {
    public:
        virtual void operator()(void* p, void* arg) {
            Cat_vector* v = static_cast<Cat_vector*>(p);
            Cat* c = static_cast<Cat*>(arg);
            v->push_back(*c);
        }
    };

    class Func_pop : public Func {
    public:
        virtual void operator()(void* p, void* arg) {
            Cat_vector* v = static_cast<Cat_vector*>(p);
            Cat* c = static_cast<Cat*>(arg);
            *c = v->back();
            v->pop_back();
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    Func_push fpush;
    Func_pop fpop;
    Cat_vector stack;
    Cat_object opers(&stack);
    opers.add_oper(Oper("push", &fpush));
    opers.add_oper(Oper("pop", &fpop));

    Cat cat1(1);
    Cat cat2(2);
    Cat cat3(3);

    opers("push", &cat1);
    opers("push", &cat2);
    opers("push", &cat3);

    Cat c;
    opers("pop", &c);
    cout << "popped " << c << '\n';
    opers("pop", &c);
    cout << "popped " << c << '\n';
    opers("pop", &c);
    cout << "popped " << c << '\n';
}
