#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <fstream>

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


    typedef std::stack<int> Int_stack;

    class Func_push : public Func<Int_stack> {
    public:
        virtual void operator()(Int_stack& v, void* arg) {
            int* c = static_cast<int*>(arg);
            v.push(*c);
        }
    };

    class Func_math_oper : public Func<Int_stack> {
    protected:
        virtual int op(int x, int y) = 0;
        void operator()(Int_stack& v, void* arg) {
            int* c = static_cast<int*>(arg);
            int x = v.top();
            v.pop();
            int y = v.top();
            v.pop();
            v.push(op(x,y));
            *c = v.top();
        }
    };

    class Func_add : public Func_math_oper {
    protected:
        int op(int x, int y) { return x + y; }
    };

    class Func_sub : public Func_math_oper {
    protected:
        int op(int x, int y) { return x - y; }
    };

    class Func_mul : public Func_math_oper {
    protected:
        int op(int x, int y) { return x * y; }
    };

    class Func_div : public Func_math_oper {
    protected:
        int op(int x, int y) { return x / y; }
    };

    // List of commands for transmission
    template<class T>
    struct Command {
        std::string name;
        T arg;
        Command(const std::string& name, const T& arg) : name(name), arg(arg) { }
        explicit Command(std::istream& in) {
            in >> name;
            in >> arg;
        }
        void write(std::ostream& out) {
            out << name << ' ' << arg << ' ';
        }
    };

    template<class T>
    struct Command_list {
        std::vector<Command<T> > commands;
    public:
        Command_list() { }
        explicit Command_list(std::istream& in) {
            for(;;) {
                Command<T> cmd(in);
                if(in)
                    commands.push_back(cmd);
                else
                    break;
            }
        }
        void add_command(const std::string& name, const T& arg) {
            commands.push_back(Command<T>(name, arg));
        }
        void write(std::ostream& out) {
            for(typename std::vector<Command<T> >::iterator i = commands.begin(); i != commands.end(); i++) {
                i->write(out);
            }
        }

        template<class O>
        T execute(O& obj) {
            T res;
            for(typename std::vector<Command<T> >::iterator i = commands.begin(); i != commands.end(); i++) {
                res = i->arg;
                obj(i->name, &res);
            }
            return res;
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    Func_push fpush;
    Func_add fadd;
    Func_sub fsub;
    Func_mul fmul;
    Func_div fdiv;

    Class<Int_stack> cls;
    cls.add_oper(Oper<Int_stack>("push", &fpush));
    cls.add_oper(Oper<Int_stack>("add", &fadd));
    cls.add_oper(Oper<Int_stack>("sub", &fsub));
    cls.add_oper(Oper<Int_stack>("mul", &fmul));
    cls.add_oper(Oper<Int_stack>("div", &fdiv));

    // set up some calculations
    Command_list<int> cwrite;
    cwrite.add_command("push", 2);
    cwrite.add_command("push", 3);
    cwrite.add_command("mul", 0); // 6
    cwrite.add_command("push", 10);
    cwrite.add_command("add", 0); // 16

    // persist operations (or could send them to another machine or something)
    {
        ofstream out("ex18.txt");
        cwrite.write(out);
    }

    // read operations in and execute
    ifstream in("ex18.txt");
    Command_list<int> cread(in);
    Object<Int_stack> obj = cls.create();
    cout << cread.execute(obj) << endl; // 16
}
