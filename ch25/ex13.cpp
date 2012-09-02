#include <stack>
#include <vector>
#include <iostream>

namespace ch25 {
    class Underflow_error { };

    template<class T>
    class Stack_impl {
    public:
        virtual size_t size() const = 0;
        virtual bool empty() const { return size() == 0; }
        virtual void push(const T& val) = 0;
        virtual T pop() = 0;
        virtual ~Stack_impl() { }
    };

    template<class T>
    class Stack_stack : public Stack_impl<T> {
        std::stack<T> stack;
    public:
        size_t size() const { return stack.size(); }
        void push(const T& val) { stack.push(val); }
        T pop() {
            if(stack.empty())
                throw Underflow_error();
            T val = stack.top();
            stack.pop();
            return val;
        }
    };

    template<class T>
    class Stack_vector : public Stack_impl<T> {
        std::vector<T> stack;
    public:
        size_t size() const { return stack.size(); }
        void push(const T& val) { stack.push_back(val); }
        T pop() {
            if(stack.empty())
                throw Underflow_error();
            T val = stack.back();
            stack.pop_back();
            return val;
        }
    };

    template<class T>
    class Stack {
        Stack_impl<T>* m_stack;

    public:
        Stack() : m_stack(new Stack_stack<T>()) { } // default implementation
        explicit Stack(Stack_impl<T>* stack) : m_stack(stack) { }
        ~Stack() { delete m_stack; }

        size_t size() const { return m_stack->size(); }
        bool empty() const { return m_stack->empty(); }
        void push(const T& val) { m_stack->push(val); }
        T pop() { return m_stack->pop(); }
        Stack_impl<T>* change(Stack_impl<T>* stack) {
            // should the stack contents be copied?
            Stack_impl<T>* p = m_stack;
            m_stack = stack;
            return p;
        }
    };
}

int main() {
    using namespace std;
    using namespace ch25;

    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.pop();
    Stack_impl<int>* p = s.change(new Stack_vector<int>());
    delete p;
    s.push(4);
    s.push(5);
    cout << s.pop() << ' ' << s.pop() << '\n'; // 5 4 
    try {
        s.pop();
    }
    catch(Underflow_error) {
        std::cout << "caught Underflow_error\n";
    }
}
