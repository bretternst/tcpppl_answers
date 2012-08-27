#include <iostream>
#include <string>
#include <sstream>


namespace ch21 {
    using namespace std;

    // Address format now delimited, e.g.
    // John Smith:123 Fake St.:Seattle:WA:98101
    struct Name_and_address {
        bool is_valid;
        string name;
        string address;
        string city;
        string state;
        unsigned int zip;
    };

    ostream& operator<<(ostream& out, const Name_and_address& x) {
        out << x.name << ':' << x.address << ':' << x.city << ':' << x.state << ':' << x.zip;
    }

    istream& operator>>(istream& in, Name_and_address& x) {
        Name_and_address tmp;
        string s;
        x.is_valid = false;
        while(in) {
            if(getline(in, s)) {
                istringstream is(s);
                if(getline(is, tmp.name, ':')
                    && getline(is, tmp.address, ':')
                    && getline(is, tmp.city, ':')
                    && getline(is, tmp.state, ':')
                    && (is >> tmp.zip))
                {
                    if(tmp.state.size() == 2) {
                        tmp.state[0] = toupper(tmp.state[0]);
                        tmp.state[1] = toupper(tmp.state[1]);
                    }
                    x = tmp;
                    x.is_valid = true;
                    break;
                }
            }
        }
        return in;
    }
}

int main() {
    using namespace ch21;

    Name_and_address x;
    while(cin >> x) {
        cout << x << endl;
    }
}
