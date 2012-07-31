#include <iostream>

#include <windows.h>

#include "Scheduler.h"
#include "Task.h"

using namespace std;
using namespace EventSimulator;

namespace Exercises
{
	class Task1 : public Task
	{
	public:
		Task1(Scheduler* s) : Task(s)
		{
		}

		void Run()
		{
			cout << "1: Running..." << endl;
			Subscribe("test");
			Delay(5);
			WaitMessage();
			cout << "1: Received message." << GetMessage("test").msg << endl;
			Delay(10);
			Unsubscribe("test");
			SendMessage("test2", Message(45678, 0));
			SendMessage("test2", Message(99999, 0));
		}
	};

	class Task2 : public Task
	{
	public:
		Task2(Scheduler* s) : Task(s)
		{
		}

		void Run()
		{
			cout << "2: Running..." << endl;
			SendMessage("test", Message(12345, 0));
			Subscribe("test2");
			WaitMessage();
			cout << "2: Received message." << GetMessage("test2").msg << endl;
			Unsubscribe("test2");
		}
	};

	class Task3 : public Task
	{
	public:
		Task3(Scheduler* s) : Task(s)
		{
		}

		void Run()
		{
			Subscribe("test");
			Subscribe("test2");
			WaitMessage();
			cout << "3: Received message." << endl;
			WaitMessage();
			cout << "3: Received message." << endl;
			Delay(20);
		}
	};

	class TestException {};

	class Task4 : public Task
	{
	public:
		Task4(Scheduler* s) : Task(s)
		{
		}

		void Run()
		{
			cout << "4: Throwing exception" << endl;
			throw TestException();
		}
	};
}

int main()
{
	using namespace Exercises;

	Scheduler s;
	Task3 t3(&s);
	s.AddTask(&t3);
	Task1 t1(&s);
	s.AddTask(&t1);
	Task2 t2(&s);
	s.AddTask(&t2);
	Task4 t4(&s);
	s.AddTask(&t4);

	s.Run();

	cout << "Total elapsed virtual time: " << s.Elapsed() << endl;

	return 0;
}
