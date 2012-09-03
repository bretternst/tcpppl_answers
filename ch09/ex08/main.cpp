#include <iostream>
#include <sstream>
#include "dc.h"

int main(int argc, char* argv[])
{
    using namespace std;

    istream* input;
    int no_of_errors = 0;

    switch(argc)
    {
    case 1:
        input = &cin;
        break;
    case 2:
        input = new istringstream(argv[1]);
        break;
    default:
        cerr << "too many arguments" << endl;
        return 1;
    }

    DeskCalc::run(input, &cout, &cerr);

    if (input != &cin) delete input;
    return no_of_errors;
}
