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

        ucontext_t context;

    private:
        State state;
        Scheduler* sched;
        QueueList queues;

        static void run_fiber(Task* This);
    protected:
        Task(Scheduler* s);
        ~Task();
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
