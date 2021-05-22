#include "Message.h"
#include <string>

Message::Message(const char* buf)
{
	strcpy_s(mBuffer, sBufferSize, buf);
}

Message::Message(MessageType type)
{
	mBuffer[0] = static_cast<char>(type);
	mBuffer[1] = 0;
}

Message::Message(MessageType type, const char* msgContent)
{
	mBuffer[0] = static_cast<char>(type);
	strcpy_s((mBuffer + 1), sBufferSize - 1, msgContent);
}

const char* Message::getBuffer() const
{
	return mBuffer;
}

MessageType Message::getMessageType() const
{
	return static_cast<MessageType>(mBuffer[0]);
}