#include <iostream>
#include <vector>
#include <algorithm>

namespace ch18 {
    //
    // unary pointer
    //
    template<class R, class T>
    class mem_fun_t : public std::unary_function<T*, R> {
        R (T::*pmf)();
    public:
        explicit mem_fun_t(R (T::*p)()) : pmf(p) {}
        R operator()(T* p) const {
            return (p->*pmf)();
        }
    };

    template<class R, class T>
    mem_fun_t<R,T> mem_fun(R (T::*f)()) {
        return mem_fun_t<R,T>(f);
    }

    template<class R, class T>
    class const_mem_fun_t : public std::unary_function<T*, R> {
        R (T::*pmf)() const;
    public:
        explicit const_mem_fun_t(R (T::*p)() const) : pmf(p) {}
        R operator()(T* p) const {
            return (p->*pmf)();
        }
    };

    template<class R, class T>
    const_mem_fun_t<R,T> mem_fun(R (T::*f)() const) {
        return const_mem_fun_t<R,T>(f);
    }


    //
    // binary pointer
    //
    template<class R, class T, class Arg>
    class mem_fun1_t : public std::binary_function<T*, Arg, R> {
        R (T::*pmf)(Arg x);
    public:
        explicit mem_fun1_t(R (T::*p)(Arg x)) : pmf(p) {}
        R operator()(T* p, Arg x) const {
            return (p->*pmf)(x);
        }
    };

    template<class R, class T, class Arg>
    mem_fun1_t<R,T,Arg> mem_fun(R (T::*f)(Arg x)) {
        return mem_fun1_t<R,T,Arg>(f);
    }

    template<class R, class T, class Arg>
    class const_mem_fun1_t : public std::binary_function<T*, Arg, R> {
        R (T::*pmf)(Arg x) const;
    public:
        explicit const_mem_fun1_t(R (T::*p)(Arg x) const) : pmf(p) {}
        R operator()(T* p, Arg x) const {
            return (p->*pmf)(x);
        }
    };

    template<class R, class T, class Arg>
    const_mem_fun1_t<R,T,Arg> mem_fun(R (T::*f)(Arg x) const) {
        return const_mem_fun1_t<R,T,Arg>(f);
    }


    //
    // unary reference
    //
    template<class R, class T>
    class mem_fun_ref_t : public std::unary_function<T&, R> {
        R (T::*pmf)();
    public:
        explicit mem_fun_ref_t(R (T::*p)()) : pmf(p) {}
        R operator()(T& p) const {
            return (p.*pmf)();
        }
    };

    template<class R, class T>
    mem_fun_ref_t<R,T> mem_fun_ref(R (T::*f)()) {
        return mem_fun_ref_t<R,T>(f);
    }

    template<class R, class T>
    class const_mem_fun_ref_t : public std::unary_function<T&, R> {
        R (T::*pmf)() const;
    public:
        explicit const_mem_fun_ref_t(R (T::*p)() const) : pmf(p) {}
        R operator()(T& p) const {
            return (p.*pmf)();
        }
    };

    template<class R, class T>
    const_mem_fun_ref_t<R,T> mem_fun_ref(R (T::*f)() const) {
        return const_mem_fun_ref_t<R,T>(f);
    }



    // 
    // binary reference
    //
    template<class R, class T, class Arg>
    class mem_fun1_ref_t : public std::binary_function<T, Arg, R> {
        R (T::*pmf)(Arg x);
    public:
        explicit mem_fun1_ref_t(R (T::*p)(Arg x)) : pmf(p) {}
        R operator()(T& p, Arg x) const {
            return (p.*pmf)(x);
        }
    };

    template<class R, class T, class Arg>
    mem_fun1_ref_t<R,T,Arg> mem_fun_ref(R (T::*f)(Arg x)) {
        return mem_fun1_ref_t<R,T,Arg>(f);
    }

    template<class R, class T, class Arg>
    class const_mem_fun1_ref_t : public std::binary_function<T, Arg, R> {
        R (T::*pmf)(Arg x) const;
    public:
        explicit const_mem_fun1_ref_t(R (T::*p)(Arg x) const) : pmf(p) {}
        R operator()(T& p, Arg x) const {
            return (p.*pmf)(x);
        }
    };

    template<class R, class T, class Arg>
    const_mem_fun1_ref_t<R,T,Arg> mem_fun_ref(R (T::*f)(Arg x) const) {
        return const_mem_fun1_ref_t<R,T,Arg>(f);
    }


    class Test {
        int x;
    public:
        Test(int x) : x(x) { }
        int no_args() {
            std::cout << "no_args called: " << x << std::endl;
            return x;
        }
        int const_no_args() {
            std::cout << "const_no_args called: " << x << std::endl;
            return x;
        }
        int one_arg(int x) {
            std::cout << "one_args called: " << this->x << " (" << x << ")" << std::endl;
            return x;
        }
        int const_one_arg(int x) {
            std::cout << "const_one_args called: " << this->x << " (" << x << ")" << std::endl;
            return x;
        }
    };
}

int main() {
    using namespace std;
    using namespace ch18;

    Test t1(1);
    Test t2(2);
    Test t3(3);

    vector<Test*> v;
    v.push_back(&t1);
    v.push_back(&t2);
    v.push_back(&t3);

    for_each(v.begin(), v.end(), ch18::mem_fun(&Test::no_args));
    for_each(v.begin(), v.end(), bind2nd(ch18::mem_fun(&Test::one_arg), 4));
    for_each(v.begin(), v.end(), ch18::mem_fun(&Test::const_no_args));
    for_each(v.begin(), v.end(), bind2nd(ch18::mem_fun(&Test::const_one_arg), 4));

    vector<Test> v1;
    v1.push_back(t1);
    v1.push_back(t2);
    v1.push_back(t3);

    for_each(v1.begin(), v1.end(), ch18::mem_fun_ref(&Test::no_args));
    for_each(v1.begin(), v1.end(), bind2nd(ch18::mem_fun_ref(&Test::one_arg), 5));
    for_each(v1.begin(), v1.end(), ch18::mem_fun_ref(&Test::const_no_args));
    for_each(v1.begin(), v1.end(), bind2nd(ch18::mem_fun_ref(&Test::const_one_arg), 5));
}