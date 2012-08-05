#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>

void handle_error() {
    abort();
}

using namespace std;

int main() try
{
    set_unexpected(handle_error);

    // do stuff here
    return 0;
}
catch(exception& ex)
{
    cout << "exception: " << ex.what() << endl;
    abort();
}
catch(...)
{
    cout << "unknown exception" << endl;
    abort();
}
