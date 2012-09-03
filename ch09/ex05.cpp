// An external include guard surrounds any include directives.
// Wherever this include exists, the guard must be placed around it.
#ifndef EX05_H
#include "ex05.h"
#define EX05_H
#endif

// The include (with external guard) is put here a second time to
// test that it works. If the guard did not work, the compiler would
// complain that the type Hello was declared twice.
#ifndef EX05_H
#include "ex05.h"
#define EX05_H
#endif

// Advantages to the external include guard:
// - The same include file would not be opened twice.
// - The same include file would not be parsed twice, even if it was only to process
//   its internal include guard.
// Disadvantages:
// - Every instance of the include must be surrounded by the guard.
// - Much copy/pasting would be required, leading to potential hard-to-diagnose errors.
// - Even includes inside includes would have to hold the guard, duplicating the guard
//   over and over instead of centralizing it.
// - A change to the name of the include file would require all guards to be carefully
//   replaced. Getting them out of sync would result in hard-to-diagnose errors.
int main()
{
}
