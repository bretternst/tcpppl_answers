#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>

// This exercise is unclear to me; I don't really understand how
// inplace_merge applies because I'm not sure what the exercise
// is asking for. It just says "a variety of criteria", but
// page 541 specifies the use if inplace_merge when the list
// is already sorted by a couple of criteria, and only for a
// single overall criterion.
// Since the exercise makes little sense to me, I'll just
// implement the example given on page 541. Take a list
// sorted by species and then weight and sort the overall
// output by weight using inplace_merge.
namespace ch18 {
    using namespace std;

    struct Catch {
        string species;
        int length;
        int weight;
        int date;
        string fisherman;

        Catch(const string& species, int length, int weight, int date, const string& fisherman)
            : species(species), length(length), weight(weight), date(date), fisherman(fisherman) { }

        string to_str() const {
            ostringstream out;
            out << "Fisherman=" << fisherman << ", Species=" << species << ", Length=" << length
                << ", Weight=" << weight << ", Date=" << date;
            return out.str();
        }
    };

    void populate(vector<Catch>& v) {
        v.push_back(Catch("bass", 7, 1, 20120104, "joe"));
        v.push_back(Catch("catfish", 12, 3, 20120204, "frank"));
        v.push_back(Catch("salmon", 10, 2, 20120104, "joe"));
        v.push_back(Catch("salmon", 11, 3, 20120101, "joe"));
        v.push_back(Catch("swordfish", 15, 3, 20120115, "bill"));
        v.push_back(Catch("tuna", 7, 2, 20120204, "frank"));
        v.push_back(Catch("tuna", 6, 4, 20120203, "frank"));
    }

    void print(const Catch& c) {
        cout << c.to_str() << endl;
    }

    bool compare_weight(const Catch& x, const Catch& y) {
        return x.weight < y.weight;
    }
}

int main() {
    using namespace ch18;
    using namespace std;

    vector<Catch> catches;
    populate(catches);

    typedef vector<Catch>::iterator I;

    I start = catches.begin();
    while(start != catches.end()) {
        I i = start + 1;
        while(i != catches.end() && i->species == start->species)
            i++;
        inplace_merge(catches.begin(), start, i, compare_weight);
        start = i;
    }

    for_each(catches.begin(), catches.end(), print);
}
