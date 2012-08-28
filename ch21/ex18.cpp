#include <iostream>
#include <string>
#include <algorithm>

namespace ch21 {
    class Based {
        friend std::ostream& operator<<(std::ostream& out, const Based& x);
        int base;
        int val;
    public:
        Based(int base, int val) : base(base), val(val) { }
        void put(std::ostream& out) const;
    };

    // Rather than using the built in base manipulators, do the conversion here
    // so that any base is supported (not just 8, 10, 16).
    void Based::put(std::ostream& out) const {
        if(val == 0)
            out << '0';
        else {
            int i = val;
            bool neg = false;
            if(i < 0) {
                neg = true;
                i = -i;
            }
            std::string tmp;
            while(i) {
                int r = i % base;
                if(base > 10 && r > 9)
                    tmp.push_back('a' + (r - 10));
                else
                    tmp.push_back('0' + r);
                i /= base;
            }
            if(neg)
                tmp.push_back('-');
            std::reverse(tmp.begin(), tmp.end());
            out << tmp;
        }
    }

    Based based(int base, int val) {
        return Based(base, val);
    }

    std::ostream& operator<<(std::ostream& out, const Based& x) {
        x.put(out);
        return out;
    }
}

int main() {
    using namespace std;
    using namespace ch21;

    cout << based(2, 9) << ' ' << based(8, 01234) << ' ' << based(16, 0x1234ab) << endl; // 1001 1234 1234ab
}
