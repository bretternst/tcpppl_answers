#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

const int maxLevel = 65535;
int throwLevel;

// on my box, using g++, and based on highly unscientific measurements,
// it appears to take about 0.01 seconds per additional 1000 levels of stack
// to unwind the stack when an exception is thrown.

struct LevelReachedError
{
    string funcName;
    int start;

    LevelReachedError(string f)
    {
        funcName=f;
        start = clock();
    }
};

void b(int level);

void a(int level)
{
    if(level==throwLevel)
        throw LevelReachedError("a");
    if(level >= maxLevel) return;
    b(level+1);
}

void b(int level)
{
    if(level==throwLevel)
        throw LevelReachedError("b");
    if(level >= maxLevel) return;
    a(level+1);
}

int main(int argc, char* argv[])
{
    if(argc<2) return -1;
    throwLevel = atoi(argv[1]);

    cout << "Clocks per sec: " << CLOCKS_PER_SEC << endl;
    try
    {
        if(throwLevel==0) throw LevelReachedError("main");
        a(1);
    }
    catch(LevelReachedError err)
    {
        cout << "calling level reached in function " + err.funcName << endl;
        cout << "unwind time was " << clock() - err.start << endl;
    }
}
