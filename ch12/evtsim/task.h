#ifndef __TASK_H__
#define __TASK_H__

#include <string>
#include <vector>
#include <ucontext.h>

#include "queue.h"

namespace evtsim
{
    class Scheduler;

    class Task
    {
    public:
        enum State { RUNNING, IDLE, DONE, ERR };

        State state;
        Scheduler* sched;
        friend class Scheduler;

    private:
        ucontext_t context;
        void *stack;
        QueueList queues;

    protected:
        Task(Scheduler* s);
        void set_state(State s) { state = s; }
        void subscribe(std::string queueName);
        void unsubscribe(std::string queueName);
        void send_message(std::string queueName, const Message& m);
        void wait_message();
        Message get_message(std::string queueName);
        void delay(int vTime);

    public:
        State get_state() { return state; }
        void schedule();

        virtual void run() = 0;
    };

    typedef std::queue<Task*> TaskList;
}

#endif
