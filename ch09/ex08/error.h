#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>

namespace Error
{
    using std::string;
    using std::istream;

    struct SyntaxError
    {
        string errMsg;
        SyntaxError(string msg)
        {
            errMsg = msg;
        }
    };

    struct DivideByZeroError { };
    struct OverflowError { };

    void skip(istream*);
}

#endif
