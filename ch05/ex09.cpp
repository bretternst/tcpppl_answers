#include <iostream>
#include <string>

using namespace std;

int main()
{
    // You might refer to the name of a type, for example, when using
    // a pointer to the same type inside its definition (e.g. a linked list).
    struct node {
        node *next;
    };

    // Name can be used in its own initializer
    // to create cycled graph data structures like this:
    node head = {&head};
}
