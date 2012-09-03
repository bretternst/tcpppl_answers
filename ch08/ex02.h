#include <iostream>

namespace HelloWorld
{
    void SayHelloWorld();
}

void HelloWorld::SayHelloWorld()
{
    std::cout << "Hello world" << std::endl;
}

// This line would be added to make ex02.h backward-compatible with a program
// that didn't know about namespaces.
using HelloWorld::SayHelloWorld;
