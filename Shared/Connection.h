#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include <queue>
#include "Message.h"

class Connection
{
public:
	Connection(SOCKET);
	~Connection();

	// Sending
	void sendMessage(Message);

	// Receiving
	bool hasRecvMessages() const;
	Message popRecvQueue();

private:
	SOCKET mSocket;
	std::queue<Message> mSendQueue;
	std::mutex mSendMtx;
	std::queue<Message> mRecvQueue;
	std::mutex mRecvMtx;

	void sendMessages();
	void recvMessages();
};