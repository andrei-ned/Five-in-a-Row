#include "Message.h"
#include <string>

Message::Message(const char* buf)
{
	strcpy_s(mBuffer, sBufferSize, buf);
}

const char* Message::getBuffer() const
{
	return mBuffer;
}