#include <iostream>

int main() {
    // little endian platforms supporting non-aligned ints would print FFFF FF
    // x86 is such a platform
    int x[2] = { 0xffff, 0 };
    int *px = (int*)((char*)x+1);
    std::cout << std::hex << x[0] << " " << *px << std::endl;
}
