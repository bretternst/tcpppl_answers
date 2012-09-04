#include <locale>
#include <iostream>

namespace xxd {
    class numpunct_es : public std::numpunct<char> {
    public:
        explicit numpunct_es(int i = 0) : std::numpunct<char>(i) { }

        std::string do_truename() const { return "verdad"; }
        std::string do_falsename() const { return "falso"; }
    };
}

int main() {
    using namespace std;
    using namespace xxd;

    bool b;
    cout << "Enter bool as number: ";
    cin >> b;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cout << "You entered: " << b << endl;

    cout << "Enter bool as English word: ";
    cin >> boolalpha >> b;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cout << "You entered: " << boolalpha << b << endl;

    locale loc(locale(), new xxd::numpunct_es());
    cin.imbue(loc);
    cout.imbue(loc);
    cout << "Enter a bool as Spanish word: ";
    cin >> b;
    if(!cin) {
        cout << "invalid" << endl;
        return 1;
    }
    cout << "You entered: " << b << endl;
}
