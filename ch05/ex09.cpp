#include <iostream>
#include <string>

using namespace std;

int main()
{
    // This question is a bit unclear.
    // You might refer to the name of a type, for example, when
    // using a pointer to the same type inside itself (e.g. a linked list).
    struct node {
        node *next;
    };
}
