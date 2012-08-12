#include <utility>
#include <functional>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

// There are a couple of approaches that would have worked here; a more efficient one may have
// been to restructure the hash map so that it doesn't use the "v" vector for storing actual
// entries. Then, entries could have simply been removed by deleting them from their bucket
// in the hash table. However, this would have made iterators significantly more complicated
// because there wouldn't be one nicely packaged collection to proxy iterators to. However, it
// would have made erase more efficient.
//
// This approach simply removes the entry from its hash table bucket and then also removes it
// from the v collection. In the process, all pointers to entries after it need to be fixed up
// since their location in memory would have changed. I take advantage of the fact that vector
// storage is contiguous. Unfortunately, this also means that erase operations are O(n).
namespace ch17
{
    using std::allocator;
    using std::pair;

    template<class T>
    class Hash
    {
    public:
        size_t operator()(const T& key) const;
    };

    template<class T>
    size_t Hash<T>::operator()(const T& key) const
    {
        size_t res = 0;
        size_t len = sizeof(T);
        const char* p = reinterpret_cast<const char*>(&key);
        while(len--) res = (res<<1) ^ *p++;
        return res;
    }

    // integer hash algorithm taken from http://burtleburtle.net/bob/hash/integer.html
    template<>
    size_t Hash<int>::operator()(const int& key) const
    {
        int a = key;
        a = (a ^ 61) ^ (a >> 16);
        a = a + (a << 3);
        a = a ^ (a >> 4);
        a = a * 0x27d4eb2d;
        a = a ^ (a >> 15);
        return a;
    }

    typedef char* Pchar;
    template<>
    size_t Hash<char*>::operator()(const Pchar& key) const
    {
        size_t res = 0;
        Pchar p = key;
        while(*p) res = (res<<1)^*p++;
        return res;
    }

    template<>
    size_t Hash<std::string>::operator()(const std::string& key) const
    {
        size_t res = 0;
        typedef std::string::const_iterator CI;
        CI p = key.begin();
        CI end = key.end();
        while(p!=end) res = (res<<1) ^ *p++;
        return res;
    }

    template<class Key, class T, class H = Hash<Key>,
        class EQ = std::equal_to<Key>, class A = allocator<pair<const Key, T> > >
    class hash_map
    {
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef std::pair<const Key, T> value_type;
        typedef A allocator_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;

        typedef typename A::size_type size_type;
        typedef typename A::difference_type difference_type;

        typedef H Hasher;
        typedef EQ key_equal;

        class value_equal : public std::binary_function<value_type,value_type,bool>
        {
            friend class hash_map;
        protected:
            EQ eq;
            value_equal(EQ e) : eq(e) {}
        public:
            bool operator()(const value_type& x, const value_type& y) const
                { return eq(x.first,y.first); }
        };

    private:
        // representation
        struct Entry
        {
            value_type p;
            Entry* next;
            int pos;
            Entry(key_type k, mapped_type v, Entry* n, int ppos) : p(k,v), next(n), pos(ppos) {}
            Entry& operator=(const Entry& x)
            {
                next = x.next; pos = x.pos; p.second = x.p.second;
                *(const_cast<Key*>(&p.first)) = x.p.first; return *this;
            }
        };
        std::vector<Entry,typename A::template rebind<Entry>::other> v;
        std::vector<Entry*,typename A::template rebind<Entry*>::other> b;

        const T default_value;
        Hasher hash;
        key_equal eq;
        float max_load;
        float grow;

        void remove_node(Entry* e);
        void adjust_nodes(Entry* e, int pos, int num);
        void adjust_nodes(int pos, int num);

    public:
        template<class Iter, class U>
        class hash_iterator
        {
            Iter i;
            Iter end;
        public:
            hash_iterator() {}
            hash_iterator(Iter ii, Iter ee) : i(ii), end(ee) {}
            U& operator*() const { return i->p; }
            U* operator->() const { return &(i->p); }
            hash_iterator& operator++() { ++i; return *this; }
            hash_iterator operator++(int) { hash_iterator i = *this; ++*this; return i; }
            hash_iterator& operator--() { --i; return *this; }
            hash_iterator operator--(int) { hash_iterator i = *this; --*this; return i; }
            bool operator==(const hash_iterator& x) { return x.i==i; }
            bool operator!=(const hash_iterator& x) { return !(*this==x); }
            Entry& entry() { return *i; }
            Iter& inner() { return i; }
        };

        typedef hash_iterator<typename std::vector<Entry>::iterator, value_type> iterator;
        typedef hash_iterator<typename std::vector<Entry>::reverse_iterator, value_type> reverse_iterator;

        template<class Iter, class U>
        class const_hash_iterator
        {
            Iter i;
            Iter end;
        public:
            const_hash_iterator() {}
            const_hash_iterator(Iter ii, Iter ee) : i(ii), end(ee) {}
            const_hash_iterator(const iterator& ii) : i(ii.i), end(ii.end) {}
            const U& operator*() const { return i->p; }
            const U* const operator->() const { return &(i->p); }
            const_hash_iterator& operator++() { ++i; return *this; }
            const_hash_iterator operator++(int) { hash_iterator<Iter,U> i = *this; ++*this; return i; }
            const_hash_iterator& operator--() { --i; return *this; }
            const_hash_iterator operator--(int) { hash_iterator<Iter,U> i = *this; --*this; return i; }
            bool operator==(const const_hash_iterator& x) { return x.i==i; }
            bool operator!=(const const_hash_iterator& x) { return !(*this==x); }
            Entry& entry() { return *i; }
        };

        typedef const_hash_iterator<typename std::vector<Entry>::const_iterator, value_type> const_iterator;
        typedef const_hash_iterator<typename std::vector<Entry>::const_reverse_iterator, value_type> const_reverse_iterator;

        hash_map(const T& dv = T(), size_type n = 101, const H& h = H(), const EQ& e = EQ()) :
          default_value(dv), b(n), hash(h), eq(e)
        {
            set_load();
            v.reserve(static_cast<unsigned int>(max_load*b.size()));
        }

        void set_load(float m = 0.7, float g = 1.6)
        {
            max_load = m;
            grow = g;
        }

        template<class In> hash_map(In first, In last,
            const T& dv = T(), size_type n = 101, const H& hf = H(), const EQ& eq = EQ());
        ~hash_map() {}

        iterator begin() { typename std::vector<Entry>::iterator i = v.begin(); return iterator(i,v.end()); }
        const_iterator begin() const { typename std::vector<Entry>::const_iterator i = v.begin(); return const_iterator(i,v.end()); }
        iterator end() { return iterator(v.end(),v.end()); }
        const_iterator end() const { return const_iterator(v.end(),v.end()); }
        reverse_iterator rbegin() { typename std::vector<Entry>::reverse_iterator i = v.rbegin(); return reverse_iterator(i,v.rend()); };
        const_reverse_iterator rbegin() const { typename std::vector<Entry>::reverse_iterator i = v.rbegin(); return reverse_iterator(i,v.rend()); };
        reverse_iterator rend() { return reverse_iterator(v.rend(),v.rend()); };
        const_reverse_iterator rend() const { return const_reverse_iterator(v.rend(),v.rend()); };

        // map functions
        iterator find(const key_type& k);
        const_iterator find(const key_type& k) const;
        size_type count(const key_type& k) const { return find(k) == end() ? 0 : 1; };
        iterator lower_bound(const key_type& k) { return find(k); }
        const_iterator lower_bound(const key_type& k) const { return find(k); }
        iterator upper_bound(const key_type& k) { iterator i = find(k); return i!=end() ? ++i : i; }
        const_iterator upper_bound(const key_type& k) const { const_iterator i = find(k); return i!=end() ? ++i : i; }
        std::pair<iterator,iterator> equal_range(const key_type& k) { iterator i,j; i=j=find(k); return std::make_pair(i,j!=end()?++j:j); }
        std::pair<const_iterator,const_iterator> equal_range(const key_type& k) const { const_iterator i,j; i=j=find(k); return std::make_pair(i,j!=end()?++j:j); }
        mapped_type& operator[](const key_type& k);

        // list functions
        std::pair<iterator, bool> insert(const value_type& val);
        iterator insert(iterator pos, const value_type& val) { return insert(val).first; }
        template<class In> void insert(In first, In last) { while(first != last) { insert(*first++); } }
        void erase(iterator pos) {
            remove_node(&pos.entry());
            adjust_nodes(pos.entry().pos, 1);
            v.erase(pos.inner());
        }
        size_type erase(const key_type& k) { iterator i=find(k); if(i!=end()) { erase(i); return 1; } return 0; }
        void erase(iterator first, iterator last) {
            size_type num = last.inner() - first.inner();
            for(iterator i = first; i != last; i++)
                remove_node(&i.entry());
            adjust_nodes(last.entry().pos, num);
            v.erase(first.inner(), last.inner());
        }
        void clear() { std::fill(b.begin(),b.end(),static_cast<Entry*>(0)); v.clear(); }

        // other functions
        size_type size() const { return v.size(); };
        size_type max_size() const { v.max_size(); };
        size_type bucket_count() const { return b.size(); }
        void resize(size_type n);
        Hasher hash_fun() const { return hash; }
        key_equal key_eq() const { return eq; }
        bool empty() const { return size()==0; }
        void swap(hash_map& x) { b.swap(x.b); v.swap(x.v); }
        hash_map& operator=(const hash_map&);

        // added to test distribution
        float bucket_utilization() const { int count=0;for(size_type i=0;i<b.size();i++)if(b[i])count++; return (float)count/(float)b.size(); }
    };

    template<class Key, class T, class H, class EQ, class A>
    typename hash_map<Key,T,H,EQ,A>::iterator hash_map<Key,T,H,EQ,A>::find(const key_type& k)
    {
        size_type i = hash(k)%b.size();
        for(Entry* p = b[i]; p; p = p->next)
        {
            if(eq(k,p->p.first))
            {
                return iterator(v.begin()+p->pos, v.end());
            }
        }
        return iterator(v.end(),v.end());
    }

    template<class Key, class T, class H, class EQ, class A>
    typename hash_map<Key,T,H,EQ,A>::const_iterator hash_map<Key,T,H,EQ,A>::find(const key_type& k) const
    {
        size_type i = hash(k)%b.size();
        for(Entry* p = b[i]; p; p = p->next)
        {
            if(eq(k,p->p.first))
            {
                return const_iterator(v.begin()+p->pos, v.end());
            }
        }
        return const_iterator(v.end(),v.end());
    }

    template<class Key, class T, class H, class EQ, class A>
    void hash_map<Key,T,H,EQ,A>::resize(size_type s)
    {
        size_type i = v.size();
        if(s <= b.size()) return;

        b.resize(s);
        std::fill(b.begin(),b.end(),static_cast<Entry*>(0));
        v.reserve(static_cast<unsigned int>(s*max_load));
        for(size_type i = 0; i<v.size(); i++)
        {
            size_type ii = hash(v[i].p.first)%b.size();
            v[i].next = b[ii];
            v[i].pos = i;
            b[ii] = &v[i];
        }
    }

    template<class Key, class T, class H, class EQ, class A>
    typename hash_map<Key,T,H,EQ,A>::mapped_type& 
    hash_map<Key,T,H,EQ,A>::operator[](const key_type& k)
    {
        size_type i = hash(k)%b.size();
        for(Entry* p = b[i]; p; p = p->next)
        {
            if(eq(k,p->p.first))
            {
                return p->p.second;
            }
        }
        if(size_type(b.size()*max_load) <= v.size())
        {
            resize(static_cast<unsigned int>(b.size()*grow));
            return operator[](k);
        }
        v.push_back(Entry(k,default_value,b[i],v.size()));
        b[i] = &v.back();
        return b[i]->p.second;
    }

    template<class Key, class T, class H, class EQ, class A>
    std::pair<typename hash_map<Key,T,H,EQ,A>::iterator, bool> hash_map<Key,T,H,EQ,A>::insert(const value_type& val)
    {
        iterator i = find(val.first);
        if(i!=end()) return std::make_pair(i, false);
        operator[](val.first) = val.second;
        return std::make_pair(find(val.first), true);
    }

    template<class Key, class T, class H, class EQ, class A>
    void hash_map<Key,T,H,EQ,A>::remove_node(Entry* e) {
        size_type i = hash(e->p.first)%b.size();
        Entry* p = b[i];
        if(p == e)
            b[i] = p->next;
        else {
            Entry* q = p;
            while(p != e) {
                q = p;
                p = p->next;
            }
            q->next = p->next;
        }
    }

    template<class Key, class T, class H, class EQ, class A>
    void hash_map<Key,T,H,EQ,A>::adjust_nodes(Entry* e, int pos, int num) {
        if(e->next) {
            adjust_nodes(e->next, pos, num);
            if(e->next->pos > pos) {
                e->next->pos -= num;
                e->next -= num;
            }
        }
    }

    template<class Key, class T, class H, class EQ, class A>
    void hash_map<Key,T,H,EQ,A>::adjust_nodes(int pos, int num) {
        for(size_type i = 0; i < b.size(); i++) {
            if(b[i]) {
                adjust_nodes(b[i], pos, num);
                if(b[i]->pos > pos) {
                    b[i]->pos -= num;
                    b[i] -= num;
                }
            }
        }
    }
}

int main() {
    using namespace ch17;
    using namespace std;

    typedef hash_map<int,int> IntInt;
    IntInt hm;
    for(int i = 0; i < 10; i++)
        hm[i] = i;

    hm.erase(5);
    hm.erase(hm.find(7), hm.find(9));

    for(IntInt::iterator i = hm.begin(); i != hm.end(); i++) {
        cout << i->first << " = " << i->second << endl;
    }
}