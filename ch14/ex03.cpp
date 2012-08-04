#include <iostream>

namespace ch14
{
    // A very simple binary tree for use in this exercise; it relies on the strings
    // pointed to by the nodes to be unchanging.
    class BinaryTree
    {
    public:
        struct Node
        {
            char* text;
            Node* left;
            Node* right;

            Node(char* s) : text(s), left(0), right(0) {}
            ~Node() { delete left; delete right; }
        };

    private:
        Node* root;

        void Add(Node* n, char* s)
        {
            int cmp = strcmp(s, n->text);
            if(cmp < 0)
            {
                if(n->left) Add(n->left, s);
                else n->left = new Node(s);
            }
            else if (cmp > 0)
            {
                if(n->right) Add(n->right, s);
                else n->right = new Node(s);
            }
        }

    public:
        BinaryTree() : root(0) {}
        ~BinaryTree() { delete root; }

        Node* Root() const { return root; }

        void Add(char* s)
        {
            if(!root) root = new Node(s);
            else Add(root, s);
        }
    };

    class NodeNotFoundError {};

    BinaryTree::Node* Find(const BinaryTree& bt, char* s)
    {
        BinaryTree::Node* n = bt.Root();
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
    bt.Add("abcd");
    bt.Add("abcx");
    bt.Add("hello");
    bt.Add("xxxx");
    bt.Add("habc");
    bt.Add("abcq");

    BinaryTree::Node* n = Find(bt, "hello");
    cout << n->text << endl; // should print hello

    try
    {
        n = Find(bt, "world");
    }
    catch(NodeNotFoundError)
    {
        cout << "node not found" << endl; // should execute
    }

    return 0;
}
