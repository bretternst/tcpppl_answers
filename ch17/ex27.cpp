#include <utility>
#include <functional>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <iostream>

// This exercise just illustrates the possibility of using a map for each hash_map bucket;
// as such, I only implemented the bare minimum (operator[]) and no resizing.
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

    template<class K, class V, class H = Hash<K>, class Cmp = std::less<K> >
    class hash_map
    {
    public:
        typedef K key_type;
        typedef V mapped_type;
        typedef std::pair<const K, V> value_type;
        typedef V& reference;
        typedef const V& const_reference;

        typedef H Hasher;
        typedef Cmp key_less;

    private:
        std::vector<std::map<K,V>*> tab;

        const V default_value;
        Hasher hash;

    public:
        hash_map(const V& dv = V(), int n = 101, const H& h = H()) :
          default_value(dv), tab(n), hash(h)
        {
        }

        template<class In> hash_map(In first, In last,
            const V& dv = V(), int n = 101, const H& hf = H());
        ~hash_map() {}

        mapped_type& operator[](const key_type& k);
    };

    template<class K, class V, class H, class Cmp>
    typename hash_map<K,V,H,Cmp>::mapped_type& 
    hash_map<K,V,H,Cmp>::operator[](const key_type& k)
    {
        int i = hash(k)%tab.size();
        std::map<K,V>* m = tab[i];
        if(m == 0)
            m = tab[i] = new std::map<K,V,Cmp>();
        return (*m)[k];
    }
}

int main() {
    using namespace std;
    using namespace ch17;

    hash_map<int,int> hm;
    hm[0] = 1;
    hm[1] = 2;
    hm[2] = 3;
    cout << hm[0] << " " << hm[1] << " " << hm[2] << endl;
}