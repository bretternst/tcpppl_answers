/*
#include <stdlib.h>

struct X {
    int x;
};

int main() {
    X* p = (X*)malloc(sizeof(X));
    free(p);
}
*/

struct X {
    int x;
};

int main() {
    X* p = new X();
    delete p;
}