#include <string>
#include <cmath>

template<class T> struct complex { T x, y; };

///////////////////////////////////////////////
char ch1;
char ch2 = 'a';

std::string s1;
std::string s2 = "hello";

int count1;
int count2 = 1;

// const can't be declared without declaration.
const double pi2 = 3.14;

extern int error_number1;
int error_number2 = 5;

const char* name1;
const char* name2 = "Brett";

const char* season1[4];
const char* season2[] = {"spring","summer","fall","winter"};

struct Date;
struct Date { int d, m, y; };

int day(Date*);
int day(Date* p){return p->d;}

double sqrt(double);
double sqrt(double d){return pow(d,0.5);}

template<class T> T abs1(T a) { return a < 0?-a : a; }

typedef complex<short> Point1;

struct User;
struct User { std::string name, password; };

enum Beer { Carlsberg, Tuborg, Thor };
enum Beer;

// namespace requires definition.
namespace NS2 { int a; }

int main()
{
}
