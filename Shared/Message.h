#pragma once

class Message
{
	// TODO different msg types, byte array
public:
	Message(const char*);

	const char* getBuffer() const;

	static const int sBufferSize = 512;
private:
	char mBuffer[sBufferSize];
};