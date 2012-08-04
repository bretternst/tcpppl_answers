#include <iostream>
#include <fstream>

using namespace std;

void invert(int* arr, int d1, int d2)
{
    int* tmp = new int[d1*d2];
    for(int i=0; i < d1*d2; i++) tmp[i] = arr[i];

    int idx = 0;
    for(int i=0; i < d2; i++)
    {
        for(int j = 0; j < d1; j++)
        {
            arr[idx++] = tmp[j*d2+i];
        }
    }
    cout << endl;
    delete tmp;
}

int main(int argc, char* argv[])
{
    int arr[3][5] = {
        {1,2,3,4,5},
        {11,12,13,14,15},
        {21,22,23,24,25}
    };
    int* p = &arr[0][0];
    for(int i = 0; i < 15; i++)
    {
        cout << *(p+i) << " ";
    }
    cout << endl;

    invert(p, 3, 5);
    for(int i = 0; i < 15; i++)
    {
        cout << *(p+i) << " ";
    }
    cout << endl;
}
