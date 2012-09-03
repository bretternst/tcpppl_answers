#include <iostream>
#include <string>
#include <string.h>

using namespace std;

struct Tnode
{
    char* word;
    int count;
    Tnode* left;
    Tnode* right;
};

Tnode* addNode(Tnode* root, string word)
{
    Tnode* node;
    if (!root)
    {
        node = new Tnode;
        node->word = new char[word.size()+1];
        strcpy(node->word, word.c_str());
        node->left = 0;
        node->right = 0;
        node->count = 1;
        return node;
    }

    int cmp = strcmp(word.c_str(), root->word);
    if (cmp < 0) {
        node = addNode(root->left, word);
        if(!root->left) root->left = node;
    }
    else if (cmp > 0) {
        node = addNode(root->right, word);
        if(!root->right) root->right = node;
    }
    else
        root->count++;

    return node;
}

void printTree(Tnode* root, int indent)
{
    if(!root) return;

    cout << string(indent, ' ') << root->word << "[" << root->count << "]" << endl;
    printTree(root->left, indent+4);
    printTree(root->right, indent+4);
}

void aPrintTree(Tnode* root)
{
    if(!root) return;

    aPrintTree(root->left);
    cout << root->word << "[" << root->count << "]" << endl;
    aPrintTree(root->right);
}

void printTree(Tnode* root)
{
    printTree(root, 0);
}

void freeTree(Tnode* root)
{
    if(!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root->word;
    delete root;
}

int main(int argc, char* argv[])
{
    Tnode* root = 0;

    string word;

    for(int i = 1; i < argc; i++)
    {
        word = string(argv[i]);
        Tnode* node = addNode(root, word);
        if (!root) root=node;
    }

    cout << "Tree:" << endl;
    printTree(root);
    cout << "Alphabetical:" << endl;
    aPrintTree(root);

    freeTree(root);
}
