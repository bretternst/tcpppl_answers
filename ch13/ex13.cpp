#include <iostream>
#include <map>
#include <string>

namespace Exercises
{
	using std::cin;
	using std::cout;

	struct Count
	{
		int qty;
	};

	struct Price
	{
		double dollarsAndCents;
	};

	struct Item
	{
		std::string name;
		Count count;
		Price price;
	};

	template<class T> void ReadLine(T& x)
	{
		input >> x;
	}

	template<> void ReadLine<Count>(Count& x)
	{
		cout << "Enter the count: ";
		cin >> x.qty;
	}

	template<> void ReadLine<Price>(Price& x)
	{
		cout << "Enter the price: ";
		cin >> x.dollarsAndCents;
	}

	template<> void ReadLine<Item>(Item& x)
	{
		cout << "Enter the item description: ";
		std::getline(cin,x.name);
		ReadLine(x.count);
		ReadLine(x.price);
	}
}

int main()
{
	using namespace std;
	using namespace Exercises;

	Item i;
	ReadLine(i);

	cout << "Name: " << i.name << endl;
	cout << "Qty: " << i.count.qty << endl;
	cout << "Price: " << i.price.dollarsAndCents << endl;

	return 0;
}
