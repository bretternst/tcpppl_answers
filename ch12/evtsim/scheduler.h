#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <queue>
#include <map>

#include "task.h"
#include "queue.h"
#include "ucontext.h"

namespace evtsim
{
    typedef std::map<std::string,Queue> QueueMap;

    class Scheduler
    {
        TaskList tasks;
        QueueMap queues;
        int vTime;
        ucontext_t context;

    public:
        Scheduler() : vTime(0) {}

        // called by consumers
        void add_task(Task *task);
        int elapsed() { return vTime; }
        void run();

        // called by tasks
        Queue* get_queue(std::string name) { return &queues[name]; }
        void yield_return(Task *t);
        void add_time(int t) { vTime += t; }
    };
}

#endif
