#include <iostream>
#include "windows.h"

namespace Exercises
{
    class Lock
    {
    private:
        HANDLE lockHandle;
        CRITICAL_SECTION lockSection;

        // Prevent copying
        Lock(const Lock& x);
        Lock& operator=(const Lock& x);

    public:
        Lock();
        ~Lock();
        void OpenLock();
        void CloseLock();
    };

    Lock::Lock()
    {
        lockHandle = CreateEvent(NULL, TRUE, TRUE, NULL);
        InitializeCriticalSection(&lockSection);
    }

    Lock::~Lock()
    {
        DeleteCriticalSection(&lockSection);
        CloseHandle(lockHandle);
    }

    void Lock::OpenLock()
    {
        EnterCriticalSection(&lockSection);
        WaitForSingleObject(lockHandle, INFINITE);
        ResetEvent(lockHandle);
        LeaveCriticalSection(&lockSection);
    }

    void Lock::CloseLock()
    {
        SetEvent(lockHandle);
    }
}

DWORD __stdcall f(LPVOID p)
{
    using namespace Exercises;

    Lock* l = static_cast<Lock*>(p);
    std::cout << "worker thread acquiring lock" << std::endl;
    l->OpenLock();
    std::cout << "worker thread acquired lock" << std::endl;
    Sleep(5000);
    std::cout << "worker thread releasing lock" << std::endl;
    l->CloseLock();
    return 0;
}

int main()
{
    using namespace std;
    using namespace Exercises;

    HANDLE thread;
    DWORD threadId;
    Lock* l = new Lock();

    thread = CreateThread(0, 0, &f, l, 0, &threadId);
    Sleep(1000);

    cout << "main thread acquiring lock" << endl;
    l->OpenLock();
    cout << "main thread acquired lock" << endl;
    // Do some locked stuff here
    std::cout << "main thread releasing lock" << endl;
    l->CloseLock();

    WaitForSingleObject(thread, INFINITE);

    delete l;

    return 0;
}
