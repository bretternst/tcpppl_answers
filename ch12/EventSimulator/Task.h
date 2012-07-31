#ifndef __TASK_H__
#define __TASK_H__

#include <string>
#include <vector>
#include <windows.h>

#include "Queue.h"

namespace EventSimulator
{
	class Scheduler;

	class Task
	{
	public:
		enum State { RUNNING, IDLE, DONE, ERR };

	private:
		State state;
		Scheduler* sched;
		LPVOID fiber;
		QueueList queues;

		static void RunFiber(Task* This);
	protected:
		Task(Scheduler* s);
		~Task();
		void SetState(State s) { state = s; }
		void Subscribe(std::string queueName);
		void Unsubscribe(std::string queueName);
		void SendMessage(std::string queueName, const Message& m);
		void WaitMessage();
		Message GetMessage(std::string queueName);
		void Delay(int vTime);

	public:
		State GetState() { return state; }
		void Save();
		void Restore();
		void Schedule();

		virtual void Run() = 0;
	};

	typedef std::queue<Task*> TaskList;
}

#endif
