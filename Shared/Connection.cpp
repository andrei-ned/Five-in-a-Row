#include "Connection.h"
#include <iostream>

Connection::Connection(SOCKET s) : 
	mSocket(s), 
	mSendThread(&Connection::sendMessages, this), 
	mRecvThread(&Connection::recvMessages, this), 
	mConnectionActive(true) 
{}

Connection::~Connection()
{
	closesocket(mSocket);
	WSACleanup();
	mConnectionActive = false;
	mSendThread.join();
	mRecvThread.join();
}

bool Connection::isActive() const
{
	return mConnectionActive;
}

// Sending
void Connection::sendMessage(Message msg)
{
	mSendMtx.lock();
	mSendQueue.push(msg);
	std::cout << "added msg to queue: " << msg.getBuffer() << "\n";
	mSendMtx.unlock();
}

// Receiving
bool Connection::hasRecvMessages()
{
	mRecvMtx.lock();
	bool isEmpty = mRecvQueue.empty();
	mRecvMtx.unlock();
	return !isEmpty;
}

Message Connection::popRecvQueue()
{
	mRecvMtx.lock();
	Message m = mRecvQueue.front();
	mRecvQueue.pop();
	mRecvMtx.unlock();
	return m;
}


void Connection::sendMessages()
{
	const char* buffer;
	int result;

	while (mConnectionActive)
	{
		mSendMtx.lock();
		bool isEmpty = mSendQueue.empty();
		mSendMtx.unlock();
		if (!isEmpty)
		{
			mSendMtx.lock();
			buffer = mSendQueue.front().getBuffer();
			mSendQueue.pop();
			std::cout << "popped from send queue: " << buffer << "\n";
			mSendMtx.unlock();


			result = send(mSocket, buffer, (int)strlen(buffer) + 1, 0);
			printf("send() result: %d\n", result);
			if (result == SOCKET_ERROR)
			{
				std::cout << "send() failed with error: " << WSAGetLastError() << "\n";
				mConnectionActive = false;
			}
		}
	}
}

void Connection::recvMessages()
{
	int result;
	char buffer[Message::sBufferSize];

	while (mConnectionActive)
	{
		result = recv(mSocket, buffer, Message::sBufferSize, 0);
		printf("recv() result: %d\n", result);
		if (result > 0)
		{
			printf("Received %d bytes\n", result);

			mRecvMtx.lock();
			char* p = buffer;
			do
			{
				int len = strlen(p) + 1;
				mRecvQueue.push(p);
				printf("Received msg of %d bytes: %s\n", len, p);
				result -= len;
				p += len;
			} while (result > 0);
			mRecvMtx.unlock();
		}
		else if (result == 0)
		{
			printf("Connection closing...\n");
			mConnectionActive = false;
		}
		else
		{
			printf("recv() failed with error: %d\n", WSAGetLastError());
			mConnectionActive = false;
		}
	}
}

void Connection::dropConnection()
{
	dropConnMtx.lock();
	if (!mConnectionActive)
	{
		dropConnMtx.unlock();
		return;
	}
	mConnectionActive = false;
	mOnConnectionLost();
	dropConnMtx.unlock();
}