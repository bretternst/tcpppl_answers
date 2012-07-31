#include <iostream>
#include <vector>
#include <string>

using namespace std;

// The predicted behavior was as follows - the ^ represents the iterator
// 1. start
// don't do this
//  ^
// 2. insert "don't" at iterator position
// don't don't do this
//  ^
// 3. increment iterator
// don't don't do this
//        ^
// 4. insert 'don't" at iterator position
// don't don't don't do this
//        ^
// etc. etc. - it'll keep growing
//
// However, that isn't all! Sooner or later, the vector needs to re-allocate
// memory. This would be implementation-dependent, but as soon as the reallocation
// happened, the iterator would not only be in an incorrect position but would
// be pointing to a memory location that has quite possibly been released. So
// on top of screwing up the vector, you might get heap corruption or an access
// violation. Basically, it results in UNDEFINED BEHAVIOR.

void duplicate(vector<string>& v)
{
	for(vector<string>::iterator i = v.begin(); i != v.end(); i++)
		v.insert(i, *i);
}

int main()
{
	vector<string> v;
	v.push_back("don't");
	v.push_back("do");
	v.push_back("this");

	dup(v);
}
