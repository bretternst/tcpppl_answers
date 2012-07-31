#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <queue>
#include <map>

#include "Task.h"
#include "Queue.h"

namespace EventSimulator
{
	typedef std::map<std::string,Queue> QueueMap;

	class Scheduler
	{
		TaskList tasks;
		QueueMap queues;
		int vTime;

	public:
		Scheduler() : vTime(0) {}

		Queue* GetQueue(std::string name) { return &queues[name]; }
		void AddTask(Task* t) { tasks.push(t); }
		int Elapsed() { return vTime; }
		void Run();

		void YieldReturn();
		void AddTime(int t) { vTime += t; }
	};
}

#endif
