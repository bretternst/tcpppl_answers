#include <iostream>

class AppInit
{
public:
    AppInit();
    ~AppInit();
} oAppInit;

AppInit::AppInit()
{
    std::cout << "Initialize" << std::endl;
}

AppInit::~AppInit()
{
    std::cout << "Clean up" << std::endl;
}

int main()
{
     std::cout << "Hello, world!\n";
}
