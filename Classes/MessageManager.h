#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

#include <string>
#include <vector>
#include <queue>

#include "MiscUtils.h"

struct Message
{
private:
	const std::string name;
public:
	Utils::VariablesSet variables;
public:
	Message(const std::string &n)
		: name(n)
	{}
	bool is(const std::string &n) const {
		return name == n;
	}
};

class MessageReceiver
{
public:
	virtual ~MessageReceiver(void) {}
	virtual void AcceptMessage(const Message &msg) = 0;
};

class MessageManager
{
private:
	std::vector<MessageReceiver *> _receivers;
	std::queue<Message> _messages;

public:
	static MessageManager& Instance(void);

	void PutMessage(const Message &msg);
	void FlushMessages(void);

	void RegisterReceiver(MessageReceiver *receiver);
	void UnregisterReceiver(MessageReceiver *receiver);
	void UnregisteraAllReceivers(void);

	void CallAcceptMessages(void);

private:
	~MessageManager(void);
	MessageManager(void);
	MessageManager(const MessageManager &);
	void operator=(const MessageManager &);
};

#endif
