#include <string>

namespace Exercises
{
    template<class T> class DefaultAllocator
    {
    public:
        static T Allocate() { return T(); }
    };

    template<class Tkey,class Tval,class Talloc = DefaultAllocator<Tval> > class Map
    {
        struct Pair;

        struct Node
        {
            Node* parent;
            Node* left;
            Node* right;
            bool red;

            Pair pair;

            Node(Node* parent, Pair& pair) : parent(parent),pair(pair),left(0),right(0),red(0) {}
            ~Node() { if(left) delete left; if(right) delete right; }

            Node* Grandparent()
            {
                return parent ? parent->parent : 0;
            }

            Node* Uncle()
            {
                Node* g = Grandparent();
                if (!g) return 0;
                return parent == g->left ? g->right : g->left;
            }

            void Fix();
            void Case2();
            void Case3();
            void Case4();
            void Case5();

            void RotateLeft();
            void RotateRight();

            int Count() const;
            int Depth() const;
        };

        Node* root;

        // prevent copying
        Map(const Map&) : root(0) {}
        void operator=(const Map&) {}

        Node* Find(Tkey& key, Node* n) const;
        Node* FindOrInsert(const Tkey& key, Node* n);

    public:
        struct Pair
        {
            Tkey key;
            Tval val;
            Pair(const Tkey& key, const Tval& val) : key(key), val(val) {}
        };

        Map() : root(0) {}
        ~Map() { if(root) delete root; }

        int Count() const;
        int Depth() const;

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

        Iterator Begin();
        Iterator End();
    };

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Iterator Map<Tkey,Tval,Talloc>::Begin()
    {
        Node* n = root;
        while(n->left) n = n->left;
        return Iterator(*this, n);
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Iterator Map<Tkey,Tval,Talloc>::End()
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
    void Map<Tkey,Tval,Talloc>::Node::Fix()
    {
        if(!parent)
            red = false;
        else
            Case2();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::Case2()
    {
        if (!parent->red)
            return;
        else
            Case3();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::Case3()
    {
        Node* u = Uncle();

        if(u && u->red)
        {
            parent->red = false;
            u->red = false;
            Node* g = Grandparent();
            g->red = true;
            g->Fix();
        }
        else
        {
            Case4();
        }
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::Case4()
    {
        Node* g = Grandparent();

        if(this == parent->right && parent == g->left)
        {
            parent->RotateLeft();
            left->Case5();
            return;
        }
        else if (this == parent->left && parent == g->right)
        {
            parent->RotateRight();
            right->Case5();
            return;
        }
        Case5();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::Case5()
    {
        Node* g = Grandparent();
        parent->red = false;
        g->red = true;
        if(this == parent->left && parent == g->left)
            g->RotateRight();
        else
            g->RotateLeft();
    }

    template<class Tkey, class Tval, class Talloc>
    void Map<Tkey,Tval,Talloc>::Node::RotateLeft()
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
    void Map<Tkey,Tval,Talloc>::Node::RotateRight()
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
    int Map<Tkey,Tval,Talloc>::Node::Count() const
    {
        return 1 + (left ? left->Count() : 0) + (right ? right->Count() : 0);
    }

    template<class Tkey, class Tval, class Talloc>
    int Map<Tkey,Tval,Talloc>::Count() const
    {
        return root ? root->Count() : 0;
    }

    template<class Tkey, class Tval, class Talloc>
    int Map<Tkey,Tval,Talloc>::Node::Depth() const
    {
        int l = left ? left->Depth() : 0;
        int r = right ? right->Depth() : 0;
        return 1 + (l > r ? l : r);
    }

    template<class Tkey, class Tval, class Talloc>
    int Map<Tkey,Tval,Talloc>::Depth() const
    {
        return root ? root->Depth() : 0;
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Node* Map<Tkey,Tval,Talloc>::Find(Tkey& key, Node* n) const
    {
        if (!n) return 0;
        else if (n->pair.key == key) return n;
        else if (key < n->pair.key) return Find(key, n->left);
        return Find(key, n->right);
    }

    template<class Tkey, class Tval, class Talloc>
    typename Map<Tkey,Tval,Talloc>::Node* Map<Tkey,Tval,Talloc>::FindOrInsert(const Tkey& key, Node* n)
    {
        Node* newNode;

        if(n == root && !n)
        {
            // Tree is empty
            return root = new Node(0, Pair(key,Talloc::Allocate()));
        }
        else if (key == n->pair.key)
        {
            // Existing node found
            return n;
        }
        else if (key < n->pair.key)
        {
            if(n->left) return FindOrInsert(key, n->left);
            newNode = n->left = new Node(n, Pair(key,Talloc::Allocate()));
        }
        else
        {
            if(n->right) return FindOrInsert(key, n->right);
            newNode = n->right = new Node(n, Pair(key,Talloc::Allocate()));
        }

        newNode->red = true;
        newNode->Fix();

        while(root->parent)
        {
            root = root->parent;
        }

        return newNode;
    }

    template<class Tkey,class Tval,class Talloc>
    Tval& Map<Tkey,Tval,Talloc>::operator[](const Tkey& key)
    {
        Node* n = FindOrInsert(key, root);
        return n->pair.val;
    }

    template<class Tval> class Map<char*,Tval> : public Map<std::string,Tval>
    {
    };
}
