#pragma once

enum class MessageType : char {
	None = 0,
	MatchStart = 1,
	PlayerMove = 2,
	OpponentDisconnect = 3
};

class Message
{
public:
	Message(const char*);
	Message(MessageType);
	Message(MessageType, const char*);

	const char* getBuffer() const;
	MessageType getMessageType() const;

	static const int sBufferSize = 512;
private:
	char mBuffer[sBufferSize];
};

