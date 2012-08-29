#include <iostream>
#include <locale>
#include <sstream>

int main(int argc, char** argv) {
    using namespace std;

    locale loc;
    if(argc > 1) {
        // use "locale -a" to find valid locales for your system
        // e.g.: en_US.utf8
        // default is usually "C"
        loc = locale(argv[1]);
    }
    string s;
    tm t;
    t.tm_year = 2012;
    t.tm_mday = 31;
    t.tm_mon = 1;
    cout << "Enter a date (e.g. ";
    const time_put<char>& tmput = use_facet<time_put<char> >(loc);
    tmput.put(ostreambuf_iterator<char>(cout), cout, ' ', &t, 'x', 0);
    cout << "): ";

    const time_get<char>& tmget = use_facet<time_get<char> >(loc);
    ios::iostate state;
    tmget.get_date(istreambuf_iterator<char>(cin), istreambuf_iterator<char>(), cin, state, &t);

    if(state != ios::goodbit)
        cout << "error parsing date" << endl;
    else {
        cout << "You entered: ";
        tmput.put(ostreambuf_iterator<char>(cout), cout, ' ', &t, 'x', 0);
        cout << endl;
    }
}
