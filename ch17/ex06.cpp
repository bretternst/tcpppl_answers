#include <iostream>
#include <stack>

namespace ch17
{
    template<class T>
    class Queue
    {
    private:
        std::stack<T> s1;
        std::stack<T> s2;
    public:
        class UnderflowError {};

        Queue() {}
        void push_back(T x) { s1.push(x); }
        T pop_front();
        bool empty() const { return s1.empty() && s2.empty(); }
    };

    template<class T>
    T Queue<T>::pop_front()
    {
        if(s2.empty())
        {
            if(s1.empty()) throw UnderflowError();
            for(int i = s1.size(); i > 0; --i)
            {
                s2.push(s1.top());
                s1.pop();
            }
        }

        T x = s2.top();
        s2.pop();
        return x;
    }
}

int main()
{
    using namespace std;
    using namespace ch17;

    Queue<int> q;
    q.push_back(1);
    q.push_back(2);
    q.push_back(3);

    cout << q.pop_front() << endl;
    q.push_back(4);
    q.push_back(5);

    while(!q.empty())
    {
        cout << q.pop_front() << endl;
    }

    return 0;
}
