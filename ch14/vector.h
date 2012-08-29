#include <iostream>
#include <limits>

namespace ch14
{
    template<class T> class Vector
    {
        static const int DefaultCapacity = 32;
        static const int ReallocFactor = 2;

        T* p;
        int sz;
        int cp;

    public:
        class RangeError {};
        class SizeError {};

        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        Vector() : p(new T[DefaultCapacity]), sz(0), cp(DefaultCapacity) {}
        Vector(int size, const T& value = T()) ;
        Vector(const Vector<T>& x);
        template<class InputIterator> Vector(InputIterator first, InputIterator last);
        ~Vector() { delete[] p; }

        Vector<T>& operator=(const Vector<T>& x);
        iterator begin() { return p; }
        const_iterator begin() const { return p; }
        iterator end() { return p + sz; }
        const_iterator end() const { return p + sz; }
        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return reverse_iterator(begin()); }
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

    template<class T>
    Vector<T>::Vector(int size, const T& value) : sz(size), p(new T[size]), cp(size)
    {
        for(int i = 0; i < sz; i++) p[i] = value;
    }

    template<class T>
    Vector<T>::Vector(const Vector<T>& x) : sz(x.sz), p(new T[x.sz]), cp(x.sz)
    {
        for(int i = 0; i < sz; i++) p[i] = x.p[i];
    }

    template<class T>
    template<class InputIterator>
    Vector<T>::Vector(InputIterator first, InputIterator last) : sz(0), p(new T[DefaultCapacity]), cp(DefaultCapacity)
    {
        while(first != last)
        {
            push_back(*first++);
        }
    }

    template<class T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& x)
    {
        if(cp != x.sz)
        {
            delete[] p;
            cp = sz = x.sz;
            p = new T[sz];
            for(int i = 0; i < sz; i++) p[i] = x.p[i];
        }
        return *this;
    }

    template<class T>
    void Vector<T>::resize(int size, T c)
    {
        if(size > sz)
        {
            if(size > cp) cp = size;

            T* newp = new T[cp];
            for(int i = 0; i < sz; i++) newp[i] = p[i];
            for(int i = sz; i < size; i++) newp[i] = c;
            delete[] p;
            p = newp;
        }
        else if (size < sz)
        {
            T* newp = new T[cp];
            for(int i = 0; i < size; i++) newp[i] = p[i];
            delete[] p;
            p = newp;
        }
        sz = size;
    }

    template<class T>
    void Vector<T>::reserve(int n)
    {
        if(n > cp)
        {
            cp = n;
            T* newp = new T[cp];
            for(int i = 0; i < sz; i++) newp[i] = p[i];
            delete[] p;
            p = newp;
        }
    }

    template<class T>
    void Vector<T>::assign(int n, const T& u)
    {
        if(n > cp)
        {
            cp = n;
            delete[] p;
            p = new T[cp];
        }
        for(int i = 0; i < n; i++) p[i] = u;
        sz = n;
    }

    template<class T>
    void Vector<T>::push_back(const T& u)
    {
        if(sz == cp) reserve(ReallocFactor * sz);
        p[sz++] = u;
    }

    template<class T>
    void Vector<T>::pop_back()
    {
        if(sz < 1) throw SizeError();
        p[--sz].~T();
    }

    template<class T>
    typename Vector<T>::iterator Vector<T>::insert(iterator pos, const T& x)
    {
        iterator copy = pos;
        if(sz==cp) {
            int diff = pos - p;
            reserve(sz+1);
            pos = p + diff;
        }
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

    template<class T>
    void Vector<T>::insert(iterator pos, int n, const T& x)
    {
        if(sz + n > cp) {
            int diff = pos - p;
            reserve(sz + n);
            pos = p + diff;
        }
        sz += n;
        for(int i = sz-1; i >= sz-n-1; i--)
            p[i] = p[i-n];
        for(int i = 0; i < n; i++)
            *(pos++) = x;
    }

    template<class T>
    template<class InputIterator>
    void Vector<T>::insert(iterator pos, InputIterator first, InputIterator last)
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

    template<class T>
    typename Vector<T>::iterator Vector<T>::erase(iterator pos)
    {
        while(pos != end())
        {
            *pos = *(pos+1);
            pos++;
        }
        sz--;
        return pos;
    }

    template<class T>
    typename Vector<T>::iterator Vector<T>::erase(iterator first, iterator last)
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

    template<class T>
    void Vector<T>::swap(Vector<T>& x)
    {
        std::swap(cp, x.cp);
        std::swap(sz, x.sz);
        std::swap(p, x.p);
    }

    template<class T>
    void Vector<T>::clear()
    {
        for(int i = 0; i < sz; i++) p[i].~T();
        sz = 0;
    }
}
