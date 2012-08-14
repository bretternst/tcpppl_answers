#ifndef __HASH_SET_H__
#define __HASH_SET_H__

#include <utility>
#include <functional>
#include <vector>
#include <string>
#include <cstdlib>
#include "hash_map.h"

namespace ch17 {
    using std::allocator;
    using std::pair;

    template<class Key, class H = Hash<Key>,
        class EQ = std::equal_to<Key>, class A = allocator<Key> >
    class hash_set
    {
    public:
        typedef Key key_type;
        typedef Key value_type;
        typedef A allocator_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;
        typedef typename A::size_type size_type;
        typedef typename A::difference_type difference_type;
        typedef typename A::pointer pointer;
        typedef typename A::const_pointer const_pointer;
        typedef EQ key_equal;
        typedef H key_hash;

    private:
        // representation
        struct Entry
        {
            value_type k;
            bool erased;
            Entry* next;
            int pos;
            Entry(key_type k, Entry* n, int ppos) : k(k), next(n), pos(ppos), erased(false) {}
            Entry& operator=(const Entry& x)
            {
                erased = x.erased; next = x.next; pos = x.pos; k = x.k;
            }
        };
        std::vector<Entry,typename A::template rebind<Entry>::other> v;
        std::vector<Entry*,typename A::template rebind<Entry*>::other> b;

        key_hash hash;
        key_equal eq;
        float max_load;
        float grow;
        size_type no_of_erased;

        typedef typename std::vector<Entry>::iterator v_iterator;
        typedef typename std::vector<Entry>::const_iterator v_const_iterator;

    public:
        class hash_iterator
        {
            v_iterator i;
            v_iterator end;
        public:
            typedef hash_set::value_type value_type;
            typedef hash_set::difference_type difference_type;
            typedef hash_set::pointer pointer;
            typedef hash_set::reference reference;
            typedef std::bidirectional_iterator_tag iterator_category;

            hash_iterator() {}
            hash_iterator(v_iterator i, v_iterator e) : i(i), end(e) {}
            reference operator*() const { return i->k; }
            pointer operator->() const { return &(i->k); }
            hash_iterator& operator++() { ++i; while(i != end && i->erased) ++i; return *this; }
            hash_iterator operator++(int) { hash_iterator i = *this; ++*this; return i; }
            hash_iterator& operator--() { --i; while(i->erased) --i; return *this; }
            hash_iterator operator--(int) { hash_iterator i = *this; --*this; return i; }
            bool operator==(const hash_iterator& x) { return x.i==i; }
            bool operator!=(const hash_iterator& x) { return !(*this==x); }
            Entry& entry() { return *i; }
        };

        typedef hash_iterator iterator;
        typedef std::reverse_iterator<hash_iterator> reverse_iterator;

        class const_hash_iterator
        {
            v_const_iterator i;
            v_const_iterator end;
        public:
            typedef hash_set::value_type value_type;
            typedef hash_set::difference_type difference_type;
            typedef hash_set::pointer pointer;
            typedef hash_set::reference reference;
            typedef std::bidirectional_iterator_tag iterator_category;

            const_hash_iterator() {}
            const_hash_iterator(v_const_iterator i, v_const_iterator e) : i(i), end(e) {}
            const_hash_iterator(const iterator& i) : i(i.i), end(i.end) {}
            const_reference operator*() const { return i->k; }
            const_pointer const operator->() const { return &(i->k); }
            const_hash_iterator& operator++() { ++i; while(i != end && i->erased) ++i; return *this; }
            const_hash_iterator operator++(int) { const_hash_iterator i = *this; ++*this; return i; }
            const_hash_iterator& operator--() { --i; while(i->erased) --i; return *this; }
            const_hash_iterator operator--(int) { const_hash_iterator i = *this; --*this; return i; }
            bool operator==(const const_hash_iterator& x) { return x.i==i; }
            bool operator!=(const const_hash_iterator& x) { return !(*this==x); }
            Entry& entry() { return *i; }
        };

        typedef const_hash_iterator const_iterator;
        typedef std::reverse_iterator<const_hash_iterator> const_reverse_iterator;

        hash_set(size_type n = 101, const H& h = H(), const EQ& e = EQ()) :
          b(n), no_of_erased(0), hash(h), eq(e)
        {
            set_load();
            v.reserve(n);
        }

        void set_load(float m = 0.7, float g = 1.6)
        {
            max_load = m;
            grow = g;
        }

        template<class In> hash_set(In first, In last,
            size_type n = 101, const H& hf = H(), const EQ& eq = EQ());
        ~hash_set() {}

        iterator begin() {
            v_iterator i = v.begin();
            while(i!=v.end() && i->erased) ++i;
            return iterator(i,v.end());
        }

        const_iterator begin() const {
            v_const_iterator i = v.begin();
            while(i!=v.end() && i->erased) ++i;
            return const_iterator(i,v.end());
        }

        iterator end() {
            return iterator(v.end(),v.end());
        }

        const_iterator end() const {
            return const_iterator(v.end(),v.end());
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        };

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        };

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

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

        // list functions
        std::pair<iterator, bool> insert(const value_type& val);
        iterator insert(iterator pos, const value_type& val) { return insert(val).first; }
        template<class In> void insert(In first, In last) { while(first != last) { insert(*first++); } }
        void erase(iterator pos) { if(pos.entry().erased==false) no_of_erased++; pos.entry().erased = true; }
        size_type erase(const key_type& k) { iterator i=find(k); if(i!=end()) { erase(i); return 1; } return 0; }
        void erase(iterator first, iterator last) { while(first!=last) erase(first++); }
        void clear() { std::fill(b.begin(),b.end(),static_cast<Entry*>(0)); v.clear(); no_of_erased = 0; }

        // other functions
        size_type size() const { return v.size()-no_of_erased; };
        size_type max_size() const { v.max_size(); };
        size_type bucket_count() const { return b.size(); }
        void resize(size_type n);
        key_hash hash_fun() const { return hash; }
        key_equal key_eq() const { return eq; }
        bool empty() const { return size()==0; }
        void swap(hash_set& x) { b.swap(x.b); v.swap(x.v); std::swap(no_of_erased,x.no_of_erased); }
        hash_set& operator=(const hash_set&);

        // added to test distribution
        float bucket_utilization() const { int count=0;for(size_type i=0;i<b.size();i++)if(b[i])count++; return (float)count/(float)b.size(); }
    };

    template<class Key, class H, class EQ, class A>
    typename hash_set<Key,H,EQ,A>::iterator hash_set<Key,H,EQ,A>::find(const key_type& k)
    {
        size_type i = hash(k)%b.size();
        for(Entry* p = b[i]; p; p = p->next)
        {
            if(eq(k,p->k) && !p->erased)
            {
                return iterator(v.begin()+p->pos, v.end());
            }
        }
        return iterator(v.end(),v.end());
    }

    template<class Key, class H, class EQ, class A>
    typename hash_set<Key,H,EQ,A>::const_iterator hash_set<Key,H,EQ,A>::find(const key_type& k) const
    {
        size_type i = hash(k)%b.size();
        for(Entry* p = b[i]; p; p = p->next)
        {
            if(eq(k,p->k) && !p->erased)
            {
                return const_iterator(v.begin()+p->pos, v.end());
            }
        }
        return const_iterator(v.end(),v.end());
    }

    template<class Key, class H, class EQ, class A>
    void hash_set<Key,H,EQ,A>::resize(size_type s)
    {
        size_type i = v.size();
        if(s <= b.size()) return;
        while(no_of_erased)
        {
            if(v[--i].erased)
            {
                v.erase(v.begin()+i);
                --no_of_erased;
            }
        }

        b.resize(s);
        std::fill(b.begin(),b.end(),static_cast<Entry*>(0));
        v.reserve(static_cast<unsigned int>(s*max_load));
        for(size_type i = 0; i<v.size(); i++)
        {
            size_type ii = hash(v[i].k)%b.size();
            v[i].next = b[ii];
            v[i].pos = i;
            b[ii] = &v[i];
        }
    }

    template<class Key, class H, class EQ, class A>
    std::pair<typename hash_set<Key,H,EQ,A>::iterator, bool> hash_set<Key,H,EQ,A>::insert(const value_type& val)
    {
        iterator iter = find(val);
        if(iter!=end()) return std::make_pair(iter, false);

        size_type i = hash(val)%b.size();
        if(b[i]) {
            Entry* q;
            for(Entry* p = b[i]; p; p = p->next)
            {
                q = p;
                if(p->erased) {
                    p->erased = false;
                    no_of_erased--;
                    return std::make_pair(find(val), true);
                }
            }
            if(size_type(b.size()*max_load) <= v.size()) {
                resize(static_cast<unsigned int>(b.size()*grow));
                return insert(val);
            }
            v.push_back(Entry(val,0,v.size()));
            q->next = &v.back();
            return std::make_pair(find(val), true);
        } else {
            if(size_type(b.size()*max_load) <= v.size())
            {
                resize(static_cast<unsigned int>(b.size()*grow));
                return insert(val);
            }
            v.push_back(Entry(val,b[i],v.size()));
            b[i] = &v.back();
            return std::make_pair(find(val), true);
        }
    }
}

#endif