#include <memory>

struct Test { };

int main() {
    std::allocator<Test> a1;
    std::allocator<Test> a2(a1);
    // I copied it without using an assignment operator, do I win a prize?
}
