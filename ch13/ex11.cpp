#include <iostream>
#include <vector>

#include "Map.h"

namespace Exercises
{
	template<class T> struct Edge
	{
		T n1;
		T n2;

		Edge(const T& n1, const T& n2) : n1(n1),n2(n2) {}
	};

	template<class T> void TopologicalSort(std::vector<Edge<T> >& edges, std::vector<T>& list)
	{
		typedef std::vector<Edge<T> > EdgeList;
		typedef std::vector<T> NodeList;
		typedef Map<T,NodeList> NodeMap;

		NodeMap forward;
		Map<T,int> backward;

		for(EdgeList::const_iterator i = edges.begin(); i != edges.end(); i++)
		{
			forward[i->n1].push_back(i->n2);
			backward[i->n1];
			backward[i->n2]++;
		}

		list.clear();

		bool done;
		do
		{
			done = true;
			for(Map<T,int>::Iterator i = backward.Begin(); i != backward.End(); i++)
			{
				if(i->val < 0) continue;
				if(i->val == 0)
				{
					list.push_back(i->key);
					NodeList& targets = forward[i->key];
					for(NodeList::iterator j = targets.begin(); j != targets.end(); j++)
					{
						backward[*j]--;
					}
					i->val--;
				}
				else if (i->val > 0)
				{
					done = false;
				}
			}
		} while(!done);
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	std::vector<Edge<int> > edges;
	edges.push_back(Edge<int>(7,11));
	edges.push_back(Edge<int>(7,8));
	edges.push_back(Edge<int>(5,11));
	edges.push_back(Edge<int>(11,2));
	edges.push_back(Edge<int>(11,9));
	edges.push_back(Edge<int>(3,8));
	edges.push_back(Edge<int>(3,10));
	edges.push_back(Edge<int>(8,9));
	edges.push_back(Edge<int>(11,10));

	std::vector<int> results;
	TopologicalSort(edges, results);

	for(std::vector<int>::const_iterator i = results.begin(); i != results.end(); i++)
	{
		cout << *i << ' ';
	}
	cout << endl;

	return 0;
}
