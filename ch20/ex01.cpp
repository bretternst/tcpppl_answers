#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

namespace ch20 {
    using namespace std;

    string combine(const string& s1, const string& s2) {
        // could also have done: string res = s1 + '.' + s2;
        string res(s1);
        res.push_back('.');
        res.append(s2);
        return res;
    }

    // This method is a lot more prone to error, the developer has to take care to
    // allocate exactly the right number of bytes. it would be easy to forget the
    // null terminator. Also, the memory allocated cannot be freed inside the
    // function, so the caller would have to free it after it's finished with
    // the result. This is a recipe for memory leaks.
    // Determining the string length requires traversing the string more than
    // once (to find the null terminator). This is likely slower than a string
    // implementation because strings store their length explicitly.
    char* combine_c(const char* s1, const char* s2) {
        int len = strlen(s1);
        char* res = (char*)malloc(len + strlen(s2) + 2);
        strcpy(res, s1);
        res[len] = '.';
        strcpy(res + len + 1, s2);
        return res;
    }
}

int main() {
    using namespace ch20;
    cout << combine("file", "write") << endl;
    char* res = combine_c("file", "write");
    cout << res << endl;
    free(res);
}
