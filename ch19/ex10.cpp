#include <iostream>
#include <limits>
#include <vector>

// This vector implementation is re-used from ch14 ex7, but modified
// to use an allocator.
namespace ch19 {
    template<class T, class A = std::allocator<T> > class Vector;

    template<class T> class Iterator
    {
        int i;
        Vector<T>* container;

    public:
        Iterator(int i, Vector<T>* container) : i(i), container(container) {}
        T& operator*() { return (*container)[i]; }
        const T& operator*() const { return *container[i]; }
        T* operator->() { return &(*container)[i]; }
        T const* operator->() const { return &*container[i]; }
        Iterator<T>& operator++() { i++; return *this; }
        Iterator<T> operator++(int) { Iterator<T> copy = *this; i++; return copy; }
        Iterator<T>& operator--() { i--; return *this; }
        Iterator<T> operator--(int) { Iterator<T> copy = *this; i--; return copy; }
        Iterator<T>& operator+=(int x) { i += x; return *this; }
        Iterator<T> operator+(int x) { Iterator<T> copy = *this; copy += x; return copy; }
        Iterator<T>& operator-=(int x) { i -= x; return *this; }
        Iterator<T> operator-(int x) { Iterator<T> copy = *this; copy -= x; return copy; }
        bool operator<(const Iterator<T>& x) const { return i < x.i; }
        bool operator>(const Iterator<T>& x) const { return i > x.i; }
        bool operator<=(const Iterator<T>& x) const { return i <= x.i; }
        bool operator>=(const Iterator<T>& x) const { return i >= x.i; }
        bool operator==(const Iterator<T>& x) const { return i == x.i; }
        bool operator!=(const Iterator<T>& x) const { return i != x.i; }
        T& operator[](int idx) { return *container[i+idx]; }
        const T& operator[](int idx) const { return *container[i+idx]; }
    };

    template<class T> class ReverseIterator
    {
        int i;
        Vector<T>* container;

    public:
        ReverseIterator(int i, Vector<T>* container) : i(i), container(container) {}
        T& operator*() { return (*container)[i]; }
        const T& operator*() const { return *container[i]; }
        T* operator->() { return &(*container)[i]; }
        T const* operator->() const { return &*container[i]; }
        ReverseIterator<T>& operator++() { i--; return *this; }
        ReverseIterator<T> operator++(int) { ReverseIterator<T> copy = *this; i--; return copy; }
        ReverseIterator<T>& operator--() { i++; return *this; }
        ReverseIterator<T> operator--(int) { ReverseIterator<T> copy = *this; i++; return copy; }
        ReverseIterator<T>& operator+=(int x) { i -= x; return *this; }
        ReverseIterator<T> operator+(int x) { ReverseIterator<T> copy = *this; copy += x; return copy; }
        ReverseIterator<T>& operator-=(int x) { i += x; return *this; }
        ReverseIterator<T> operator-(int x) { ReverseIterator<T> copy = *this; copy -= x; return copy; }
        bool operator<(const ReverseIterator<T>& x) const { return i < x.i; }
        bool operator>(const ReverseIterator<T>& x) const { return i > x.i; }
        bool operator<=(const ReverseIterator<T>& x) const { return i <= x.i; }
        bool operator>=(const ReverseIterator<T>& x) const { return i >= x.i; }
        bool operator==(const ReverseIterator<T>& x) const { return i == x.i; }
        bool operator!=(const ReverseIterator<T>& x) const { return i != x.i; }
        T& operator[](int idx) { return *container[i-idx]; }
        const T& operator[](int idx) const { return *container[i-idx]; }
    };

    template<class T, class A> class Vector
    {
        static const int DefaultCapacity = 32;
        static const int ReallocFactor = 2;

        A alloc;
        T* p;
        int sz;
        int cp;

    public:
        class RangeError {};
        class SizeError {};

        typedef Iterator<T> iterator;
        typedef const Iterator<T> const_iterator;
        typedef ReverseIterator<T> reverse_iterator;
        typedef const ReverseIterator<T> const_reverse_iterator;

        Vector() : sz(0), cp(DefaultCapacity) {
            p = alloc.allocate(DefaultCapacity);
        }
        Vector(int size, const T& value = T()) ;
        Vector(const Vector<T,A>& x);
        template<class InputIterator> Vector(InputIterator first, InputIterator last);
        ~Vector() { alloc.deallocate(p, cp); }

        Vector<T,A>& operator=(const Vector<T,A>& x);
        iterator begin() { return iterator(0, this); }
        const_iterator begin() const { return iterator(0, this); }
        iterator end() { return iterator(sz, this); }
        const_iterator end() const { return iterator(sz, this); }
        reverse_iterator rbegin() { return reverse_iterator(sz-1, this); }
        const_reverse_iterator rbegin() const { return reverse_iterator(sz-1, this); }
        reverse_iterator rend() { return reverse_iterator(-1, this); }
        const_reverse_iterator rend() const { return reverse_iterator(-1, this); }
        int size() const { return sz; }
        int max_size() const { return std::numeric_limits<int>::max(); }
        void resize(int sz, T c = T());
        int capacity() const { return cp; }
        bool empty() const { return sz == 0; }
        void reserve(int n);
        T& operator[](int i) { return p[i]; }
        const T& operator[](int i) const { return p[i]; }
        T& at(int i) { if(i>=sz || i < 0) throw RangeError(); return p[i]; }
        const T& at(int i) const { if(i>=sz || i < 0) throw RangeError(); return p[i]; }
        T& front() { if(sz<1) throw SizeError(); return p[0]; }
        const T& front() const { if(sz<1) throw SizeError(); return p[0]; }
        T& back() { if(sz<1) throw SizeError(); return p[sz-1]; }
        const T& back() const { if(sz<1) throw SizeError(); return p[sz-1]; }
        void assign(int n, const T& u);
        void push_back(const T& x);
        void pop_back();
        iterator insert(iterator pos, const T& x);
        void insert(iterator pos, int n, const T& x);
        template<class InputIterator> void insert(iterator pos, InputIterator first, InputIterator last);
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void swap(Vector<T>& x);
        void clear();
    };

    template<class T, class A>
    Vector<T,A>::Vector(int size, const T& value) : sz(size), cp(size)
    {
        p = alloc.allocate(size);
        for(int i = 0; i < sz; i++) p[i] = value;
    }

    template<class T, class A>
    Vector<T,A>::Vector(const Vector<T,A>& x) : sz(x.sz), cp(x.sz)
    {
        p = alloc.allocate(size);
        for(int i = 0; i < sz; i++) p[i] = x.p[i];
    }

    template<class T, class A>
    template<class InputIterator>
    Vector<T,A>::Vector(InputIterator first, InputIterator last) : sz(0), cp(DefaultCapacity)
    {
        p = alloc.allocate(DefaultCapacity);
        while(first != last)
        {
            push_back(*first++);
        }
    }

    template<class T, class A>
    Vector<T,A>& Vector<T,A>::operator=(const Vector<T,A>& x)
    {
        if(cp != x.sz)
        {
            alloc.deallocate(p, cp);
            cp = sz = x.sz;
            p = alloc.allocate(sz);
            for(int i = 0; i < sz; i++) p[i] = x.p[i];
        }
        return *this;
    }

    template<class T, class A>
    void Vector<T,A>::resize(int size, T c)
    {
        if(size > sz)
        {
            int oldcp = cp;
            if(size > cp) cp = size;

            T* newp = alloc.allocate(cp);
            for(int i = 0; i < sz; i++) newp[i] = p[i];
            for(int i = sz; i < size; i++) newp[i] = c;
            alloc.deallocate(p, oldcp);
            p = newp;
        }
        else if (size < sz)
        {
            T* newp = alloc.allocate(cp);
            for(int i = 0; i < size; i++) newp[i] = p[i];
            alloc.deallocate(p, cp);
            p = newp;
        }
        sz = size;
    }

    template<class T, class A>
    void Vector<T,A>::reserve(int n)
    {
        if(n > cp)
        {
            int oldcp = cp;
            cp = n;
            T* newp = alloc.allocate(cp);
            for(int i = 0; i < sz; i++) newp[i] = p[i];
            alloc.deallocate(p, oldcp);
            p = newp;
        }
    }

    template<class T, class A>
    void Vector<T,A>::assign(int n, const T& u)
    {
        if(n > cp)
        {
            cp = n;
            alloc.deallocate(p, cp);
            p = alloc.allocate(cp);
        }
        for(int i = 0; i < n; i++) p[i] = u;
        sz = n;
    }

    template<class T, class A>
    void Vector<T,A>::push_back(const T& u)
    {
        if(sz == cp) reserve(ReallocFactor * sz);
        p[sz++] = u;
    }

    template<class T, class A>
    void Vector<T,A>::pop_back()
    {
        if(sz < 1) throw SizeError();
        p[--sz].~T();
    }

    template<class T, class A>
    typename Vector<T,A>::iterator Vector<T,A>::insert(iterator pos, const T& x)
    {
        iterator copy = pos;
        if(sz==cp) reserve(sz+1);
        sz++;
        T tmp = *pos;
        *pos = x;
        while(pos != end())
        {
            pos++;
            std::swap(tmp, *pos);
        }
        return copy;
    }

    template<class T, class A>
    void Vector<T,A>::insert(iterator pos, int n, const T& x)
    {
        if(sz + n > cp) reserve(sz + n);
        sz += n;
        for(int i = sz-1; i >= sz-n-1; i--)
            p[i] = p[i-n];
        for(int i = 0; i < n; i++)
            *(pos++) = x;
    }

    template<class T, class A>
    template<class InputIterator>
    void Vector<T,A>::insert(iterator pos, InputIterator first, InputIterator last)
    {
        Vector<T> newvec;
        newvec.reserve(sz);
        iterator i = begin();
        while(i != pos)
        {
            newvec.push_back(*i++);
        }
        while (first != last)
        {
            newvec.push_back(*first++);
        }
        while(i != end())
        {
            newvec.push_back(*i++);
        }
        swap(newvec);
    }

    template<class T, class A>
    typename Vector<T,A>::iterator Vector<T,A>::erase(iterator pos)
    {
        while(pos != end())
        {
            *pos = *(pos+1);
            pos++;
        }
        sz--;
        return pos;
    }

    template<class T, class A>
    typename Vector<T,A>::iterator Vector<T,A>::erase(iterator first, iterator last)
    {
        iterator i = first;
        iterator i2 = last;
        int removed = 0;
        while(i != last)
        {
            removed++;
            if(i2 != end())
            {
                *i = *i2;
                i2++;
            }
            i++;
        }
        sz -= removed;
        return i;
    }

    template<class T, class A>
    void Vector<T,A>::swap(Vector<T>& x)
    {
        std::swap(cp, x.cp);
        std::swap(sz, x.sz);
        std::swap(p, x.p);
    }

    template<class T, class A>
    void Vector<T,A>::clear()
    {
        for(int i = 0; i < sz; i++) p[i].~T();
        sz = 0;
    }
}



template<class T1, class T2> void compare_vec(T1& v1, T2& v2)
{
    using namespace std;
    static int count = 0;

    cout << count++ << ": ";

    if(v1.size() != v2.size())
    {
        cout << "sizes don't match: " << v1.size() << " vs. " << v2.size() << endl;
        return;
    }
    for(int i = 0; i < v1.size(); i++)
    {
        if(v1[i] != v2[i])
        {
            cout << "element " << i << " doesn't match" << endl;
            return;
        }
    }
    cout << "match" << endl;
}

int main()
{
    using namespace std;
    using namespace ch19;

    vector<int> tmpl;
    tmpl.push_back(1);
    tmpl.push_back(2);
    tmpl.push_back(3);
    tmpl.push_back(4);
    tmpl.push_back(5);

    Vector<int> V1(tmpl.begin(), tmpl.end());
    vector<int> v1(tmpl.begin(), tmpl.end());
    compare_vec(V1,v1);

    Vector<int> V2(V1.begin(), V1.end());
    compare_vec(V2,v1);

    Vector<int> V3(V1.rbegin(), V1.rend());
    vector<int> v3(v1.rbegin(), v1.rend());
    compare_vec(V3,v3);

    try
    {
        cout << V3.at(5) << endl;
    }
    catch(Vector<int>::RangeError)
    {
        cout << "caught range error" << endl;
    }

    V3.clear();
    try
    {
        cout << V3.front() << endl;
    }
    catch(Vector<int>::SizeError)
    {
        cout << "caught size error" << endl;
    }

    V1.erase(V1.begin()+2);
    v1.erase(v1.begin()+2);
    compare_vec(V1,v1);

    V1.erase(V1.begin()+1,V1.end());
    v1.erase(v1.begin()+1,v1.end());
    compare_vec(V1,v1);

    V1 = Vector<int>(tmpl.begin(),tmpl.end());
    V2 = Vector<int>(tmpl.begin(),tmpl.end());
    v1 = vector<int>(tmpl.begin(),tmpl.end());
    vector<int> v2 = vector<int>(tmpl.begin(),tmpl.end());

    V1.insert(V1.begin()+2,3,2);
    v1.insert(v1.begin()+2,3,2);
    compare_vec(V1,v1);

    V1.insert(V1.begin()+3,V2.begin(),V2.end());
    v1.insert(v1.begin()+3,v2.begin(),v2.end());
    compare_vec(V1,v1);

    int q = 5;
    int r = 7;
    Vector<int*> Vp;
    Vp.push_back(&q);
    Vp.push_back(&r);
    Vp.push_back(&q);
    Vp.push_back(&q);
    Vp.push_back(&r);
    Vp.pop_back();

    vector<int*> vp;
    vp.push_back(&q);
    vp.push_back(&r);
    vp.push_back(&q);
    vp.push_back(&q);
    vp.push_back(&r);
    vp.pop_back();

    Vp.insert(Vp.begin()+2,&r);
    vp.insert(vp.begin()+2,&r);

    compare_vec(Vp,vp);

    return 0;
}
