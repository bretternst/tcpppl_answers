#include <iostream>
#include <string>

using namespace std;

int main()
{
    int arr[] = { 1, 2, 3, 4, 5 };

    cout << arr[2] << endl;
    /*
     * compiled with -O0
    movl    -24(%rbp), %eax
    movl    %eax, %esi
    movl    $_ZSt4cout, %edi
    call    _ZNSolsEi
    */
    /*
     * compmiled with -O2
    movl    $3, %esi
    movl    $_ZSt4cout, %edi
    call    _ZNSolsEi
    */

    cout << *(arr+2) << endl;
    /*
     * compiled with -O0
    movl    -24(%rbp), %eax
    movl    %eax, %esi
    movl    $_ZSt4cout, %edi
    call    _ZNSolsEi
    */
    /*
     * compiled with -O2
    movl    $3, %esi
    movl    $_ZSt4cout, %edi
    call    _ZNSolsEi
    */
}
