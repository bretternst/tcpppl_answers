#include <algorithm>
#include <windows.h>

#include "task.h"
#include "scheduler.h"

namespace evtsim
{
    Task::Task(Scheduler *s, ucontext_t *context) : state(RUNNING), sched(s), context(context)
    {
        makecontext(
        context = s->create_context();
    }

    Task::~Task()
    {
        DeleteFiber(fiber);
    }

    void Task::subscribe(std::string queueName)
    {
        Queue* q = sched->GetQueue(queueName);
        queues.push_back(q);
    }

    void Task::unsubscribe(std::string queueName)
    {
        Queue* q = sched->get_queue(queueName);
        queues.erase(find(queues.begin(),queues.end(),q));
    }

    void Task::send_message(std::string queueName, const Message& m)
    {
        Queue* q = sched->get_queue(queueName);
        q->Enqueue(m);
    }

    void Task::wait_message()
    {
        state = IDLE;
        sched->yield_return();
    }

    Message Task::get_message(std::string queueName)
    {
        Queue* q = sched->get_queue(queueName);
        Message& m = q->dequeue();
        return m;
    }

    void Task::delay(int vTime)
    {
        sched->add_time(vTime);
    }

    void Task::schedule()
    {
        state = IDLE;
        for(QueueList::const_iterator i = queues.begin(); i != queues.end(); i++)
        {
            if((*i)->has_message())
            {
                state = RUNNING;
                break;
            }
        }
    }

    void Task::run_fiber(Task* t)
    {
        try
        {
            t->run();
            t->state = DONE;
        }
        catch(...)
        {
            t->state = ERR;
        }
        t->sched->yield_return();
    }
}
