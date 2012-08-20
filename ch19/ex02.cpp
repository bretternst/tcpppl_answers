#include <iterator>

// A sink might be useful in situations where some iterator could be passed
// into a library function designed to write some kind of output. For example,
// say a library function takes an output iterator to write log entries to.
// If the consumer of the library does not wish to persist log entries, then
// a Sink may be passed in to perform a task without writing log output.
namespace ch18 {
    using namespace std;

    template<class T>
    class Sink : public iterator<output_iterator_tag, T> {
    public:
        Sink& operator++() { return *this; }
        Sink operator++(int) { return *this; }
        Sink& operator*() { return *this; }
        Sink& operator=(const T& x) { return *this; }
    };
}

int main() {
    using namespace std;
    using namespace ch18;

    int arr[] = { 1, 2, 3, 4, 5 };
    copy(arr, arr + 5, Sink<int>());
}
