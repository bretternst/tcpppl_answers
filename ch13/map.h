#include <string>

namespace ch13
{
    template<class T> class DefaultAllocator
    {
    public:
        static T allocate() { return T(); }
    };

    template<class Tkey,class Tval,class Talloc = DefaultAllocator<Tval> > class Map
    {
    public:
        struct Pair;
        
    private:
        struct Node
        {
            Node* parent;
            Node* left;
            Node* right;
            bool red;

            Pair pair;

            Node(Node* parent, Pair& pair) : parent(parent),pair(pair),left(0),right(0),red(0) {}
            ~Node() { if(left) delete left; if(right) delete right; }

            Node* grandparent()
            {
                return parent ? parent->parent : 0;
            }

            Node* uncle()
            {
                Node* g = grandparent();
                if (!g) return 0;
                return parent == g->left ? g->right : g->left;
            }

            void fix();
            void case2();
            void case3();
            void case4();
            void case5();

            void rotate_left();
            void rotate_right();

            int count() const;
            int depth() const;
        };

        Node* root;

        // prevent copying
        Map(const Map&) : root(0) {}
        void operator=(const Map&) {}

        Node* find(Tkey& key, Node* n) const;
        Node* find_or_insert(const Tkey& key, Node* n);

    public:
        struct Pair
        {
            Tkey key;
            Tval val;
            Pair(const Tkey& key, const Tval& val) : key(key), val(val) {}
        };

        Map() : root(0) {}
        ~Map() { if(root) delete root; }

        int count() const;
        int depth() const;

        Tval& operator[](const Tkey& key);

        class Iterator
        {
            friend class Map<Tkey,Tval,Talloc>;

            Node* cur;
            Map& map;
            Iterator(Map& m, Node* c) : map(m),cur(c) {}
        public:
            Iterator& operator++(int);
            Iterator& operator--(int);
            Pair& operator*();
            Pair* operator->();
            bool operator==(const Iterator& x);
            bool operator!=(const Iterator& y);
        };

        Iterator begin();
        Iterator end();
    };

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Iterator Map<Tkey,Tval,Talloc>::begin()
    {
        Node* n = root;
        while(n->left) n = n->left;
        return Iterator(*this, n);
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Iterator Map<Tkey,Tval,Talloc>::end()
    {
        return Iterator(*this, 0);
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Iterator& Map<Tkey,Tval,Talloc>::Iterator::operator++(int)
    {
        if(!cur)
            throw std::exception();

        if(cur->right)
        {
            cur = cur->right;
            while(cur->left) cur = cur->left;
        }
        else if (cur == cur->parent->left)
            cur = cur->parent;
        else
        {
            while(cur->parent && cur == cur->parent->right)
                cur = cur->parent;

            cur = cur->parent;
        }
        return *this;
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Iterator& Map<Tkey,Tval,Talloc>::Iterator::operator--(int)
    {
        if(!cur)
        {
            cur = map.root;
            if(!cur) throw std::exception();
            while(cur->right) cur = cur->right;
        }
        else
        {
            if(cur->left)
            {
                cur = cur->left;
                while(cur->right) cur = cur->right;
            }
            else if (cur == cur->parent->right)
                cur = cur->parent;
            else
            {
                while(cur->parent && cur == cur->parent->left)
                    cur = cur->parent;
                cur = cur->parent;
            }
        }
        return *this;
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Pair& Map<Tkey,Tval,Talloc>::Iterator::operator*()
    {
        if(!cur) throw std::exception();
        return cur->pair;
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Pair* Map<Tkey,Tval,Talloc>::Iterator::operator->()
    {
        if(!cur) throw std::exception();
        return &cur->pair;
    }

    template<class Tkey, class Tval, class Talloc>
    bool Map<Tkey,Tval,Talloc>::Iterator::operator==(const typename Map<Tkey,Tval,Talloc>::Iterator& x)
    {
        return x.cur == cur;
    }

    template<class Tkey, class Tval, class Talloc>
    bool Map<Tkey,Tval,Talloc>::Iterator::operator!=(const typename Map<Tkey,Tval,Talloc>::Iterator& x)
    {
        return x.cur != cur;
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::fix()
    {
        if(!parent)
            red = false;
        else
            case2();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::case2()
    {
        if (!parent->red)
            return;
        else
            case3();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::case3()
    {
        Node* u = uncle();

        if(u && u->red)
        {
            parent->red = false;
            u->red = false;
            Node* g = grandparent();
            g->red = true;
            g->fix();
        }
        else
        {
            case4();
        }
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::case4()
    {
        Node* g = grandparent();

        if(this == parent->right && parent == g->left)
        {
            parent->rotate_left();
            left->case5();
            return;
        }
        else if (this == parent->left && parent == g->right)
        {
            parent->rotate_right();
            right->case5();
            return;
        }
        case5();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::case5()
    {
        Node* g = grandparent();
        parent->red = false;
        g->red = true;
        if(this == parent->left && parent == g->left)
            g->rotate_right();
        else
            g->rotate_left();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::rotate_left()
    {
        Node* n = right;
        if(parent)
        {
            if(parent->left == this) parent->left = n;
            else if(parent->right == this) parent->right = n;
        }
        n->parent = parent;
        parent = n;
        right = n->left;
        if(right) right->parent = this;
        n->left = this;
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::rotate_right()
    {

        Node* n = left;
        if(parent)
        {
            if(parent->left == this) parent->left = n;
            if(parent->right == this) parent->right = n;
        }
        n->parent = parent;
        parent = n;
        left = n->right;
        if(left) left->parent = this;
        n->right = this;
    }

    template<class Tkey, class Tval, class Talloc>
    int Map<Tkey,Tval,Talloc>::Node::count() const
    {
        return 1 + (left ? left->count() : 0) + (right ? right->count() : 0);
    }

    template<class Tkey, class Tval, class Talloc>
    int Map<Tkey,Tval,Talloc>::count() const
    {
        return root ? root->count() : 0;
    }

    template<class Tkey, class Tval, class Talloc>
    int Map<Tkey,Tval,Talloc>::Node::depth() const
    {
        int l = left ? left->depth() : 0;
        int r = right ? right->depth() : 0;
        return 1 + (l > r ? l : r);
    }

    template<class Tkey, class Tval, class Talloc>
    int Map<Tkey,Tval,Talloc>::depth() const
    {
        return root ? root->depth() : 0;
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Node* Map<Tkey,Tval,Talloc>::find(Tkey& key, Node* n) const
    {
        if (!n) return 0;
        else if (n->pair.key == key) return n;
        else if (key < n->pair.key) return find(key, n->left);
        return find(key, n->right);
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Node* Map<Tkey,Tval,Talloc>::find_or_insert(const Tkey& key, Node* n)
    {
        Node* newNode;

        if(n == root && !n)
        {
            // Tree is empty
            Pair p(key,Talloc::allocate());
            return root = new Node(0, p);
        }
        else if (key == n->pair.key)
        {
            // Existing node found
            return n;
        }
        else if (key < n->pair.key)
        {
            if(n->left) return find_or_insert(key, n->left);
            Pair p(key,Talloc::allocate());
            newNode = n->left = new Node(n, p);
        }
        else
        {
            if(n->right) return find_or_insert(key, n->right);
            Pair p(key,Talloc::allocate());
            newNode = n->right = new Node(n, p);
        }

        newNode->red = true;
        newNode->fix();

        while(root->parent)
        {
            root = root->parent;
        }

        return newNode;
    }

    template<class Tkey,class Tval,class Talloc>
    Tval& Map<Tkey,Tval,Talloc>::operator[](const Tkey& key)
    {
        Node* n = find_or_insert(key, root);
        return n->pair.val;
    }

    template<class Tval> class Map<char*,Tval> : public Map<std::string,Tval>
    {
    };
}
