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
        Object() { }

        void operator()(const std::string& id, void* arg) {
            (*cls)(obj, id, arg);
        }
    };

    class Invalid_op { };

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
            } else {
                throw Invalid_op();
            }
        }

        Object<O> create() {
            return Object<O>(this);
        }
    };

    template<class T>
    class Func_push : public Func<std::vector<T> > {
    public:
        virtual void operator()(std::vector<T>& v, void* arg) {
            T* c = static_cast<T*>(arg);
            v.push_back(*c);
        }
    };

    template<class T>
    class Func_pop : public Func<std::vector<T> > {
    public:
        virtual void operator()(std::vector<T>& v, void* arg) {
            T* c = static_cast<T*>(arg);
            *c = v.back();
            v.pop_back();
        }
    };

    template<class T>
    class Stack {
        static Class<std::vector<T> > cls;
        struct Init {
            Init();
            void f() { }
        };
        static Init init;
        static Func_push<T> fpush;
        static Func_pop<T> fpop;

        Object<std::vector<T> > obj;

    public:
        Stack() : obj(cls.create()) { init.f(); } // force initialization of static members
        void push(const T& val) {
            obj("push", const_cast<void*>(static_cast<const void*>(&val)));
        }
        T pop() {
            T val;
            obj("pop", &val);
            return val;
        }
    };

    template<class T>
    Stack<T>::Init::Init() {
        std::cerr << "adding operations" << std::endl;
        Stack<T>::cls.add_oper(Oper<std::vector<T> >("push", &Stack<T>::fpush));
        Stack<T>::cls.add_oper(Oper<std::vector<T> >("pop", &Stack<T>::fpop));
    }

    template<class T>
    Class<std::vector<T> > Stack<T>::cls;
    template<class T>
    Func_push<T> Stack<T>::fpush;
    template<class T>
    Func_pop<T> Stack<T>::fpop;
    template<class T>
    typename Stack<T>::Init Stack<T>::init;
}

int main() {
    using namespace std;
    using namespace ch25;

    Stack<string> stack;
    stack.push("foo");
    stack.push("bar");
    stack.push("baz");
    cout << stack.pop() << endl;
    cout << stack.pop() << endl;
    cout << stack.pop() << endl;
}
