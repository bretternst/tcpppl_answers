#include <iostream>

namespace ch17 {
    template<class T>
    class Hash
    {
        typedef size_t (*hash_func)(const T& key);
        hash_func hash;
    public:
        Hash(hash_func hf) : hash(hf) {}

        size_t operator()(const T& key) const {
            size_t res = 0;
            while(size_t v = hash(key)) res = (res << 3) ^ v;
            return res;
        };
    };

    // Seems a little scary to have to store the position in a static field.
    // Very thread-unsafe. Did I miss something the exercise was hinting at?
    typedef const char* Pchar;
    size_t hash_cstr(const Pchar& s) {
        static int pos = 0;
        if(s[pos])
            return s[pos++];
        else {
            pos = 0;
            return 0;
        }
    }
}

int main() {
    using namespace std;
    using namespace ch17;

    Hash<const char*> h(hash_cstr);
    cout << h("Hello World") << endl;
}
