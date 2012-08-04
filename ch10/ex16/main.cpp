#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include "parser.h"

using namespace std;
using namespace Exercises;

void interactive()
{
    Calculator calc;
    calc.eval(&cin, &cout, &cerr);
}

string fromString(string expr)
{
    istringstream istr(expr);
    ostringstream ostr;
    Calculator calc;
    calc.eval(&istr, &ostr, &ostr);
    ostr.flush();
    return ostr.str();
}

void fromFile(char* inFileName, char* outFileName)
{
    ifstream infile(inFileName);
    ostream* out = &cout;
    if(outFileName)
    {
        out = new ofstream(outFileName);
    }
    Calculator calc;
    calc.eval(&infile, out, out);
    if (out != &cout) delete out;
}

// Usage:
// <executable>                                -- Interactive mode
// <executable> <expressions>                  -- Takes input from command line
// <executable> - inFile                       -- Takes input from file
// <executable> - inFile outFile               -- Takes input from file, sends output to file
int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        interactive();
    }
    else if(argc == 2)
    {
        string output = fromString(argv[1]);
        cout << output;
    }
    else if (argc >= 3 && strcmp(argv[1],"-")==0)
    {
        fromFile(argv[2], argc==4?argv[3]:0);
    }
}
