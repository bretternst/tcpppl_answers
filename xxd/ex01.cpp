#include <locale>
#include <iostream>
#include <algorithm>

namespace xxd {
    enum Season { spring, summer, fall, winter };

    class Season_io : public std::locale::facet {
    public:
        Season_io(int i = 0) : std::locale::facet(i) { }
        ~Season_io() { }

        virtual const std::string& to_str(Season x) const = 0;
        virtual bool from_str(const std::string&, Season&) const = 0;

        static std::locale::id id;
    };

    std::locale::id Season_io::id;

    class ES_season_io : public Season_io {
        static const std::string seasons[];
    public:
        const std::string& to_str(Season) const;
        bool from_str(const std::string&, Season&) const;
    };

    const std::string ES_season_io::seasons[] = { "primavera", "verano", "otono", "invierno" };

    const std::string& ES_season_io::to_str(Season x) const {
        if(x < spring || x > winter) {
            static const std::string ss = "no-esta-temporada";
            return ss;
        }
        return seasons[x];
    }

    bool ES_season_io::from_str(const std::string& s, Season& x) const {
        const std::string* beg = &seasons[spring];
        const std::string* end = &seasons[winter] + 1;
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

    locale loc(locale(), new ES_season_io());
    cin.imbue(loc);
    cout.imbue(loc);

    Season x;
    cout << "Enter season in spanish: ";
    cin >> x;
    if(cin)
       cout << "You entered: " << x << endl;
    else
        cout << "invalid" << endl;
}
