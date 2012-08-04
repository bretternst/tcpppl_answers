#include <iostream>

namespace ch12
{
    class Char_vec {
        int sz;
        char element[1];
    public:
        static Char_vec* new_char_vec(int s);
        char& operator[](int i) { return element[i]; }
    };

    // Evil.
    // This will severely break if the order of fields change, new fields are added,
    // virtual methods are added (because there would then be a vtable pointer and maybe
    // other fields from derived classes) and for any number of other reasons that I may
    // not be thinking of right now.
    //
    // You can also tell from the fact that there are two uses of reinterpret_cast that
    // this function is entirely evil.
    Char_vec* Char_vec::new_char_vec(int s)
    {
        char* p = new char[sizeof(int)+s];
        int* csz = reinterpret_cast<int*>(p);
        *csz = s;
        return reinterpret_cast<Char_vec*>(p);
    }
}

int main()
{
    using namespace std;
    using namespace ch12;

    Char_vec* cv = Char_vec::new_char_vec(10);
    (*(cv))[9] = 'x';
    cout << (*(cv))[9] << endl;
}
