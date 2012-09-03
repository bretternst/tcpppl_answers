#include "stdlib.h"

int main() {
    int new = 5;
    int* p = malloc(sizeof(int));
    free(p);
}
