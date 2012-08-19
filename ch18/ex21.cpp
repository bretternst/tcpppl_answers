#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

const char* names[] = {
    "stan", "eric", "kyle", "kenny", "wendy", "gerald", "butters", "sheila", "bebe", "barbrady",
    "bradley", "clyde", "craig", "jimmy", "timmy", "token", "tweek", "randy", "sharon", "shelley",
    "marvin", "jimbo", "ike", "liane", "stuart", "carol", "stephen", "linda", "ned", "alphonse",
    "al", "terrance", "phillip", "harrison", "pip", "garrison", "mackey", "hankey", "towelie", "tuong"
};

int main() {
    using namespace std;

    int math_s[] = { 0, 2, 4, 6, 7, 8, 9, 12, 15, 16, 23, 28, 29, 30, 31, 33, 36, 37, 38, 39 };
    int english_s[] = { 1, 3, 5, 6, 7, 8, 10, 11, 13, 14, 15, 16, 17, 19, 24, 25, 26, 27, 28, 29 };
    int french_s[] = { 0, 1, 4, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 32, 34, 35, 36 };
    int biology_s[] = { 2, 3, 4, 5, 6, 11, 13, 23, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 };

    vector<const char*> all;
    for(int i = 0; i < 40; i++)
        all.push_back(names[i]);

    vector<const char*> math;
    vector<const char*> english;
    vector<const char*> french;
    vector<const char*> biology;
    for(int i = 0; i < 20; i++) {
        math.push_back(names[math_s[i]]);
        english.push_back(names[english_s[i]]);
        french.push_back(names[french_s[i]]);
        biology.push_back(names[biology_s[i]]);
    }
    sort(math.begin(), math.end());
    sort(english.begin(), english.end());
    sort(french.begin(), french.end());
    sort(biology.begin(), biology.end());

    ostream_iterator<const char*> out(cout, " ");
    cout << "Students taking both math and English:" << endl;
    set_intersection(math.begin(), math.end(), english.begin(), english.end(), out);
    cout << endl << endl;

    vector<const char*> tmp;
    cout << "Students taking French but not biology or math " << endl;
    set_union(biology.begin(), biology.end(), math.begin(), math.end(), back_inserter(tmp));
    set_difference(french.begin(), french.end(), tmp.begin(), tmp.end(), out);
    cout << endl << endl;
    tmp.clear();

    cout << "Students not taking a science course:" << endl;
    set_union(math.begin(), math.end(), biology.begin(), biology.end(), back_inserter(tmp));
    set_difference(all.begin(), all.end(), tmp.begin(), tmp.end(), out);
    cout << endl << endl;
    tmp.clear();

    vector<const char*> tmp2;
    set_union(french.begin(), french.end(), math.begin(), math.end(), back_inserter(tmp));
    set_union(english.begin(), english.end(), biology.begin(), biology.end(), back_inserter(tmp2));
    cout << "Students taking French and math but neither English nor biology:" << endl;
    set_difference(tmp.begin(), tmp.end(), tmp2.begin(), tmp2.end(), out);
    cout << endl;
}