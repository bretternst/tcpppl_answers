#include <iostream>
#include <cstring>

namespace ch14
{
    // A very simple binary tree for use in this exercise; it relies on the strings
    // pointed to by the nodes to be unchanging.
    class BinaryTree
    {
    public:
        struct Node
        {
            const char* text;
            Node* left;
            Node* right;

            Node(const char* s) : text(s), left(0), right(0) {}
            ~Node() { delete left; delete right; }
        };

    private:
        Node* m_root;

        void add(Node* n, const char* s)
        {
            int cmp = strcmp(s, n->text);
            if(cmp < 0)
            {
                if(n->left) add(n->left, s);
                else n->left = new Node(s);
            }
            else if (cmp > 0)
            {
                if(n->right) add(n->right, s);
                else n->right = new Node(s);
            }
        }

    public:
        BinaryTree() : m_root(0) {}
        ~BinaryTree() { delete m_root; }

        Node* root() const { return m_root; }

        void add(const char* s)
        {
            if(!m_root) m_root = new Node(s);
            else add(m_root, s);
        }
    };

    class NodeNotFoundError {};

    BinaryTree::Node* find(const BinaryTree& bt, const char* s)
    {
        BinaryTree::Node* n = bt.root();
        while(n)
        {
            int cmp = strcmp(s,n->text);
            if(cmp == 0) return n;
            else if (cmp < 0) n = n->left;
            else if (cmp > 0) n = n->right;
        }
        throw NodeNotFoundError();
    }
}

int main()
{
    using namespace std;
    using namespace ch14;

    BinaryTree bt;
    bt.add("abcd");
    bt.add("abcx");
    bt.add("hello");
    bt.add("xxxx");
    bt.add("habc");
    bt.add("abcq");

    BinaryTree::Node* n = find(bt, "hello");
    cout << n->text << endl; // should print hello

    try
    {
        n = find(bt, "world");
    }
    catch(NodeNotFoundError)
    {
        cout << "node not found" << endl; // should execute
    }

    return 0;
}
