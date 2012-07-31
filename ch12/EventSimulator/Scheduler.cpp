#include <windows.h>

#include "Scheduler.h"
#include "Task.h"

#include <iostream>

namespace EventSimulator
{
	class SchedulerInit
	{
		LPVOID primary;
	public:
		SchedulerInit()
		{
			primary = ConvertThreadToFiber(0);
		}

		void SwitchToPrimary() { SwitchToFiber(primary); }
	};
	SchedulerInit s_sched_init;

	void Scheduler::Run()
	{
		while(tasks.size() > 0)
		{
			Task* t = tasks.front();
			tasks.pop();
			switch(t->GetState())
			{
			case Task::DONE:
			case Task::ERR:
				continue;
			case Task::IDLE:
				t->Schedule();
				if(t->GetState()!=Task::RUNNING)
				{
					tasks.push(t);
					continue;
				}
			case Task::RUNNING:
				t->Restore();
				tasks.push(t);
				break;
			}
		}
	}

	void Scheduler::YieldReturn()
	{
		s_sched_init.SwitchToPrimary();
	}
}
