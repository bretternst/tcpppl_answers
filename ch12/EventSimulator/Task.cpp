#include <algorithm>
#include <windows.h>

#include "Task.h"
#include "Scheduler.h"

namespace EventSimulator
{
	Task::Task(Scheduler* s) : state(RUNNING), sched(s)
	{
		fiber = CreateFiber(0, (LPFIBER_START_ROUTINE)&Task::RunFiber, this);
	}

	Task::~Task()
	{
		DeleteFiber(fiber);
	}

	void Task::Subscribe(std::string queueName)
	{
		Queue* q = sched->GetQueue(queueName);
		queues.push_back(q);
	}

	void Task::Unsubscribe(std::string queueName)
	{
		Queue* q = sched->GetQueue(queueName);
		queues.erase(find(queues.begin(),queues.end(),q));
	}

	void Task::SendMessage(std::string queueName, const Message& m)
	{
		Queue* q = sched->GetQueue(queueName);
		q->Enqueue(m);
	}

	void Task::WaitMessage()
	{
		state = IDLE;
		sched->YieldReturn();
	}

	Message Task::GetMessage(std::string queueName)
	{
		Queue* q = sched->GetQueue(queueName);
		Message& m = q->Dequeue();
		return m;
	}

	void Task::Delay(int vTime)
	{
		sched->AddTime(vTime);
	}

	void Task::Save()
	{
		sched->Run();
	}

	void Task::Restore()
	{
		SwitchToFiber(fiber);
	}

	void Task::Schedule()
	{
		state = IDLE;
		for(QueueList::const_iterator i = queues.begin(); i != queues.end(); i++)
		{
			if((*i)->HasMessage())
			{
				state = RUNNING;
				break;
			}
		}
	}

	void Task::RunFiber(Task* t)
	{
		try
		{
			t->Run();
			t->state = DONE;
		}
		catch(...)
		{
			t->state = ERR;
		}
		t->sched->YieldReturn();
	}
}
