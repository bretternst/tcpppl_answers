#include <iostream>
using namespace std;

void print_char_vals (char start, char end) {
    for(char ch = start; ch <= end; ch++) {
        cout << ch << " = " << (int)ch << hex << " 0x" << (int)ch << dec << endl;
    }
}

int main()
{
    print_char_vals('a', 'z');
    print_char_vals('0', '9');
}
