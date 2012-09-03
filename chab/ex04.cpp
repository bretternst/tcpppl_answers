/* Stuff like:
#define X 5

void f(void* p) {
    int x = *((int*)p);
}

becomes:
*/

const int x = 5;

template<class T>
void f(T* val) {
    T x = *val;
}

int main() {
}

/* so on and so forth... */