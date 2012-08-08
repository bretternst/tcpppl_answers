#include <iostream>
#include <limits>
#include <QtCore/QVector>

namespace ch16
{
    // Compile with:
    // g++ -I/usr/include/qt4 -o ex14 ex14.cpp -lQtCore
    //
    // This answer uses the Qt4 library. Adjust the include path as necessary for your
    // distro. The QVector class is very much like the std vector class, but to keep
    // with the spirit of this exercise, I use a very minimal set of functions from it.
    // I essentially just use it as a backing store, pretending that it only has a very
    // basic interface.
    //
    // The backing QVector may not match the size of the vector itself, which is why I
    // store the size separately. I could have used the full functionality of the class,
    // making this a wrapper that does almost nothing, but I think the purpose of this
    // exercise was to adapt a different kind of container to work like the STL one.
    //
    // The last part of the question asks how I would deal with functionality available
    // from the non-standard vector that doesn't exist in the standard vector. Two
    // approaches to this: 1) Don't - if the user expects to use this class like the
    // standard vector, then the additional functionality is not needed, or 2) Provide
    // those extra functions as part of this class and call through to QVector. It
    // would no longer be standard, but would have non-standard extensions instead.
    // That way, it could be used by users with both sets of expectations.
    template<class T> class ReverseIterator
    {
        T* i;

    public:
        ReverseIterator() : i(0) {}
        ReverseIterator(T* i) : i(i) {}
        T& operator*() { return *i; }
        const T& operator*() const { return *i; }
        T* operator->() { return i; }
        T const* operator->() const { return i; }
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
        T& operator[](int idx) { return *(i-idx); }
        const T& operator[](int idx) const { return *(i-idx); }
    };

    template<class T> class Vector
    {
        QVector<T> arr;
        int sz; // store actual size, not capacity

    public:
        class RangeError {};
        class SizeError {};

        typedef T* iterator;
        typedef const T* const const_iterator;
        typedef ReverseIterator<T> reverse_iterator;
        typedef const ReverseIterator<T> const_reverse_iterator;

        Vector() : sz(0) {}
        Vector(int size, const T& value = T());
        Vector(const Vector<T>& x);
        template<class InputIterator> Vector(InputIterator first, InputIterator last);
        ~Vector() { }

        Vector<T>& operator=(const Vector<T>& x);
        iterator begin() { return &arr[0]; }
        const_iterator begin() const { return &arr[0]; }
        iterator end() { return &arr[sz-1]+1; }
        const_iterator end() const { return &arr[sz-1]+1; }
        reverse_iterator rbegin() { return reverse_iterator(&arr[sz-1]); }
        const_reverse_iterator rbegin() const { return reverse_iterator(&arr[sz-1]); }
        reverse_iterator rend() { return reverse_iterator(&arr[0]-1); }
        const_reverse_iterator rend() const { return reverse_iterator(&arr[0]-1); }
        int size() const { return sz; }
        int max_size() const { return std::numeric_limits<int>::max(); }
        void resize(int sz, T c = T());
        int capacity() const { return arr.size(); }
        bool empty() const { return sz == 0; }
        void reserve(int n);
        T& operator[](int i) { return arr[i]; }
        const T& operator[](int i) const { return arr[i]; }
        T& at(int i) { if(i>=sz) throw RangeError(); return arr[i]; }
        const T& at(int i) const { if(i>=sz) throw RangeError(); return arr[i]; }
        T& front() { if(sz<1) throw SizeError(); return arr[0]; }
        const T& front() const { if(sz<1) throw SizeError(); return arr[0]; }
        T& back() { if(sz<1) throw SizeError(); return arr[sz-1]; }
        const T& back() const { if(sz<1) throw SizeError(); return arr[sz-1]; }
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

    template<class T>
    Vector<T>::Vector(int size, const T& value) : sz(size), arr(size)
    {
        for(int i = 0; i < sz; i++) arr[i] = value;
    }

    template<class T>
    Vector<T>::Vector(const Vector<T>& x) : sz(x.size()), arr(size)
    {
        for(int i = 0; i < x.size(); i++) arr[i]=x[i];  
    }

    template<class T>
    template<class InputIterator>
    Vector<T>::Vector(InputIterator first, InputIterator last) : sz(last-first)
    {
        while(first != last) arr.push_back(*first++);
    }

    template<class T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& x)
    {
        if(arr.size() != x.size())
            arr.resize(sz = x.size());
        for(int i = 0; i < x.size(); i++) arr[i] = x[i];
        return *this;
    }

    template<class T>
    void Vector<T>::resize(int size, T c)
    {
        int osz = arr.size();
        arr.resize(sz = size);
        for(int i = osz; i < size; i++)
        {
            arr[i] = c;
        }
    }

    template<class T>
    void Vector<T>::reserve(int n)
    {
        arr.resize(n);
    }

    template<class T>
    void Vector<T>::assign(int n, const T& u)
    {
        if(n > arr.size())
            arr.resize(sz = n);
        for(int i = 0; i < n; i++) arr[i] = u;
    }

    template<class T>
    void Vector<T>::push_back(const T& u)
    {
        if(sz == arr.size())
            arr.push_back(u);
        else
            arr[sz] = u;
        sz++;
    }

    template<class T>
    void Vector<T>::pop_back()
    {
        --sz;
    }

    template<class T>
    typename Vector<T>::iterator Vector<T>::insert(iterator pos, const T& x)
    {
        arr.insert(arr.begin()+(pos - &arr[0]), x);
        sz++;
    }

    template<class T>
    void Vector<T>::insert(iterator pos, int n, const T& x)
    {
        while(n-- > 0)
        {
            arr.insert(arr.begin()+(pos++ - &arr[0]), x);
            sz++;
        }
    }

    template<class T>
    template<class InputIterator>
    void Vector<T>::insert(iterator pos, InputIterator first, InputIterator last)
    {
        while(first != last)
        {
            arr.insert(arr.begin()+(pos++ - &arr[0]), *(first++));
            sz++;
        }
    }

    template<class T>
    typename Vector<T>::iterator Vector<T>::erase(iterator pos)
    {
        arr.remove(pos - &arr[0]);
        sz--;
        return pos;
    }

    template<class T>
    typename Vector<T>::iterator Vector<T>::erase(iterator first, iterator last)
    {
        arr.remove(first - &arr[0], last-first);
        sz -= last-first;
        return first;
    }

    template<class T>
    void Vector<T>::swap(Vector<T>& x)
    {
        std::swap(sz,x.sz);
        std::swap(arr,x.arr);
    }

    template<class T>
    void Vector<T>::clear()
    {
        arr.clear();
        sz = 0;
    }
}

int main()
{
    using namespace std;
    using namespace ch16;

    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    for(Vector<int>::iterator i = v.begin(); i != v.end(); i++)
    {
        cout << *i;
    }
    cout << endl;

    v[1] = 4;
    for(Vector<int>::iterator i = v.begin(); i != v.end(); i++)
    {
        cout << *i;
        *i = 0;
    }
    cout << endl;

    for(Vector<int>::iterator i = v.begin(); i != v.end(); i++)
    {
        cout << *i;
        *i = 0;
    }
    cout << endl;

    v.erase(v.begin());
    for(Vector<int>::iterator i = v.begin(); i != v.end(); i++)
    {
        cout << *i;
        *i = 0;
    }
    cout << endl;

    return 0;
}
