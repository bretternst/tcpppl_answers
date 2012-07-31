#include <iostream>
#include <vector>
#include <algorithm>

namespace Exercises
{
	using std::vector;

	struct Record
	{
		int count;
		int price;

		Record() : count(0), price(0) {}
		Record(int count, int price) : count(count), price(price) {}
	};

	std::ostream& operator<<(std::ostream& output, const Record& r)
	{
		output << r.count << ',' << r.price;
		return output;
	}

	class CountComparer
	{
	public:
		static int Compare(Record a, Record b)
		{
			return a.count - b.count;
		}
	};

	class PriceComparer
	{
	public:
		static int Compare(Record a, Record b)
		{
			return a.price - b.price;
		}
	};

	// A simple bubble sort, just to differentiate this exercise from the next one
	// which calls for a qsort.
	template<class TCriteria> void Sort(vector<Record>& list)
	{
		bool done;
		do
		{
			done = true;
			for(unsigned int i = 0; i < list.size()-1; i++)
			{
				if(TCriteria::Compare(list[i],list[i+1]) > 0)
				{
					std::swap(list[i],list[i+1]);
					done = false;
				}
			}
		} while (!done);
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	vector<Record> list;
	list.push_back(Record(2, 4));
	list.push_back(Record(3, 1));
	list.push_back(Record(1, 5));
	list.push_back(Record(2, 3));

	cout << "Before sort: " << endl;
	for(vector<Record>::const_iterator i = list.begin(); i != list.end(); i++)
		cout << *i << endl;

	Sort<CountComparer>(list);
	cout << endl << "After sort by count: " << endl;
	for(vector<Record>::const_iterator i = list.begin(); i != list.end(); i++)
		cout << *i << endl;

	Sort<PriceComparer>(list);
	cout << endl << "After sort by price: " << endl;
	for(vector<Record>::const_iterator i = list.begin(); i != list.end(); i++)
		cout << *i << endl;

	return 0;
}