#include <iostream>
#include <string>

int main()
{
    using namespace std;

    try
    {
        // do stuff here
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

    return 0;
}
