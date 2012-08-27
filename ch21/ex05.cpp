#include <iostream>
#include <sstream>

namespace ch21 {
    template<class T>
    bool parse(std::string s, T& x) {
        std::istringstream in(s);
        return in >> x;
    }

    bool parse(std::string s, std::string& x) {
        x = s;
        return true;
    }

    template<class T>
    T prompt(const char* prompt, bool allowEmpty = false) {
        T result;
        for(;;) {
            std::cout << prompt;
            std::string s;
            std::getline(std::cin, s);
            if(allowEmpty && s.empty()
               || (!s.empty() && parse(s, result)))
                return result;
        }
    }

    struct Mail_address {
        std::string address1;
        std::string address2;
        std::string city;
        std::string state;
        unsigned int zip;
    };

    std::ostream& operator<<(std::ostream& out, const Mail_address& x) {
        out << x.address1 << '\n';
        if (!x.address2.empty())
            out << x.address2 << '\n';
        out << x.city << ", " << x.state << ' ' << x.zip;
    }

    Mail_address prompt_mail_address() {
        Mail_address x;
        x.address1 = prompt<std::string>("Address Line 1: ");
        x.address2 = prompt<std::string>("Address Line 2 (blank for none): ", true);
        x.city = prompt<std::string>("City: ");
        x.state = prompt<std::string>("State: ");
        x.zip = prompt<int>("Zip: ");
        return x;
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    // stop everything if cin closes
    cin.exceptions(ios::eofbit);

    int i = prompt<int>("Enter an int: ");
    cout << "You entered: " << i << endl;

    float f = prompt<float>("Enter a float: ");
    cout << "You entered: " << f << endl;

    Mail_address m = prompt_mail_address();
    cout << "You entered: " << '\n' << m << endl;
}
