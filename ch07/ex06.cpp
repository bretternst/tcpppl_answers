#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

struct User
{
    const char* name;
    const char* id;
    int dept;
};

User heads[] = {
    "Ritchie D.M.", "dmr", 11271,
    "Sethi R.", "ravi", 11272,
    "Szymanski T.G.", "tgs", 11273,
    "Schryer N.L.", "nls", 11274,
    "Schryer N.L.", "nls", 11275,
    "Kernighan B.W.", "bwk", 11276
};

void print_id(User* v, int n)
{
    for(int i=0;i<n;i++)
    {
        cout << v[i].name << '\t' << v[i].id << '\t' << v[i].dept << endl;
    }
}

int cmp1(const void* p, const void* q)
{
    return strcmp(static_cast<const User*>(p)->name, static_cast<const User *>(q)->name);
}

int cmp2(const void* p, const void* q)
{
    return static_cast<const User*>(p)->dept - static_cast<const User *>(q)->dept;
}

typedef int (*CFT)(const void*, const void*);

void swap(char *from, char *to, int size)
{
    for(int k = 0; k < size; k++)
    {
        char tmp = to[k];
        to[k] = from[k];
        from[k] = tmp;
    }
}

char* partition(char* start, char* end, int sz, CFT cmp)
{
    char* pivotIndex = start;
    char* index = start;
    char* i;

    swap(pivotIndex, end, sz);
    for(i = start; i < end; i+=sz)
    {
        if(cmp(i, end) < 0)
        {
            swap(i, index, sz);
            index += sz;
        }
    }
    swap(end, index, sz);
    return index;
}

void qsort__(char* start, char* end, int sz, CFT cmp)
{
    if (end > start)
    {
        char* i = partition(start, end, sz, cmp);
        qsort__(start, i-sz, sz, cmp);
        qsort__(i+sz, end, sz, cmp);
    }
}

void qsort_(void* base, size_t n, size_t sz, CFT cmp)
{
    qsort__(static_cast<char *>(base), static_cast<char *>(base)+(n*sz-sz), sz, cmp);
}

int main(int argc, char* argv[])
{
    cout << "Heads in alphabetical order:" << endl;
    qsort_(heads,6,sizeof(User),cmp1);
    print_id(heads,6);
    cout << "Heads in order of department number:" << endl;
    qsort_(heads,6,sizeof(User),cmp2);
    print_id(heads,6);
}
