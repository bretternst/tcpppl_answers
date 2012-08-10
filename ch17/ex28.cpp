#include <vector>
#include <cstdlib>
#include <iostream>

// Again, we'll keep this exercise very simple and only support operator[].
// The rest of the standard container functions have already been covered
// thoroughly, I don't feel the need to implement them yet again. Also, no
// compare or allocator template parameters.
//
// One disadvantage of using a splay tree is that lookups modify the underlying
// store, so const-ness becomes a bit of a problem; also, multithreaded reads
// would blow up.
namespace ch17 {
    // This implementation is adapted from http://en.wikipedia.org/wiki/Splay_tree
    template<class K, class V>
    struct splay_tree {
        struct Node {
            const K key;
            V value;
            Node* parent;
            Node* left;
            Node* right;

            Node(const K& key, V& value) : key(key), value(value) { }
        };

        Node* root;

        void splay(Node* x);
        Node* rotate_right(Node* p);
        Node* rotate_left(Node* p);
        Node* insert(const K& key, V& value);
        Node* lookup(const K& key);
        Node* search(const K& key);

        V& operator[](const K& key);
        const V& operator[](const K& key) const;
    };

    template<class K, class V>
    void splay_tree<K,V>::splay(Node* x) {
        if(x == root)
            return;
        else if (x->parent == root) {
            if (x==x->parent->left)
                root = rotate_right(root);
            else
                root = rotate_left(root);
        }
        else {
            Node *p, *g;
            p = x->parent;
            g = p->parent;
            if(x == p->left && p == g->left) {
                root = rotate_right(g);
                root = rotate_right(p);
            }
            else if (x == p->right && p == g->right) {
                root = rotate_left(g);
                root = rotate_left(p);
            }
            else if (x == p->right && p == g->left) {
                root = rotate_left(p);
                root = rotate_right(g);
            }
            else if (x == p->left && p == g->right) {
                root = rotate_right(p);
                root = rotate_left(g);
            }
            splay(x);
        }
    }

    template<class K, class V>
    typename splay_tree<K,V>::Node* splay_tree<K,V>::rotate_right(Node* p) {
        Node* x = p->left;
        p->left = x->right;
        if(x->right)
            x->right->parent = p;
        x->right = p;
        if(p->parent) {
            if(p == p->parent->right)
                p->parent->right = x;
            else
                p->parent->left = x;
        }
        x->parent = p->parent;
        p->parent = x;
        if(p == root)
            return x;
        else
            return root;
    }

    template<class K, class V>
    typename splay_tree<K,V>::Node* splay_tree<K,V>::rotate_left(Node* p) {
        Node* x = p->right;
        p->right = x->left;
        if(x->left)
            x->left->parent = p;
        x->left = p;
        if(p->parent) {
            if(p == p->parent->left)
                p->parent->left = x;
            else
                p->parent->right = x;
        }
        x->parent = p->parent;
        p->parent = x;
        if(p == root)
            return x;
        else
            return root;
    }

    template<class K, class V>
    typename splay_tree<K,V>::Node* splay_tree<K,V>::insert(const K& key, V& value) {
        Node *tmp1, *tmp2, *par, *x;
        if(!root) {
            root = new Node(key, value);
            return root;
        }
        else {
            tmp2 = root;
            while(tmp2) {
                tmp1 = tmp2;
                if(tmp2->key > key)
                    tmp2 = tmp2->left;
                else if (tmp2->key < value)
                    tmp2 = tmp2->right;
                else if(tmp2->key == key)
                    return tmp2;
            }
            if(tmp1->key > value) {
                par = tmp1;
                tmp1->left = new Node(key, value);
                tmp1 = tmp1->left;
                tmp1->parent = par;
            }
            else {
                par = tmp1;
                tmp1->right = new Node(key, value);
                tmp1 = tmp1->right;
                tmp1->parent = par;
            }
        }
        splay(tmp1);
    }

    template<class K, class V>
    typename splay_tree<K,V>::Node* splay_tree<K,V>::lookup(const K& key) {
        Node *tmp1, *tmp2;
        if(root) {
            tmp1 = root;
            while(tmp1) {
                tmp2 = tmp1;
                if(tmp1->key > key)
                    tmp1 = tmp1->left;
                else if (tmp1->key < key)
                    tmp1 = tmp1->right;
                else
                    return tmp1;
            }
            return tmp2;
        }
        return 0;
    }

    template<class K, class V>
    typename splay_tree<K,V>::Node* splay_tree<K,V>::search(const K& key) {
        Node *x = lookup(key);
        if(x) {
            splay(x);
            if (x->key == key)
                return x;
        }
        return 0;
    }

    template<class K, class V>
    V& splay_tree<K,V>::operator[](const K& key) {
        Node* x = search(key);
        if(!x) {
            V def = V();
            return insert(key, def)->value;
        }
        else
            return x->value;
    }


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

    template<class K, class V, class H = Hash<K> >
    class hash_map
    {
    private:
        std::vector<splay_tree<K,V>*> tab;
        H hash;

    public:
        hash_map(int n = 101, const H& h = H()) :
          tab(n), hash(h)
        {
        }

        V& operator[](const K& k);
    };

    template<class K, class V, class H> 
    V& hash_map<K,V,H>::operator[](const K& k)
    {
        int i = hash(k)%tab.size();
        splay_tree<K,V>* m = tab[i];
        if(m == 0)
            m = tab[i] = new splay_tree<K,V>();
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
    hm[1] = 5;
    cout << hm[0] << " " << hm[1] << " " << hm[2] << endl;
}
