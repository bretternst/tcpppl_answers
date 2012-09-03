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
    class Class;

    template<class O>
    class Object {
        Class<O>* cls;
        std::map<std::string, Oper<O> > m_opers;
        O obj;

        template<class P>
        friend class Class;

        Object(Class<O>* cls) : cls(cls) { }
    public:
        void operator()(const std::string& id, void* arg) {
            (*cls)(obj, id, arg);
        }
    };

    // Well, this pattern is looking familiar!
    template<class O>
    class Class {
        std::map<std::string, Oper<O> > m_opers;

    public:
        void add_oper(const Oper<O>& oper) {
            m_opers[oper.id()] = oper;
        }

        void remove_oper(const std::string& id) {
            m_opers.erase(id);
        }

        void operator()(O& obj, const std::string& id, void* arg) {
            typename std::map<std::string, Oper<O> >::iterator i = m_opers.find(id);
            if(i != m_opers.end()) {
                i->second.func()(obj, arg);
            }
        }

        Object<O> create() {
            return Object<O>(this);
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
    Class<String_vector> cls;
    cls.add_oper(Oper<String_vector>("push", &fpush));
    cls.add_oper(Oper<String_vector>("pop", &fpop));
    Object<String_vector> obj = cls.create();

    string s1("foo");
    string s2("bar");
    string s3("baz");

    obj("push", &s1);
    obj("push", &s2);
    obj("push", &s3);

    string s;
    obj("pop", &s);
    cout << "popped " << s << '\n';
    obj("pop", &s);
    cout << "popped " << s << '\n';
    obj("pop", &s);
    cout << "popped " << s << '\n';
}
