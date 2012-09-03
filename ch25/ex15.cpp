#include <iostream>
#include <vector>
#include <map>

namespace ch25 {
    template<class O>
    class Func {
    public:
        virtual void operator()(O& obj, void* arg) = 0;
        virtual ~Func() { }
    };

    template<class O>
    class Oper {
        std::string m_id;
        Func<O>* m_func;
    public:
        Oper() { }
        Oper(const std::string& id, Func<O>* func) : m_id(id), m_func(func) { }
        std::string id() const { return m_id; }
        Func<O>& func() { return *m_func; }
    };

    template<class O>
    class Object {
        std::map<std::string, Oper<O> > m_opers;
        O obj;
    public:
        void add_oper(const Oper<O>& oper) {
            m_opers[oper.id()] = oper;
        }

        void remove_oper(const std::string& id) {
            m_opers.erase(id);
        }

        void operator()(const std::string& id, void* arg) {
            typename std::map<std::string, Oper<O> >::iterator i = m_opers.find(id);
            if(i != m_opers.end()) {
                i->second.func()(obj, arg);
            }
        }
    };

    typedef std::vector<std::string> String_vector;

    class Func_push : public Func<String_vector> {
    public:
        virtual void operator()(String_vector& v, void* arg) {
            std::string* c = static_cast<std::string*>(arg);
            v.push_back(*c);
        }
    };

    class Func_pop : public Func<String_vector> {
    public:
        virtual void operator()(String_vector& v, void* arg) {
            std::string* c = static_cast<std::string*>(arg);
            *c = v.back();
            v.pop_back();
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    Func_push fpush;
    Func_pop fpop;
    Object<String_vector> opers;
    opers.add_oper(Oper<String_vector>("push", &fpush));
    opers.add_oper(Oper<String_vector>("pop", &fpop));

    string s1("foo");
    string s2("bar");
    string s3("baz");

    opers("push", &s1);
    opers("push", &s2);
    opers("push", &s3);

    string s;
    opers("pop", &s);
    cout << "popped " << s << '\n';
    opers("pop", &s);
    cout << "popped " << s << '\n';
    opers("pop", &s);
    cout << "popped " << s << '\n';
}
