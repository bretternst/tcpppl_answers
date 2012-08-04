#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    for(int i = 1; i < argc; i++)
    {
        ifstream from(argv[i], ifstream::in);
        while (from.good())
        {
            cout << (char)from.get();
        }
        from.close();
    }
}
