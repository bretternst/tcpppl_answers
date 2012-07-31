#include <iostream>

struct MemberA
{
public:
	int x;
	MemberA();
};

struct MemberB
{
	int x;
public:
	MemberB();
};

class ClassA
{
public:
	static MemberA x;
};

class ClassB
{
public:
	static MemberB x;
};

MemberA::MemberA()
{
	x = ClassB::x.x; // <- not yet constructed!
	std::cout << "MemberA constructor called!" << std::endl;
}

MemberB::MemberB()
{
	x = ClassA::x.x;
	std::cout << "MemberB constructor called!" << std::endl;
}

MemberA ClassA::x;
MemberB ClassB::x;

// At the time when MemberA's constructor is called, MemberB's
// constructor will not yet have been called. This means that 
// Class A's member's constructor may reference Class B's member, 
// but Class B's member is NOT yet initialized.
// Since ClassA and ClassB are in the same compilation unit, their
// static members will be constructed in the order they are defined.
//
// This is why dependencies between static fields should be avoided
// without careful attention to the order in which they are run.
// In general: don't do it!
int main()
{
}
