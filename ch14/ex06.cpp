#include <iostream>
#include <vector>

#include "Vector.h"

template<class T1, class T2> void CompareVec(T1& v1, T2& v2)
{
    using namespace std;
    static int count = 0;

    cout << count++ << ": ";

    if(v1.size() != v2.size())
    {
        cout << "sizes don't match: " << v1.size() << " vs. " << v2.size() << endl;
        return;
    }
    for(int i = 0; i < v1.size(); i++)
    {
        if(v1[i] != v2[i])
        {
            cout << "element " << i << " doesn't match" << endl;
            return;
        }
    }
    cout << "match" << endl;
}

template<class T> void PrintVec(T& vec)
{
    using namespace std;

    for(int i = 0; i < vec.size(); i++)
        cout << vec[i] << " ";
    cout << endl;
}

int main()
{
    using namespace std;
    using namespace Exercises;

    vector<int> tmpl;
    tmpl.push_back(1);
    tmpl.push_back(2);
    tmpl.push_back(3);
    tmpl.push_back(4);
    tmpl.push_back(5);

    Vector<int> V1(tmpl.begin(), tmpl.end());
    vector<int> v1(tmpl.begin(), tmpl.end());
    CompareVec(V1,v1);

    Vector<int> V2(V1.begin(), V1.end());
    CompareVec(V2,v1);

    Vector<int> V3(V1.rbegin(), V1.rend());
    vector<int> v3(v1.rbegin(), v1.rend());
    CompareVec(V3,v3);

    try
    {
        cout << V3.at(5) << endl;
    }
    catch(Vector<int>::RangeError)
    {
        cout << "caught range error" << endl;
    }

    V3.clear();
    try
    {
        cout << V3.front() << endl;
    }
    catch(Vector<int>::SizeError)
    {
        cout << "caught size error" << endl;
    }

    V1.erase(V1.begin()+2);
    v1.erase(v1.begin()+2);
    CompareVec(V1,v1);

    V1.erase(V1.begin()+1,V1.end());
    v1.erase(v1.begin()+1,v1.end());
    CompareVec(V1,v1);

    V1 = Vector<int>(tmpl.begin(),tmpl.end());
    V2 = Vector<int>(tmpl.begin(),tmpl.end());
    v1 = vector<int>(tmpl.begin(),tmpl.end());
    vector<int> v2 = vector<int>(tmpl.begin(),tmpl.end());

    V1.insert(V1.begin()+2,3,2);
    v1.insert(v1.begin()+2,3,2);
    CompareVec(V1,v1);

    V1.insert(V1.begin()+3,V2.begin(),V2.end());
    v1.insert(v1.begin()+3,v2.begin(),v2.end());
    CompareVec(V1,v1);

    int q = 5;
    int r = 7;
    Vector<int*> Vp;
    Vp.push_back(&q);
    Vp.push_back(&r);
    Vp.push_back(&q);
    Vp.push_back(&q);
    Vp.push_back(&r);
    Vp.pop_back();

    vector<int*> vp;
    vp.push_back(&q);
    vp.push_back(&r);
    vp.push_back(&q);
    vp.push_back(&q);
    vp.push_back(&r);
    vp.pop_back();

    Vp.insert(Vp.begin()+2,&r);
    vp.insert(vp.begin()+2,&r);

    CompareVec(Vp,vp);

    return 0;
}
