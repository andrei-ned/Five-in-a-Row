#include "PlayerMove.h"
#include <cassert>

PlayerMove::PlayerMove(Message message)
{
	assert(message.getMessageType() == MessageType::PlayerMove);
	auto buffer = message.getBuffer();
	mContent[0] = buffer[1];
	mContent[1] = buffer[2];
	mContent[2] = 0;
}

PlayerMove::PlayerMove(char x, char y)
{
	mContent[0] = x + 1;
	mContent[1] = y + 1;
	mContent[2] = 0;
}

PlayerMove::PlayerMove(unsigned int x, unsigned int y)
{
	mContent[0] = static_cast<char>(x) + 1;
	mContent[1] = static_cast<char>(y) + 1;
	mContent[2] = 0;
}

char PlayerMove::getX() const
{
	return mContent[0] - 1;
}

char PlayerMove::getY() const
{
	return mContent[1] - 1;
}

void PlayerMove::setX(char x)
{
	mContent[0] = x + 1;
}

void PlayerMove::setX(unsigned int x)
{
	mContent[0] = static_cast<char>(x) + 1;
}

void PlayerMove::setY(char y)
{
	mContent[1] = y + 1;
}

void PlayerMove::setY(unsigned int y)
{
	mContent[1] = static_cast<char>(y) + 1;
}

const char* PlayerMove::getContent() const
{
	return mContent;
}