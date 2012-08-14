#ifndef __HASH_MULTISET_H__
#define __HASH_MULTISET_H__

#include <utility>
#include <functional>
#include <vector>
#include <string>
#include <cstdlib>
#include "hash_map.h"

// To simplify this a bit, I left out the const iterators (so this class can
// only reasonably be used non-const). A real implementation would obviously
// need to be more complete.
namespace ch17 {
    using std::allocator;
    using std::pair;

    template<class Key, class H = Hash<Key>,
        class EQ = std::equal_to<Key>, class A = allocator<Key> >
    class hash_multiset
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
        typedef std::vector<value_type, A> value_bag;

        // representation
        struct Entry
        {
            key_type k;
            value_bag bag;
            Entry* next;
            int pos;
            Entry(key_type k, Entry* n, int ppos) : k(k), next(n), pos(ppos) {}
            Entry& operator=(const Entry& x)
            {
                next = x.next; pos = x.pos; k = x.k;
                bag = value_bag(x.bag);
            }
        };
        std::vector<Entry,typename A::template rebind<Entry>::other> v;
        std::vector<Entry*,typename A::template rebind<Entry*>::other> b;

        key_hash hash;
        key_equal eq;
        float max_load;
        float grow;
        size_type sz;

        typedef typename std::vector<Entry>::iterator v_iterator;
        typedef typename std::vector<Entry>::const_iterator v_const_iterator;
        typedef typename value_bag::iterator b_iterator;

    public:
        class hash_iterator
        {
            friend class hash_multiset;

            hash_multiset& map;
            v_iterator i1;
            b_iterator i2;
        public:
            typedef hash_multiset::value_type value_type;
            typedef hash_multiset::difference_type difference_type;
            typedef hash_multiset::pointer pointer;
            typedef hash_multiset::reference reference;
            typedef std::bidirectional_iterator_tag iterator_category;

            hash_iterator() {}
            hash_iterator(hash_multiset& map, v_iterator i1, b_iterator i2) : map(map), i1(i1), i2(i2) {}
            reference operator*() const { return *i2; }
            pointer operator->() const { return &(*i2); }
            hash_iterator& operator++() {
                i2++;
                if(i2 == i1->bag.end()) {
                    do {
                        i1++;
                    } while (i1 != map.v.end() && i1->bag.size() == 0);
                    if(i1 != map.v.end())
                        i2 = i1->bag.begin();
                }
                return *this;
            }
            hash_iterator operator++(int) { hash_iterator i = *this; ++*this; return i; }
            hash_iterator& operator--() {
                if(i2 == i1->bag.begin()) {
                    do {
                        i1--;
                    } while (i1->bag.size() == 0);
                    i2 = i1->bag.end() - 1;
                } else
                    i2--;
                return *this;
            }
            hash_iterator operator--(int) { hash_iterator i = *this; --*this; return i; }
            bool operator==(const hash_iterator& x) {
                bool b = i1 == map.v.end() && x.i1 == map.v.end() || i2 == x.i2;
                return b;
            }
            bool operator!=(const hash_iterator& x) { return !(*this==x); }
            Entry& entry() { return *i1; }
        };

        typedef hash_iterator iterator;
        typedef std::reverse_iterator<hash_iterator> reverse_iterator;

        hash_multiset(size_type n = 101, const H& h = H(), const EQ& e = EQ()) :
            b(n), sz(0), hash(h), eq(e)
        {
            set_load();
            v.reserve(n);
        }

        void set_load(float m = 0.7, float g = 1.6)
        {
            max_load = m;
            grow = g;
        }

        template<class In> hash_multiset(In first, In last,
            size_type n = 101, const H& hf = H(), const EQ& eq = EQ());
        ~hash_multiset() {}

        iterator begin() {
            v_iterator i = v.begin();
            while(i!=v.end() && i->bag.size() == 0) ++i;
            return iterator(*this, i, i == v.end() ? v.back().bag.end() : i->bag.begin());
        }

        iterator end() {
            return iterator(*this, v.end(),v.back().bag.end());
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        };

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        // map functions
        iterator find(const key_type& k);
        size_type count(const key_type& k) {
            iterator i = find(k);
            if(i == end())
                return 0;
            else
                return i->bag.size();
        };
        iterator lower_bound(const key_type& k) {
            return find(k);
        }
        iterator upper_bound(const key_type& k) {
            iterator i = find(k);
            v_iterator i1 = i.i1;
            i1++;
            return iterator(*this, i1, i1 == v.end() ? v.back().bag.end() : i1->bag.begin());
        }
        std::pair<iterator,iterator> equal_range(const key_type& k) {
            iterator i = lower_bound(k);
            iterator j = upper_bound(k);
            return std::make_pair(i,j);
        }

        // list functions
        std::pair<iterator, bool> insert(const value_type& val);
        iterator insert(iterator pos, const value_type& val) { return insert(val).first; }
        template<class In> void insert(In first, In last) { while(first != last) { insert(*first++); } }
        void erase(iterator pos) {
            pos.entry().bag.erase(pos.i2);
            if(pos.entry().bag.size() == 0)
                sz--;
        }
        size_type erase(const key_type& k) {
            iterator i=find(k);
            if(i!=end()) {
                size_t num = i.i1->bag.size();
                i.i1->bag.clear();
                sz -= num;
                return num;
            }
            return 0;
        }
        void clear() { std::fill(b.begin(),b.end(),static_cast<Entry*>(0)); v.clear(); sz = 0; }

        // other functions
        size_type size() const { return sz; };
        size_type max_size() const { v.max_size(); };
        size_type bucket_count() const { return b.size(); }
        void resize(size_type n);
        key_hash hash_fun() const { return hash; }
        key_equal key_eq() const { return eq; }
        bool empty() const { return size()==0; }
        void swap(hash_multiset& x) { b.swap(x.b); v.swap(x.v); std::swap(sz,x.sz); }
        hash_multiset& operator=(const hash_multiset&);
    };

    template<class Key, class H, class EQ, class A>
    typename hash_multiset<Key,H,EQ,A>::iterator hash_multiset<Key,H,EQ,A>::find(const key_type& k)
    {
        size_type i = hash(k)%b.size();
        for(Entry* p = b[i]; p; p = p->next)
        {
            if(eq(k,p->k) && p->bag.size() > 0)
            {
                return iterator(*this, v.begin()+p->pos, p->bag.begin());
            }
        }
        return end();
    }

    template<class Key, class H, class EQ, class A>
    void hash_multiset<Key,H,EQ,A>::resize(size_type s)
    {
        size_type i = v.size();
        if(s <= b.size()) return;
        while(i >= 0) {
            if(v[--i].bag.size() == 0)
            {
                v.erase(v.begin()+i);
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
    std::pair<typename hash_multiset<Key,H,EQ,A>::iterator, bool> hash_multiset<Key,H,EQ,A>::insert(const value_type& val)
    {
        size_type i = hash(val)%b.size();
        if(b[i]) {
            Entry* q;
            for(Entry* p = b[i]; p; p = p->next)
            {
                q = p;
                if(eq(val,p->k))
                {
                    p->bag.push_back(val);
                    iterator iter = find(val);
                    return std::make_pair(iterator(*this, iter.i1, p->bag.end()-1),true);
                }
            }
            if(size_type(b.size()*max_load) <= v.size())
            {
                resize(static_cast<unsigned int>(b.size()*grow));
                return insert(val);
            }
            v.push_back(Entry(val,0,v.size()));
            q->next = &v.back();
            v.back().bag.push_back(val);
            iterator iter = find(val);
            return std::make_pair(iter, true);
        } else {
            if(size_type(b.size()*max_load) <= v.size())
            {
                resize(static_cast<unsigned int>(b.size()*grow));
                return insert(val);
            }
            v.push_back(Entry(val,0,v.size()));
            b[i] = &v.back();
            v.back().bag.push_back(val);
            iterator iter = find(val);
            return std::make_pair(iter, true);
        }
    }
}

#endif
