#include <iostream>
#include <map>
#include <string>

namespace ch13
{
    using std::cin;
    using std::cout;

    struct Count
    {
        int qty;
    };

    struct Price
    {
        // just an example, never use double for currency :)
        double dollarsAndCents;
    };

    struct Item
    {
        std::string name;
        Count count;
        Price price;
    };

    template<class T> void readline(T& x)
    {
        cin >> x;
    }

    template<> void readline<Count>(Count& x)
    {
        cout << "Enter the count: ";
        cin >> x.qty;
    }

    template<> void readline<Price>(Price& x)
    {
        cout << "Enter the price: ";
        cin >> x.dollarsAndCents;
    }

    template<> void readline<Item>(Item& x)
    {
        cout << "Enter the item description: ";
        std::getline(cin,x.name);
        readline(x.count);
        readline(x.price);
    }
}

int main()
{
    using namespace std;
    using namespace ch13;

    Item i;
    readline(i);

    cout << "Name: " << i.name << endl;
    cout << "Qty: " << i.count.qty << endl;
    cout << "Price: " << i.price.dollarsAndCents << endl;

    return 0;
}
