#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <queue>
#include <algorithm>
#include <map>

namespace evtsim
{
    struct Message
    {
        int msg;
        void* data;

        Message(int msg, void* data) : msg(msg), data(data) {}
    };

    class Queue
    {
        std::queue<Message> messages;

    public:
        bool has_message() { return messages.size() > 0; }
        void enqueue(const Message& m) { messages.push(m); }
        Message dequeue() { Message m = messages.front(); messages.pop(); return m; }
        Message peek() { return messages.front(); }
    };

    typedef std::vector<Queue*> QueueList;
}

#endif
