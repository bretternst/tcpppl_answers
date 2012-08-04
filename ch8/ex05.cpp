#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

const int maxLevel = 10;
int throwLevel;

struct LevelReachedError
{
    string funcName;

    LevelReachedError(string f)
    {
        funcName=f;
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

    try
    {
        if(throwLevel==0) throw LevelReachedError("main");
        a(1);
    }
    catch(LevelReachedError err)
    {
        cout << "calling level reached in function " + err.funcName << endl;
    }
}
