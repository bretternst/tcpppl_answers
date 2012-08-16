#include <algorithm>
#include <list>
#include <string>
#include <iostream>

namespace ch18 {
    using namespace std;

    class Person {
    public:
        string name;
        Person(const string& n) : name(n) { }
    };

    class Club {
    public:
        string name;
        list<Person*> members;
        list<Person*> officers;

        Club(const string& n) : name(n) { }
    };

    class Extract_officers {
        list<Person*>& lst;
    public:
        Extract_officers(list<Person*>& x) : lst(x) { }
        void operator()(const Club& c) const {
            copy(c.officers.begin(), c.officers.end(), back_inserter(lst));
        }
    };

    void extract(const list<Club>& lc, list<Person*>& off) {
        for_each(lc.begin(), lc.end(), Extract_officers(off));
    }

    class Print_name {
        ostream& out;
    public:
        Print_name(ostream& out) : out(out) { }
        void operator()(const Person* p) {
            out << p->name << endl;
        }
    };

    void extract_and_print(const list<Club>& lc) {
        list<Person*> off;
        extract(lc, off);
        for_each(off.begin(), off.end(), Print_name(cout));
    }
}

int main() {
    using namespace ch18;
    using namespace std;

    Person* people[] = { new Person("Bob"), new Person("James"), new Person("Frank") };
    list<Club> clubs;
    Club c("club1");
    c.officers.push_back(people[0]);
    c.officers.push_back(people[1]);
    clubs.push_back(c);
    c = Club("club2");
    c.officers.push_back(people[2]);
    clubs.push_back(c);

    extract_and_print(clubs);
}