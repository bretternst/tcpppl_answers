#include "scheduler.h"
#include "task.h"

#include <iostream>

namespace evtsim
{
    extern "C" void run_fiber(Task *t)
    {
        try
        {
            t->run();
            t->state = Task::DONE;
        }
        catch(...)
        {
            t->state = Task::ERR;
        }
        t->sched->yield_return(0);
    }

    const int StackSize = 16384;

    void Scheduler::add_task(Task *t) {
        t->stack = malloc(StackSize);
        getcontext(&t->context);
        t->context.uc_stack.ss_sp = t->stack;
        t->context.uc_stack.ss_size = StackSize;
        t->context.uc_link = &context;
        makecontext(&t->context, (void (*)())run_fiber, 1, t);
        tasks.push(t);
    }

    void Scheduler::run()
    {
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
                if (swapcontext(&this->context, &t->context)) {
                    std::cerr << "swapcontext failed" << std::endl;
                    return;
                }
                tasks.push(t);
                break;
            }
        }
    }

    void Scheduler::yield_return(Task *t)
    {
        if(t) {
            if(swapcontext(&t->context, &this->context))
                std::cerr << "swapcontext failed" << std::endl;
        }
    }
}
