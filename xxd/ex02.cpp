#include <locale>
#include <iostream>
#include <algorithm>

namespace xxd {
    enum Season { spring, summer, fall, winter };

    class Season_io : public std::locale::facet {
        const std::string* names;
    public:
        Season_io(const std::string* names, int i = 0) : names(names), std::locale::facet(i) { }
        ~Season_io() { }

        const std::string& to_str(Season x) const;
        bool from_str(const std::string&, Season&) const;

        static std::locale::id id;
    };

    std::locale::id Season_io::id;

    const std::string& Season_io::to_str(Season x) const {
        if(x < spring || x > winter) {
            static const std::string ss = "no-such-season";
            return ss;
        }
        return names[x];
    }

    bool Season_io::from_str(const std::string& s, Season& x) const {
        const std::string* beg = &names[spring];
        const std::string* end = &names[winter] + 1;
        const std::string* p = std::find(beg, end, s);
        if(p == end)
            return false;
        x = Season(p-beg);
        return true;
    }

    std::ostream& operator<<(std::ostream& s, Season x) {
        const std::locale& loc = s.getloc();
        if(std::has_facet<Season_io>(loc))
            return s << std::use_facet<Season_io>(loc).to_str(x);
        else
            return s << int(x);
    }

    std::istream& operator>>(std::istream& s, Season& x) {
        const std::locale& loc = s.getloc();
        if(std::has_facet<Season_io>(loc)) {
            const Season_io& f = std::use_facet<Season_io>(loc);
            std::string buf;
            if(!(s>>buf && f.from_str(buf,x)))
                s.setstate(std::ios_base::failbit);
            return s;
        }

        int i;
        s >> i;
        x = Season(i);
        return s;
    }
}

int main() {
    using namespace std;
    using namespace xxd;

    const string seasons[] = { "spring", "summer", "fall", "winter" };

    locale loc(locale(), new Season_io(seasons));
    cin.imbue(loc);
    cout.imbue(loc);

    Season x;
    cout << "Enter season: ";
    cin >> x;
    if(cin)
       cout << "You entered: " << x << endl;
    else
        cout << "invalid" << endl;
}
