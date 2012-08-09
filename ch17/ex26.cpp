#include <map>
#include "hash_map.h"

// Profiled using:
// g++ -o ex26 ex26.cpp -pg
// ./ex26
// gprof ./ex26

// After 10 million lookups and inserts with random keys from 0-100, these were the
// hot points. Nothing surprising here. The lesson is that the hash function can have
// a large impact on performance, since it is required both when inserting and during
// any lookup.
//
// Not surprisingly, the operator[]() function is also at the top, since it does the
// work of traversing elements in a bucket and the like.
//
//  %   cumulative   self              self     total
// time   seconds   seconds    calls  ms/call  ms/call  name
// 37.52      0.24     0.24 10001848     0.00     0.00  ch17::Hash<int>::operator()(int const&) const
// 37.52      0.48     0.24 10000000     0.00     0.00  ch17::hash_map<int, int, ch17::Hash<int>, std::equal_to<int>, std::allocator<std::pair<int const, int> > >::operator[](int const&)

// When using std::map, we see more work being done on internal functions, probably 
// traversing the red-black tree. However, the compare function (in this case, using std::less)
// is still up there, at #3. Map seems to spend more time doing traversal as opposed to
// comparing elements, which is not surprising, given the inherent differences between a tree
// and a hash table.
//  %   cumulative   self              self     total
// time   seconds   seconds    calls   s/call   s/call  name
// 32.61      1.27     1.27 10000000     0.00     0.00  std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_M_lower_bound(std::_Rb_tree_node<std::pair<int const, int> >*, std::_Rb_tree_node<std::pair<int const, int> >*, int const&)
// 16.05      1.90     0.63 102224440     0.00     0.00  std::_Rb_tree<int, std::pair<int const, int>, std::_Select1st<std::pair<int const, int> >, std::less<int>, std::allocator<std::pair<int const, int> > >::_S_key(std::_Rb_tree_node<std::pair<int const, int> > const*)
// 9.63       2.27     0.38 112226905     0.00     0.00  std::less<int>::operator()(int const&, int const&) const

template<class T>
void f(T& container) {
    srand(time(0));

    // profile inserts and lookups
    for(int i = 0; i < 10000000; i++) {
        container[rand() % 1000] = rand();
    }   
}

int main() {
    ch17::hash_map<int,int> m;
    //std::map<int,int> m;
    f(m);
}
