#include <iostream>

// I couldn't think of a good way to combine the two functions
// without introducing ambiguity or restrictions on the arguments.
namespace ch18 {
    template<class In, class Pred>
    In find_if(In first, In last, Pred pred) {
        while(first < last) {
            if(pred(*first))
                break;
            first++;
        }
        return first;
    }

    template<class In, class T>
    In find(In first, In last, const T& value) {
        return find_if(first, last, bind2nd(std::equal_to<int>(), value));
    }
}

int main() {
    using namespace std;

    int arr[] = { 1, 2, 3, 4, 5 };
    int* p = ch18::find(arr, arr + 5, 3);
    cout << *p << endl;
}
