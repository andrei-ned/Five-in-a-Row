#pragma once
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include <queue>
#include "Message.h"
#include <functional>

class Connection
{
public:
	Connection(SOCKET);
	~Connection();

	std::function<void()> mOnConnectionLost;
	bool isActive() const;

	// Sending
	void sendMessage(Message);

	// Receiving
	bool hasRecvMessages();
	Message popRecvQueue();

private:
	SOCKET mSocket;
	std::queue<Message> mSendQueue;
	std::mutex mSendMtx;
	std::thread mSendThread;
	std::queue<Message> mRecvQueue;
	std::mutex mRecvMtx;
	std::thread mRecvThread;
	std::atomic<bool> mConnectionActive;
	std::mutex dropConnMtx;

	void sendMessages();
	void recvMessages();

	void dropConnection();
};