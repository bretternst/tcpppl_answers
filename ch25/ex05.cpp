#include <iostream>

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

    class Dupe_filter : public Filter {
        std::istream& is;
        std::ostream& os;
        bool suppress;
        std::string cur;
    public:
        Dupe_filter(std::istream& is, std::ostream& os) : is(is), os(os) { }
        
        int read() {
            std::string s;
            if (is >> s) {
                suppress = s == cur;
                cur = s;
                return true;
            }
            return false;
        }

        void write() {
            if(!suppress)
                os << cur << ' ';
        }

        int result() {
            return 0;
        }
    };
}

int main() {
    using namespace ch25;
    using namespace std;

    Dupe_filter f(cin, cout);
    return main_loop(&f);
}