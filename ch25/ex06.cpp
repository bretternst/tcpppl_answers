#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

namespace ch25 {
    class Filter {
    public:
        class Retry {
        public:
            virtual const char* message() { return 0; }
        };

        virtual void start() { }
        virtual int read() = 0;
        virtual void write() { }
        virtual void compute() { }
        virtual int result() = 0;
        virtual int retry(Retry& m) { std::cerr << m.message() << '\n'; return 2; }

        virtual ~Filter() { }
    };

    int main_loop(Filter* p) {
        for(;;) {
            try {
                p->start();
                while(p->read()) {
                    p->compute();
                    p->write();
                }
                return p->result();
            }
            catch(Filter::Retry& m) {
                if (int i = p->retry(m)) return i;
            }
            catch(...) {
                std::cerr << "Fatal filter error\n";
                return 1;
            }
        }
    }

    bool compare_value_desc(const std::pair<std::string, int>& x, const std::pair<std::string, int>& y) {
        return x.second > y.second;
    }

    class Count_filter : public Filter {
        std::istream& is;
        std::ostream& os;
        std::map<std::string, int> freq;

    public:
        Count_filter(std::istream& is, std::ostream& os) : is(is), os(os) { }
        
        int read() {
            std::string s;
            if (is >> s) {
                freq[s]++;
                return true;
            }
            return false;
        }

        typedef std::vector<std::pair<std::string,int> > totals_vector;

        int result() {
            totals_vector v;
            std::copy(freq.begin(), freq.end(), std::back_inserter(v));
            std::sort(v.begin(), v.end(), compare_value_desc);
            for(totals_vector::iterator i = v.begin(); i != v.end(); i++)
                std::cout << i->first << ": " << i->second << '\n';
            return 0;
        }
    };
}

int main() {
    using namespace ch25;
    using namespace std;

    Count_filter f(cin, cout);
    return main_loop(&f);
}