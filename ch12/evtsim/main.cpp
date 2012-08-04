#include <iostream>

#include "scheduler.h"
#include "task.h"

using namespace std;
using namespace evtsim;

class Task1 : public Task
{
public:
    Task1(Scheduler* s) : Task(s)
    {
    }

    void run()
    {
        cout << "1: running..." << endl;
        subscribe("test");
        delay(5);
        wait_message();
        cout << "1: Received message." << get_message("test").msg << endl;
        delay(10);
        unsubscribe("test");
        send_message("test2", Message(45678, 0));
        send_message("test2", Message(99999, 0));
    }
};

class Task2 : public Task
{
public:
    Task2(Scheduler* s) : Task(s)
    {
    }

    void run()
    {
        cout << "2: running..." << endl;
        send_message("test", Message(12345, 0));
        subscribe("test2");
        wait_message();
        cout << "2: Received message." << get_message("test2").msg << endl;
        unsubscribe("test2");
    }
};

class Task3 : public Task
{
public:
    Task3(Scheduler* s) : Task(s)
    {
    }

    void run()
    {
        subscribe("test");
        subscribe("test2");
        wait_message();
        cout << "3: Received message." << endl;
        wait_message();
        cout << "3: Received message." << endl;
        delay(20);
    }
};

class TestException {};

class Task4 : public Task
{
public:
    Task4(Scheduler* s) : Task(s)
    {
    }

    void run()
    {
        cout << "4: Throwing exception" << endl;
        throw TestException();
    }
};

int main()
{
    Scheduler s;
    Task3 t3(&s);
    s.add_task(&t3);
    Task1 t1(&s);
    s.add_task(&t1);
    Task2 t2(&s);
    s.add_task(&t2);
    Task4 t4(&s);
    s.add_task(&t4);

    s.run();

    cout << "Total elapsed virtual time: " << s.elapsed() << endl;

    return 0;
}
