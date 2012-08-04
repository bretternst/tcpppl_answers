#include "scheduler.h"
#include "task.h"

#include <iostream>

namespace evtsim
{
    void Scheduler::add_task(Task *t) {
        tasks.push(t);
    }
    void add_task(Task* t) { tasks.push(t); }

    void Scheduler::run()
    {
        ucontext_t context;
        getcontext(&context)

        while(tasks.size() > 0)
        {
            Task* t = tasks.front();
            tasks.pop();
            switch(t->get_state())
            {
            case Task::DONE:
            case Task::ERR:
                continue;
            case Task::IDLE:
                t->schedule();
                if(t->get_state()!=Task::RUNNING)
                {
                    tasks.push(t);
                    continue;
                }
            case Task::RUNNING:
                t->restore();
                tasks.push(t);
                break;
            }
        }
    }

    void Scheduler::yield_return()
    {
        s_sched_init.SwitchToPrimary();
    }
}
