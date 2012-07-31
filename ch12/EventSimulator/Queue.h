#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <queue>
#include <algorithm>
#include <map>

namespace EventSimulator
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
		bool HasMessage() { return messages.size() > 0; }
		void Enqueue(const Message& m) { messages.push(m); }
		Message Dequeue() { Message m = messages.front(); messages.pop(); return m; }
		Message Peek() { return messages.front(); }
	};

	typedef std::vector<Queue*> QueueList;
}

#endif
