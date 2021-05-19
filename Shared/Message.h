#pragma once

enum class MessageType : char {
	None = 0,
	MatchStart = 'A'
};

class Message
{
	// TODO different msg types, byte array
public:
	Message(const char*);
	Message(MessageType);

	const char* getBuffer() const;
	MessageType getMessageType() const;

	static const int sBufferSize = 512;
private:
	char mBuffer[sBufferSize];
};

