#include <iostream>

using namespace std;

// This is Duff's device. It "unrolls" a loop by performing 8 work items per iteration instead of one.
// The "remainder" of the count is handled by potentially starting in the middle of the loop on the first
// go (this is what the switch statement is for). Then the do/while loop takes over and performs the rest
// of the work, doing 8 copies at a time.
// Loop unrolling has speed benefits because executing sequential instructions that copy values is faster
// than doing many conditional jumps, which is what a loop does. This loop has to do up to 8 times fewer
// conditional jumps to perform the same work.
// Some compilers unroll loops automatically when feasible and when that optimization is enabled.
void send(int* to, int* from, int count)
{
    int n = (count+7)/8;
    switch(count%8)
    {
    case 0: do { *to++ = *from++;
    case 7:      *to++ = *from++;
    case 6:      *to++ = *from++;
    case 5:      *to++ = *from++;
    case 4:      *to++ = *from++;
    case 3:      *to++ = *from++;
    case 2:      *to++ = *from++;
    case 1:      *to++ = *from++;
            } while (--n>0);
    }
}

int main(int argc, char* argv[])
{
    int a[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    int b[20];

    send(b, a, 20);

    for(int i = 0; i < 20; i++)
        cout << b[i] << endl;
}
