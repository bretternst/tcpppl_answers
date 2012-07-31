#include <iostream>
#include <string>

namespace Exercises
{
	using std::string;
	using std::cout;
	using std::endl;
	using std::ostream;

	class Tnode
	{
		string m_word;
		int m_count;
		friend class Tree;

		void add_word(string w)
		{
			if (w < m_word)
			{
				if (!left) left = new Tnode(w);
				else left->add_word(w);
			}
			else if (w > m_word)
			{
				if (!right) right = new Tnode(w);
				else right->add_word(w);
			}
			else m_count++;
		}

	public:
		Tnode* left;
		Tnode* right;

		Tnode(string word) : m_count(1), left(0), right(0), m_word(word) { }
		~Tnode()
		{
			if(left) delete left;
			if(right) delete right;
		}
		const string& word() const { return m_word; }
		const int count() const { return m_count; }
	};

	class Tree
	{
		Tnode* root;

		void print(ostream& output, Tnode* node, int indent) const
		{
			if(!node) return;

			cout << string(indent, ' ') << node->word() << "[" << node->count() << "]" << endl;
			print(output, node->left, indent+1);
			print(output, node->right, indent+1);
		}

		void print_sorted(ostream& output, Tnode* node) const
		{
			if(!node) return;

			print_sorted(output, node->left);
			cout << node->word() << "[" << node->count() << "]" << endl;
			print_sorted(output, node->right);
		}

	public:
		Tree() : root(0) { }
		~Tree() { if(root) delete root; }

		void print(ostream& output) const
		{
			print(output, root, 0);
		}

		void print_sorted(ostream& output) const
		{
			print_sorted(output, root);
		}

		void add_word(string w)
		{
			if(!root) root = new Tnode(w);
			else root->add_word(w);
		}
	};
}

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace Exercises;

	Tree tree;

	string word;
	for(int i = 1; i < argc; i++)
	{
		word = string(argv[i]);
		tree.add_word(word);
	}

	cout << "Tree:" << endl;
	tree.print(cout);
	cout << "Alphabetical:" << endl;
	tree.print_sorted(cout);
}
