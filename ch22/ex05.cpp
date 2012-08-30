#include <valarray>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>

namespace ch22 {
    template<class T>
    std::ostream& operator<<(std::ostream& out, const std::valarray<T> x) {
        out << "[ ";
        std::copy(&x[0], &x[0]+x.size(), std::ostream_iterator<T>(out, " "));
        out << ']';
    }

    template<class T>
    std::istream& operator>>(std::istream& in, std::valarray<T>& x) {
        in.ignore(80, '[');
        if(!in)
            return in;
        // write into temporary to prevent modifying incompletely
        std::valarray<T> tmp(x);
        for(int i = 0; i < x.size(); i++) {
            in >> tmp[i];
            if(!in) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        in.ignore(80, ']');
        x = tmp;
        return in;
    }

    template<class T>
    std::valarray<T> get_array(std::istream& in) {
        size_t n;
        if(!(in >> n))
            return std::valarray<T>();
        std::valarray<T> tmp(n);
        in >> tmp;
        return tmp;
    }
}

int main() {
    using namespace std;
    using namespace ch22;

    cout << "Enter 5-number vararray, e.g. [1 2 3 4 5]: ";
    valarray<int> va(5);
    cin >> va;
    if(!cin) {
        cout << "invalid entry" << endl;
        cin.clear();
    }
    else
        cout << "You entered: " << va << endl;

    cin.ignore(80, '\n');

    cout << "Enter N-number valarray, e.g., 3[1 2 3]: ";
    valarray<int> va2 = get_array<int>(cin);
    if(!cin)
        cout << "invalid entry" << endl;
    else
        cout << "You entered: " << va2 << endl;
}
