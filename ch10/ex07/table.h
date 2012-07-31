#include <iostream>
#include <string>

// In real code, you'd want to consider using something like vector or map as
// a backing store (or just use map directly) but this being an exercise, I
// implemented it manually instead.
// Since removing items is not required for this exercise, it is not implemented.
namespace Table
{
	using std::string;

	template<class T> class NameValuePair
	{
	public:
		string name;
		T value;
	};

	template<class T> class Table
	{
		NameValuePair<T>* p;
		int sz;
		int count;

		void extend()
		{
			int oldSize = sz;
			NameValuePair<T> *old = p;
			sz *= 2;
			p = new NameValuePair<T>[sz];
			for(int i = 0; i < oldSize; i++)
				p[i] = old[i];
			delete[] old;
		}

	public:
		Table(size_t sz) : count(0), sz(sz) { p = new NameValuePair<T>[sz]; }
		~Table() {delete[] p;}

		NameValuePair<T>* lookup(string name)
		{
			for(int i = 0; i < count; i++)
			{
				if(p[i].name==name) return &p[i];
			}
			return 0;
		}

        // works like lookup() except missing elements are automatically added
        // with their default constructors.
		T& operator[](string name)
		{
			NameValuePair<T>* item = lookup(name);
			if(!item)
			{
				if(count > sz) extend();

				p[count].name = name;
				p[count].value = T();
				return p[count++].value;
			}
			return item->value;
		}

		bool insert(char* name, T value)
		{
			if(lookup(name)) return false;
			if(count > sz) extend();

			p[count].name = name;
			p[count++].value = value;
			return true;
		}
	};
}
